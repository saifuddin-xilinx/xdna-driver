# AMD XDNA Driver - HAL Layer Architecture

## Overview

This document describes the Hardware Abstraction Layer (HAL) architecture for the AMD XDNA driver. The HAL layer provides a unified interface that abstracts platform-specific operations, enabling support for multiple hardware platforms (AIE2/PCI, AIE4/PCI, VE2/Aux) through a common framework.

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    User Space (Applications)                 │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           │ DRM IOCTLs
                           │
┌──────────────────────────▼──────────────────────────────────┐
│              Common Framework (DRM Layer)                    │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  amdxdna_drm_create_hwctx_ioctl()                    │   │
│  │  amdxdna_drm_destroy_hwctx_ioctl()                   │   │
│  │  amdxdna_drm_submit_cmd_ioctl()                     │   │
│  │  amdxdna_drm_wait_cmd_ioctl()                       │   │
│  └──────────────────┬──────────────────────────────────┘   │
└──────────────────────┼──────────────────────────────────────┘
                       │
                       │ HAL Wrapper Functions
                       │
┌──────────────────────▼──────────────────────────────────────┐
│                    HAL Layer                                 │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  amdxdna_hal_hwctx_init()                            │   │
│  │  amdxdna_hal_hwctx_fini()                            │   │
│  │  amdxdna_hal_cmd_submit_job()                       │   │
│  │  amdxdna_hal_cmd_wait_ctx()                         │   │
│  └──────────────────┬──────────────────────────────────┘   │
└──────────────────────┼──────────────────────────────────────┘
                       │
                       │ Platform-specific Ops
                       │
┌──────────────────────┼──────────────────────────────────────┐
│                       │                                       │
│  ┌────────────────────▼────────────────────┐                 │
│  │      AIE2/PCI (aie2_pci.c)             │                 │
│  │  - aie2_dev_init/fini()                │                 │
│  │  - aie2_hwctx_init/fini()              │                 │
│  │  - aie2_sched_submit/wait()            │                 │
│  └────────────────────────────────────────┘                 │
│                                                               │
│  ┌────────────────────▼────────────────────┐                 │
│  │      AIE4/PCI (aie4_pci.c)             │                 │
│  │  - aie4_dev_init/fini()                │                 │
│  │  - aie4_hwctx_init/fini()              │                 │
│  │  - aie4_sched_submit/wait()            │                 │
│  └────────────────────────────────────────┘                 │
│                                                               │
│  ┌────────────────────▼────────────────────┐                 │
│  │      VE2/Aux (ve2_aux.c)               │                 │
│  │  - ve2_dev_init/fini()                 │                 │
│  │  - ve2_hwctx_init/fini()               │                 │
│  │  - ve2_sched_submit/wait()             │                 │
│  └────────────────────────────────────────┘                 │
│                                                               │
└───────────────────────────────────────────────────────────────┘
```

## Key Design Principles

1. **Unified Interface**: All platforms use the same HAL interface (`amdxdna_hal_dev_ops`)
2. **Seamless Integration**: HAL wrapper functions match the base API, enabling drop-in replacement
3. **Platform Abstraction**: Platform-specific code is isolated in separate files (aie2_pci.c, aie4_pci.c, ve2_aux.c)
4. **No Separate Types**: Uses `amdxdna_dev` and `amdxdna_ctx` directly (no HAL-specific types)

## Execution Flows

### Device Initialization

**Execution flow: Device Init**

```
PCI (AIE2/AIE4)                    Auxiliary (VE2)
│                                  │
│ Kernel binds PCI device          │ Kernel binds accel-vdev
↓                                  ↓
aie2_pci_probe() /                 amdxdna_aux_probe()
aie4_pci_probe()                   │
│                                  │
└──────────────┬───────────────────┘
               ↓
amdxdna_hal_dev_probe(dev, hal)
├─ Allocate amdxdna_dev
├─ Set xdna->hal_ops = hal->ops
├─ Initialize framework (DRM, etc.)
├─ Call ops->dev_init(xdna)
│  ↓
│  ┌────┴────┐
│  │         │
│  AIE2      VE2
│  │         │
│  ├─ Get parent (xilinx-aie-engine)
│  ├─ Parse device tree / config
│  └─ Store dev_handle
│
└─ Register with framework (accel, DRM, etc.)
Device ready
```

**Key Functions:**
- `amdxdna_hal_dev_probe()` - Called from PCI/Aux probe functions
- `ops->dev_init()` - Platform-specific device initialization

### Device Removal

**Execution flow: Device Fini**

```
PCI (AIE2/AIE4)                    Auxiliary (VE2)
│                                  │
aie2_pci_remove() /                amdxdna_aux_remove()
aie4_pci_remove()                  │
│                                  │
└──────────────┬───────────────────┘
               ↓
amdxdna_hal_dev_remove(xdna)
├─ Unregister from framework
├─ Call ops->dev_fini(xdna)
│  ↓
│  ┌────┴────┐
│  │         │
│  AIE2      VE2
│  │         │
│  ├─ Release partition refs
│  ├─ Cleanup dev_handle
│  └─ Teardown mailbox, unload firmware
│
└─ Free amdxdna_dev
Device gone
```

**Key Functions:**
- `amdxdna_hal_dev_remove()` - Called from PCI/Aux remove functions
- `ops->dev_fini()` - Platform-specific device cleanup

### Context Creation

**Execution flow: Context Creation**

```
User IOCTL (CREATE_HWCTX)
↓
amdxdna_drm_create_hwctx_ioctl()
├─ Allocate amdxdna_ctx
├─ Initialize id, state, locks, lists
├─ Copy QoS info from user space
├─ Allocate context ID from client's xarray
└─ Call amdxdna_hal_hwctx_init(ctx)
   ├─ Prepare config from context
   ├─ Call ops->hwctx_init(ctx, config)
   │  ↓
   │  ┌────┴────┐
   │  │         │
   │  AIE2      VE2
   │  │         │
   │  ├─ aie_partition_request()
   │  ├─ ve2_mgmt_ctx_init()
   │  ├─ ve2_mgmt_ctx_configure()
   │  └─ ve2_mgmt_ctx_start()
   │
   ├─ Call ops->sched_init(ctx)
   └─ Create syncobj
Return hwctx_handle to user
```

**Key Functions:**
- `amdxdna_drm_create_hwctx_ioctl()` - DRM IOCTL handler
- `amdxdna_hal_hwctx_init()` - HAL wrapper (replaces `ops->ctx_init()`)
- `ops->hwctx_init()` - Platform-specific context initialization
- `ops->sched_init()` - Platform-specific scheduler initialization

### Context Destruction

**Execution flow: Context Destruction**

```
User IOCTL (DESTROY_HWCTX)
↓
amdxdna_drm_destroy_hwctx_ioctl()
├─ Look up context from client's xarray
└─ Call amdxdna_ctx_destroy_rcu()
   └─ Call amdxdna_hal_hwctx_fini(ctx)
      ├─ Call ops->sched_fini(ctx)
      │  ↓
      │  ┌────┴────┐
      │  │         │
      │  AIE2      VE2
      │  │         │
      │  ├─ ve2_mgmt_ctx_stop()
      │  ├─ ve2_mgmt_ctx_fini()
      │  └─ aie_partition_release()
      │
      ├─ Call ops->hwctx_fini(ctx)
      └─ Destroy syncobj
Done
```

**Key Functions:**
- `amdxdna_drm_destroy_hwctx_ioctl()` - DRM IOCTL handler
- `amdxdna_hal_hwctx_fini()` - HAL wrapper (replaces `ops->ctx_fini()`)
- `ops->hwctx_fini()` - Platform-specific context cleanup
- `ops->sched_fini()` - Platform-specific scheduler cleanup

### Command Submission

**Execution flow: Command Submit**

```
User IOCTL (SUBMIT_CMD or execbuf)
↓
amdxdna_drm_submit_cmd_ioctl()
├─ Validate command type
└─ Call amdxdna_drm_submit_execbuf() / dependency() / signal()
   └─ Call amdxdna_cmd_submit()
      ├─ Allocate scheduler job
      ├─ Look up command BO and argument BOs
      ├─ Lock BOs
      ├─ Create fence
      └─ Call amdxdna_hal_cmd_submit_job(job, ...)
         ├─ Get ops from dev_info
         ├─ Validate hwctx, cmd, BOs
         ├─ Resolve addresses (resolver)
         └─ Call ops->sched_submit(ctx, cmd)
            ↓
            ┌────┴────┐
            │         │
            AIE2      VE2
            │         │
            ├─ Push cmd to partition queue
            ├─ ve2_sched_submit() → partition / DMA
            └─ Build message, aie2_send_msg_wait(SUBMIT_CMD)
Return seq / fence to user
```

**Key Functions:**
- `amdxdna_drm_submit_cmd_ioctl()` - DRM IOCTL handler
- `amdxdna_cmd_submit()` - Command submission helper
- `amdxdna_hal_cmd_submit_job()` - HAL wrapper (replaces `ops->cmd_submit()`)
- `ops->sched_submit()` - Platform-specific command submission

### Command Wait

**Execution flow: Command Wait**

```
User IOCTL (WAIT_CMD or sync)
↓
amdxdna_drm_wait_cmd_ioctl()
└─ Call amdxdna_cmd_wait()
   ├─ Look up context from client's xarray
   └─ Call amdxdna_hal_cmd_wait_ctx(ctx, seq, timeout)
      ├─ Get ops from dev_info
      ├─ Look up hwctx, validate seq
      └─ Call ops->sched_wait(ctx, seq, timeout_ms)
         ↓
         ┌────┴────┐
         │         │
         AIE2      VE2
         │         │
         ├─ Wait on partition completion
         ├─ ve2_sched_wait() → poll / completion
         └─ Wait on mailbox response or completion queue
Return 0 or -ETIMEDOUT
```

**Key Functions:**
- `amdxdna_drm_wait_cmd_ioctl()` - DRM IOCTL handler
- `amdxdna_cmd_wait()` - Command wait helper
- `amdxdna_hal_cmd_wait_ctx()` - HAL wrapper (replaces `ops->cmd_wait()`)
- `ops->sched_wait()` - Platform-specific command wait

## HAL Interface Structure

### Platform Operations Structure

```c
struct amdxdna_hal_dev_ops {
    const char *name;  /* "aie2_pci", "aie4_pci", or "ve2_aux" */

    /* Device-level operations */
    int (*dev_init)(struct amdxdna_dev *xdna);
    void (*dev_fini)(struct amdxdna_dev *xdna);

    /* Hardware context operations */
    int (*hwctx_init)(struct amdxdna_ctx *hwctx,
                      struct amdxdna_hal_hwctx_config *config);
    void (*hwctx_fini)(struct amdxdna_ctx *hwctx);

    /* Scheduler operations */
    int (*sched_init)(struct amdxdna_ctx *hwctx);
    void (*sched_fini)(struct amdxdna_ctx *hwctx);
    int (*sched_submit)(struct amdxdna_ctx *hwctx, struct amdxdna_hal_cmd *cmd);
    int (*sched_wait)(struct amdxdna_ctx *hwctx, u64 seq, u32 timeout_ms);

    /* Firmware operations */
    int (*fw_load)(struct amdxdna_dev *xdna);
    void (*fw_unload)(struct amdxdna_dev *xdna);
};
```

### HAL Descriptor

Each platform provides a HAL descriptor:

```c
struct amdxdna_hal {
    const char *name;
    const struct amdxdna_hal_dev_ops *ops;
};

/* Platform instances */
extern const struct amdxdna_hal amdxdna_hal_aie2;
extern const struct amdxdna_hal amdxdna_hal_aie4;
extern const struct amdxdna_hal amdxdna_hal_ve2;
```

## Key Changes from Base Implementation

### 1. HAL Layer Integration

**Before:**
```c
/* Direct call to platform ops */
ret = xdna->dev_info->ops->ctx_init(ctx);
```

**After:**
```c
/* Route through HAL layer */
ret = amdxdna_hal_hwctx_init(ctx);
/* Which internally calls: ops->hwctx_init(ctx, config) */
```

### 2. Platform-Specific Code Isolation

Platform-specific implementations are now isolated in separate files:
- `aie2_pci.c` - AIE2/PCI platform implementation
- `aie4_pci.c` - AIE4/PCI platform implementation
- `ve2_aux.c` - VE2/Auxiliary platform implementation

### 3. Unified Bus Driver Binding

Both PCI and Auxiliary bus drivers use the same HAL interface:
- `amdxdna_pci_drv.c` - PCI bus driver (calls `amdxdna_hal_dev_probe()`)
- `amdxdna_aux_drv.c` - Auxiliary bus driver (calls `amdxdna_hal_dev_probe()`)

### 4. Wrapper Functions

HAL wrapper functions provide seamless integration:
- `amdxdna_hal_hwctx_init()` - Replaces `ops->ctx_init()`
- `amdxdna_hal_hwctx_fini()` - Replaces `ops->ctx_fini()`
- `amdxdna_hal_cmd_submit_job()` - Replaces `ops->cmd_submit()`
- `amdxdna_hal_cmd_wait_ctx()` - Replaces `ops->cmd_wait()`

## File Structure

```
amdxdna/
├── amdxdna_hal_drv.h          # HAL interface definitions
├── amdxdna_hal_drv.c          # HAL implementation
├── amdxdna_pci_drv.c          # PCI bus driver
├── amdxdna_aux_drv.c          # Auxiliary bus driver
├── aie2_pci.c                 # AIE2/PCI platform implementation
├── aie4_pci.c                 # AIE4/PCI platform implementation
├── ve2_aux.c                  # VE2/Auxiliary platform implementation
├── amdxdna_ctx.c              # Context management (uses HAL)
├── amdxdna_drm.c              # DRM core
└── ... (other common framework files)
```

## Benefits

1. **Code Reusability**: Common framework code is shared across all platforms
2. **Platform Isolation**: Platform-specific code is clearly separated
3. **Easy Extension**: Adding new platforms requires only implementing the HAL ops
4. **Maintainability**: Changes to common framework don't affect platform code
5. **Testability**: Platform implementations can be tested independently

## Future Enhancements

1. **Full Implementation**: Complete the prototype implementations with real platform code
2. **Error Handling**: Enhanced error handling and recovery mechanisms
3. **Performance**: Optimize HAL layer overhead
4. **Documentation**: Expand inline documentation for platform-specific operations

## Contact

For questions or contributions, please contact the AMD XDNA driver team.

