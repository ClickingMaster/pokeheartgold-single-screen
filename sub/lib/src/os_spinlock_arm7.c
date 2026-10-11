#include <nitro.h>

#define OSi_LOCKID_INITIAL_FLAG_0 0xffffffff
#define OSi_LOCKID_INITIAL_FLAG_1 0xffff0000

#define OSi_ANYP_LOCK_ID_FLAG  HW_LOCK_ID_FLAG_SUB
#define OSi_ANYP_LOCK_ID_START OS_SUBP_LOCK_ID_START

void OSi_AllocateCartridgeBus(void);
void OSi_FreeCartridgeBus(void);
void OSi_AllocateCardBus(void);
void OSi_FreeCardBus(void);

// The ARM7 calls SVC_WaitByLoop (Thumb) through an ARM veneer of its own.
void VENEER_SVC_WaitByLoop(s32 count);

static inline void OSi_WaitByLoop(void) {
    VENEER_SVC_WaitByLoop(0x1000 / 4);
}

#pragma force_active on

void OS_InitLock(void) {
    static BOOL isInitialized = FALSE;
    OSLockWord *lockp;

    if (isInitialized) {
        return;
    }

    isInitialized = TRUE;
    lockp = (OSLockWord *)HW_INIT_LOCK_BUF;

    lockp->extension = 0;
    while (lockp->ownerID != OS_MAINP_SYSTEM_LOCK_ID) {
        OSi_WaitByLoop();
    }
    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[0] = OSi_LOCKID_INITIAL_FLAG_0;
    ((u32 *)OSi_ANYP_LOCK_ID_FLAG)[1] = OSi_LOCKID_INITIAL_FLAG_1;
    lockp->extension = OS_SUBP_SYSTEM_LOCK_ID;
}

#pragma dont_inline on

void VENEER_SVC_WaitByLoop(s32 count) {
    SVC_WaitByLoop(count);
}

#pragma dont_inline reset

static inline s32 OSi_DoLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    s32 lastLockFlag;

    while ((lastLockFlag = OSi_DoTryLockByWord(lockID, lockp, ctrlFuncp, disableFiq)) > 0) {
        OSi_WaitByLoop();
    }
    return lastLockFlag;
}

#pragma force_active off

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

s32 OS_LockCartridge(u16 lockID) {
    return OSi_DoLockByWord(lockID, (OSLockWord *)HW_CTRDG_LOCK_BUF, OSi_AllocateCartridgeBus, TRUE);
}

s32 OS_UnlockCartridge(u16 lockID) {
    return OSi_DoUnlockByWord(lockID, (OSLockWord *)HW_CTRDG_LOCK_BUF, OSi_FreeCartridgeBus, TRUE);
}

// Alias of OS_UnlockCartridge.
// clang-format off
asm s32 OS_UnLockCartridge(u16 lockID) {
    ldr r1, =OS_UnlockCartridge
    bx r1
}
// clang-format on

// -ipa file would otherwise inline the lock functions above into the callers below.
#pragma dont_inline on

s32 OS_TryLockCartridge(u16 lockID) {
    return OSi_DoTryLockByWord(lockID, (OSLockWord *)HW_CTRDG_LOCK_BUF, OSi_AllocateCartridgeBus, TRUE);
}

void OSi_AllocateCartridgeBus(void) {
}

void OSi_FreeCartridgeBus(void) {
}

#pragma force_active off

s32 OS_LockCard(u16 lockID) {
    return OS_LockByWord(lockID, (OSLockWord *)HW_CARD_LOCK_BUF, OSi_AllocateCardBus);
}

#pragma force_active on

s32 OS_UnlockCard(u16 lockID) {
    return OSi_DoUnlockByWord(lockID, (OSLockWord *)HW_CARD_LOCK_BUF, OSi_FreeCardBus, FALSE);
}

#pragma dont_inline reset
