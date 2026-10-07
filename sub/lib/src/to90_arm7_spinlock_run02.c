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


#ifdef SDK_ARM7
static inline
#endif
s32 OSi_DoLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    s32 lastLockFlag;

    while ((lastLockFlag = OSi_DoTryLockByWord(lockID, lockp, ctrlFuncp, disableFiq)) > 0) {
        OSi_WaitByLoop();
    }
    return lastLockFlag;
}
s32 OS_LockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void)) {
    return OSi_DoLockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

#pragma force_active on
s32 OSi_DoUnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    if (lockID != lockp->ownerID) {
        return -2;
    }
    OSIntrMode ime = disableFiq ? OS_DisableInterrupts_IrqAndFiq() : OS_DisableInterrupts();
    lockp->ownerID = 0;
    if (ctrlFuncp != NULL) {
        ctrlFuncp();
    }
    lockp->lockFlag = 0;
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(ime);
    } else {
        OS_RestoreInterrupts(ime);
    }
    return 0;
}
#pragma force_active off

s32 OS_UnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void)) {
    return OSi_DoUnlockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

#pragma force_active on
s32 OSi_DoTryLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    s32 lastLockFlag;
    OSIntrMode ime;

    ime = disableFiq ? OS_DisableInterrupts_IrqAndFiq() : OS_DisableInterrupts();
    lastLockFlag = MI_SwapWord(lockID, &lockp->lockFlag);
    if (lastLockFlag == 0) {
        if (ctrlFuncp != NULL) {
            ctrlFuncp();
        }
        lockp->ownerID = lockID;
    }
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(ime);
    } else {
        OS_RestoreInterrupts(ime);
    }
    return lastLockFlag;
}
#pragma force_active off

#pragma force_active on
s32 OS_LockCartridge(u16 lockID) {
    return OSi_DoLockByWord(lockID, (OSLockWord*)HW_CTRDG_LOCK_BUF, OSi_AllocateCartridgeBus, TRUE);
}
#pragma force_active off

#pragma force_active on
s32 OS_UnlockCartridge(u16 lockID) {
    return OSi_DoUnlockByWord(lockID, (OSLockWord*)HW_CTRDG_LOCK_BUF, OSi_FreeCartridgeBus, TRUE);
}
#pragma force_active off
