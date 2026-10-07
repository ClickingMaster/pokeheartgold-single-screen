#include <nitro.h>

#define OSi_LOCKID_INITIAL_FLAG_0     0xffffffff
#define OSi_LOCKID_INITIAL_FLAG_1     0xffff0000

#ifdef SDK_ARM9
#define OSi_ANYP_LOCK_ID_FLAG  HW_LOCK_ID_FLAG_MAIN
#define OSi_ANYP_LOCK_ID_START OS_MAINP_LOCK_ID_START
#else
#define OSi_ANYP_LOCK_ID_FLAG  HW_LOCK_ID_FLAG_SUB
#define OSi_ANYP_LOCK_ID_START OS_SUBP_LOCK_ID_START
#endif

void OSi_AllocateCartridgeBus(void);
void OSi_FreeCartridgeBus(void);
void OSi_AllocateCardBus(void);
void OSi_FreeCardBus(void);

// FIXME: This looks like it's meant to be a linker-inserted veneer, but I can't get it to insert properly yet
#ifdef SDK_ARM9
static inline void OSi_WaitByLoop(void) {
    SVC_WaitByLoop(0x1000 / 4);
}
#else
void VENEER_SVC_WaitByLoop(s32 count);
static inline void OSi_WaitByLoop(void) {
    VENEER_SVC_WaitByLoop(0x1000 / 4);
}
#endif


#pragma force_active on
void OS_InitLock(void) {
    static BOOL isInitialized = FALSE;
    OSLockWord *lockp;

    if (isInitialized)
        return;

    isInitialized = TRUE;
    lockp = (OSLockWord *)HW_INIT_LOCK_BUF;

#ifdef SDK_ARM9
    lockp->lockFlag = 0;
    OS_LockByWord(OS_MAINP_SYSTEM_LOCK_ID - 1, lockp, NULL);
    while (lockp->extension != 0) {
        OSi_WaitByLoop();
    }
    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[0] = OSi_LOCKID_INITIAL_FLAG_0;
    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[1] = OSi_LOCKID_INITIAL_FLAG_1;
    MI_CpuClear32((void *)HW_SHARED_LOCK_BUF, HW_CTRDG_LOCK_BUF - HW_SHARED_LOCK_BUF);
    MIi_SetCardProcessor(MI_PROCESSOR_ARM7);
    MIi_SetCartridgeProcessor(MI_PROCESSOR_ARM7);
    OS_UnlockByWord(OS_MAINP_SYSTEM_LOCK_ID - 1, lockp, NULL);
    OS_LockByWord(OS_MAINP_SYSTEM_LOCK_ID, lockp, NULL);
#else
    lockp->extension = 0;
    while (lockp->ownerID != OS_MAINP_SYSTEM_LOCK_ID) {
        OSi_WaitByLoop();
    }
    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[0] = OSi_LOCKID_INITIAL_FLAG_0;
    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[1] = OSi_LOCKID_INITIAL_FLAG_1;
    lockp->extension = OS_SUBP_SYSTEM_LOCK_ID;
#endif
}
#pragma force_active off

#pragma dont_inline on
#pragma force_active on
void VENEER_SVC_WaitByLoop(s32 count){SVC_WaitByLoop(count);}
#pragma force_active off
#pragma dont_inline off
