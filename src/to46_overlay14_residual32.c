#include "to46_overlay14_residual32_private.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void ov14_021EDE38(void *p) {
    ov14_021F3044(p);
    ov14_021E7ED0(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_SlideInPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    U8_AT(p, 0x22) = 2;
    PCBox_WaitForUpdateFunc(p, ov14_021E9518, 0x54);
}

void ov14_021EDE70(void *p) {
    PCBox_AllocMoveWork(p);
    PCBox_WaitForUpdateFunc(p, PCBox_AnimateMoveToPartyEnd, 0x55);
}

void ov14_021EDE88(void *p) {
    struct {
        u8 y;
        u8 x;
    } pos;

    PlaySE(0x5ea);
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), U8_AT(p, 0x21));
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(PTR_AT(PTR_AT(p, 0x34), 0x2c), U8_AT(p, 0x21)), &pos.x, &pos.y);
    ManagedSprite_SetPositionXY(PTR_AT(PTR_AT(p, 0x34), 0x320), pos.x, pos.y);
    U8_AT(p, 0x21) = 0xff;
    PCBox_ApplyMonMove(p);
    PCBox_FreeMoveWork(p);
    PCBox_ClearMonInfo(p);
    PCBox_SlideOutPartyPanel(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_WaitForUpdateFunc(p, ov14_021E9518, 0x56);
}

void ov14_021EDF08(void *p) {
    ov14_021E8328(PTR_AT(PTR_AT(p, 0x34), 0x2f0));
    PCBox_WaitForUpdateFunc(p, ov14_021E95B4, 0x52);
}

u32 ov14_021EDF28(void *p) {
    u32 input = U32_AT(PTR_AT(PTR_AT(p, 0x34), 0xc), 0xe4);

    PlaySE(0x5ea);
    PCBox_ApplyMonMove(p);
    PCBox_FreeMoveWork(p);
    ov14_021F6678(PTR_AT(p, 0x34), 0x28);
    if (U8_AT(p, 0x21) == 0xff) {
        PCBox_PrintMonActionMessage(p, 0, 0, 0x27);
    } else {
        PCBox_PrintMonActionMessage(p, U8_AT(p, 0x21), 1, 0x27);
        input = 0x22;
    }
    GridInputHandler_SetNextInput(PTR_AT(PTR_AT(p, 0x34), 0x2c), (u8)input);
    GridInputHandler_SetButtonInputMode(PTR_AT(PTR_AT(p, 0x34), 0x2c), 1);
    return 0x51;
}
