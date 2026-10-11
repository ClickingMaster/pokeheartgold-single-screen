#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define TASK_NONE 0xFFFF

// IRQs masked while the task queues change (wireless and timer 0).
#define TASK_IRQ_MASK 0x1000010

extern void (*const pTaskFunc[24])(void);

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Shuts the hardware down and restarts the driver with its current station table.
void WlessLibReboot(void) {
    WlWork *w;
    ClearTimeOut();
    WShutdown();
    InitMac();
    ReleaseAllWlHeapBuf();
    InitializeTask();
    w = wlWork;
    InitializeParam(w->param.camTable, w->param.camTableSize);
    InitializeCmdIf();
    InitializeMLME();
    InitializeCAM();
    WSetDefaultParameters();
}

// Sets up the driver in param->work, checks the hardware and starts the driver thread; returns the
// error flags (WlParam.errors).
u16 WL_InitDriver(WlInitParam *param) {
    u32 checksum;
    WlWork *initial = param->work;
    wlWork = initial;
    __VENEER_MIi_CpuClearFast(0, initial, sizeof(WlWork));
    wlWork->lockId = __VENEER_OS_GetLockID();
    wlWork->dmaNo = param->dmaNo;
    wlWork->unk_310 = param->unk_1c >> 1;
    {
        WlWork *w = wlWork;
        if (w->unk_310 == 0) {
            w->unk_310 = 0xFFFFFFFF;
        }
    }
    InitializeHeapBuf(&param->heap);
    FLASH_MakeImage();
    *(vu16 *)0x04000304 |= 2; // POWCNT2: wireless on
    *(vu16 *)0x04000206 = 48; // WIFIWAITCNT
    wlWork->unk_68c = 3;
    FLASH_Read(0x3C, 2, &wlWork->unk_692);
    FLASH_Read(0x3E, 2, &wlWork->hwWorkarounds);
    wlWork->requestQueue = param->requestQueue;
    wlWork->indicateQueue = param->indicateQueue;
    {
        u32 stations = __VENEER__u32_div_f(param->camTableSize, 28);
        InitializeParam(param->camTable, stations);
    }
    InitializeTask();
    InitializeCmdIf();
    InitializeMLME();
    InitializeCAM();
    InitializeAlarm();
    if (FLASH_VerifyCheckSum(&checksum)) {
        WlWork *w = wlWork;
        w->param.errors |= 0x80;
    } else {
        WConfigDevice();
        DiagMacRegister();
        WWakeUp();
        InitMac();
        InitRF();
        DiagMacMemory();
        DiagBaseBand();
        InitBaseBand();
        WSetDefaultParameters();
        WShutdown();
    }
    __VENEER_OS_CreateThread(&wlWork->thread, MainTaskRoutine, NULL, param->stack, param->stackSize, param->priority);
    __VENEER_OS_WakeupThreadDirect(&wlWork->thread);
    InitializeIntr();
    return wlWork->param.errors;
}

// clang-format off
asm void __VENEER_MIi_CpuClearFast(u32 data, void *dest, u32 size) {
    ldr pc, =MIi_CpuClearFast
}
// clang-format on

// clang-format off
asm u32 __VENEER_OS_GetLockID(void) {
    ldr pc, =OS_GetLockID
}
// clang-format on

void *WL_GetThreadStruct(void) {
    return &wlWork->thread;
}

// Queues the terminate task and waits for the driver thread to end.
void WL_Terminate(void) {
    WlWork *w = wlWork;
    if (__VENEER_OS_IsThreadTerminated(&w->thread) != TRUE) {
        AddTask(2, 22);
        __VENEER_OS_JoinThread(&w->thread);
        while (!__VENEER_OS_IsThreadTerminated(&w->thread)) {}
    }
}

// clang-format off
asm BOOL __VENEER_OS_IsThreadTerminated(const OSThread *thread) {
    ldr pc, =OS_IsThreadTerminated
}
// clang-format on

// clang-format off
asm void __VENEER_OS_JoinThread(OSThread *thread) {
    ldr pc, =OS_JoinThread
}
// clang-format on

void InitializeTask(void) {
    WlWork *w = *(WlWork **)WL_WORK_ADDR;
    u32 i;
    w->taskPriority = 0;
    w->taskSelected = 0;
    for (i = 0; i < 4; i++) {
        w->taskTail[i] = TASK_NONE;
        w->taskHead[i] = w->taskTail[i];
    }
    for (i = 0; i < 24; i++) {
        w->task[i].next = TASK_NONE;
        w->task[i].queued = 0;
        w->task[i].func = pTaskFunc[i];
    }
    AddTask(3, 12);
}

// Driver thread: hands received requests to ExecuteMessage and runs the queued tasks, highest
// priority first.
void MainTaskRoutine(void *arg) {
    WlWork *w = wlWork;
    OSMessage message;
    u32 mask = TASK_IRQ_MASK;
    w->taskPriority = 0;
    u32 none = TASK_NONE;
    w->taskCurrent = 0;
    while (1) {
        if (__VENEER_OS_ReceiveMessage(wlWork->requestQueue, &message, OS_MESSAGE_NOBLOCK)) {
            ExecuteMessage((WlHeapBuf **)&message);
        }
        {
            u32 irq = __VENEER_OS_DisableIrqMask(mask);
            u16 priority = VOLATILE_U16(w->taskPriority);
            VOLATILE_U16(w->taskSelected) = priority;
            if (w->taskHead[priority] == none) {
                VOLATILE_U16(w->taskPriority) += 1;
                __VENEER_OS_EnableIrqMask(irq);
                continue;
            }
            __VENEER_OS_EnableIrqMask(irq);
        }
        VOLATILE_U16(w->taskCurrent) = DeleteTask(VOLATILE_U16(w->taskSelected));
        w->task[VOLATILE_U16(w->taskCurrent)].func();
        VOLATILE_U16(w->taskCurrent) = none;
    }
}

// clang-format off
asm u32 __VENEER_OS_DisableIrqMask(u32 mask) {
    ldr pc, =OS_DisableIrqMask
}
// clang-format on

// clang-format off
asm void __VENEER_OS_EnableIrqMask(u32 mask) {
    ldr pc, =OS_EnableIrqMask
}
// clang-format on

// Appends task `index` to the queue of `priority` unless it is queued already; wakes the driver thread
// when it idles in the lowest priority.
void AddTask(int priority, u32 index) {
    WlWork *w = wlWork;
    WlTask *task = w->task;
    u32 irq = __VENEER_OS_DisableIrqMask(TASK_IRQ_MASK);
    if (task[index].queued == 0) {
        task[index].queued = 1;
        task[index].next = TASK_NONE;
        {
            u16 tail = w->taskTail[priority];
            if (tail == TASK_NONE) {
                w->taskHead[priority] = index;
            } else {
                w->task[tail].next = index;
            }
        }
        w->taskTail[priority] = index;
        if (priority < VOLATILE_U16(w->taskPriority)) {
            VOLATILE_U16(w->taskPriority) = priority;
        }
    }
    __VENEER_OS_EnableIrqMask(irq);
    if (priority != 3 && VOLATILE_U16(w->taskSelected) == 3) {
        __VENEER_OS_SendMessage(wlWork->requestQueue, NULL, OS_MESSAGE_NOBLOCK);
    }
}

// Takes the first task off the queue of `priority` (TASK_NONE: empty).
u16 DeleteTask(u32 priority) {
    WlWork *w = wlWork;
    u32 irq = __VENEER_OS_DisableIrqMask(TASK_IRQ_MASK);
    u16 index = w->taskHead[priority];
    if (index != TASK_NONE) {
        WlTask *task = w->task;
        task[index].queued = 0;
        {
            u16 next = task[index].next;
            if (next == TASK_NONE) {
                w->taskHead[priority] = TASK_NONE;
                w->taskTail[priority] = TASK_NONE;
            } else {
                w->taskHead[priority] = next;
                task[index].next = TASK_NONE;
            }
        }
    }
    __VENEER_OS_EnableIrqMask(irq);
    return index;
}

// Task 12 (lowest priority): waits for the next request.
void LowestIdleTask(void) {
    OSMessage message;
    __VENEER_OS_ReceiveMessage(wlWork->requestQueue, &message, OS_MESSAGE_BLOCK);
    ExecuteMessage((WlHeapBuf **)&message);
    AddTask(3, 12);
}

// A request is a heap buffer: hand it to the request manager and queue task 11 to run it.
void ExecuteMessage(WlHeapBuf **message) {
    if (*message) {
        NewHeapBuf(&wlWork->heap.bufMan[10], *message);
        AddTask(2, 11);
    }
}
