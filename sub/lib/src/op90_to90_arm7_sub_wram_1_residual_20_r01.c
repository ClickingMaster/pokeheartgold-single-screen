#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma dollar_identifiers on
extern u32 skipCheck$3668[], nextCount$3668[];
extern BOOL CARD_IsPulledOut(void);
extern u8 *CARD_GetRomHeader(void);
extern void VENEER_SVC_WaitByLoop(s32);
extern int PXI_SendWordByFifo(int, u32, BOOL);
void CARD_CheckPullOut_Polling(void);
// @fn CARD_CheckPullOut_Polling
void CARD_CheckPullOut_Polling(void)
{
    u32 next;
    if (skipCheck$3668[1])
        return;
    if (*(vu16 *)0x027ffc40 == 2)
        return;
    next = nextCount$3668[0];
    if (next == 0xffffffff) {
        nextCount$3668[0] = *(vu32 *)0x027ffc3c + 10;
        return;
    }
    if (*(vu32 *)0x027ffc3c < next)
        return;
    nextCount$3668[0] = *(vu32 *)0x027ffc3c + 10;
    if (CARD_IsPulledOut()) {
        skipCheck$3668[1] = TRUE;
        if (*(u32 *)(CARD_GetRomHeader() + 0xc) == 0 && nextCount$3668[1] != 0)
            return;
    }
    nextCount$3668[1] = 0;
    if (skipCheck$3668[1]) {
        while (PXI_SendWordByFifo(14, 17, FALSE))
            VENEER_SVC_WaitByLoop(100);
    }
}
