#include "to42_overlay15_r16_private.h"

#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))

s32 BagApp_State_SetupSellQuantity(void *work) {
    if (!TextPrinterCheckActive(U8(work, 0x616))) {
        if (U16(work, 0x682) > 99) {
            U16(work, 0x682) = 99;
        }
        BagApp_LoadSubScreenLayout(work, 4, 0, 0);
        BagApp_ShowQuantityButtons(work, 2, U16(work, 0x682));
        BagApp_PrintSellLabel(work);
        BagApp_PrintCancelButton(work, 1);
        BagApp_SetCancelSpriteVisible(work, 1);
        BagApp_SetQuantityCursorPosition(work, 0);
        return 18;
    }
    return 17;
}
