#include <nitro.h>

void OSi_AllocateCardBus(void);
void OSi_FreeCardBus(void);

#pragma force_active on

BOOL OS_TryLockCard(u16 lockID) {
    return OSi_DoTryLockByWord(lockID, (OSLockWord *)HW_CARD_LOCK_BUF, OSi_AllocateCardBus, FALSE);
}

void OSi_AllocateCardBus(void) {
}

void OSi_FreeCardBus(void) {
}

u16 OS_ReadOwnerOfLockWord(OSLockWord *lockp) {
    return lockp->ownerID;
}

// Allocates the first free ID of the ARM7 lock-ID bitmap (0x027FFFB8: IDs 0x80-0x9F, then 0xA0-0xBF).
// clang-format off
asm s32 OS_GetLockID(void) {
    ldr r3, =0x027fffb8
    ldr r1, [r3]
    mov r2, #0
    mov r0, #0x80000000
@scan1:
    tst r1, r0
    bne @found1
    add r2, r2, #1
    cmp r2, #32
    beq @found1
    mov r0, r0, lsr #1
    b @scan1
@found1:
    cmp r2, #32
    movne r0, #128
    bne @alloc
    add r3, r3, #4
    ldr r1, [r3]
    mov r2, #0
    mov r0, #0x80000000
@scan2:
    tst r1, r0
    bne @found2
    add r2, r2, #1
    cmp r2, #32
    beq @found2
    mov r0, r0, lsr #1
    b @scan2
@found2:
    cmp r2, #32
    ldr r0, =0xfffffffd
    bxeq lr
    mov r0, #160
@alloc:
    add r0, r0, r2
    mov r1, #0x80000000
    mov r1, r1, lsr r2
    ldr r2, [r3]
    bic r2, r2, r1
    str r2, [r3]
    bx lr
}
// clang-format on

// clang-format off
asm void OS_ReleaseLockID(u16 lockID) {
    ldr r3, =0x027fffb8
    cmp r0, #160
    addpl r3, r3, #4
    subpl r0, r0, #160
    submi r0, r0, #128
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3]
    orr r2, r2, r1
    str r2, [r3]
    bx lr
}
// clang-format on
