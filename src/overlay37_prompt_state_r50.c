#include "global.h"

#include "overlay_37_helpers_internal.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))

extern void Comm_SendCommand(u32, const void *, u32);
extern void MI_CpuFill8(void *, u8, u32);
extern void CopyWindowToVram(Ov37Window *);
extern s32 YesNoPrompt_HandleInput(void *);

u32 ov37_021E69DC(Ov37Work *work, u32 state);

u32 ov37_021E69DC(Ov37Work *work, u32 state) {
    u8 packet[4];
    u32 player;

    U8_AT(work, 0x4380) &= ~0x38;
    if (U8_AT(work, 0x438B) == 2 && Comm_GetLocalNetId() != 0) {
        ov37_021E68AC(work);
        if (((u32 (*)(Ov37Work *))ov37_021E78C4)(work) != 0) {
            PlaySE(0x5F2);
        }
        return state;
    }
    if (Comm_GetLocalNetId() == 0 && U32_AT(work, 0x93B4) != 0) {
        ov37_021E68AC(work);
        return state;
    }
    player = ov37_021E75C4();
    if (player != Comm_CountConnected()) {
        ov37_021E68AC(work);
        return state;
    }
    switch (YesNoPrompt_HandleInput(PTR_AT(work, 0x93F0))) {
    case 1:
        if (Comm_GetLocalNetId() == 0) {
            ov37_021E7844(work, 0xD);
            ov37_021E762C(work, 4, 1);
        } else {
            MI_CpuFill8(packet, 0, 4);
            packet[2] = 0;
            packet[0] = (u8)Comm_GetLocalNetId();
            U8_AT(work, 0x93BC) = 1;
            U16_AT(work, 0x93BA) = 0;
            ov37_021E7844(work, 7);
            Comm_SendCommand(0x7E, packet, 4);
            ClearFrameAndWindow2((void *)((u8 *)work + 0x2D8), 1);
        }
        ov37_021E78A4(work);
        CopyWindowToVram((Ov37Window *)((u8 *)work + 0x2C8));
        break;
    case 2:
        ov37_021E7844(work, 4);
        ov37_021E6848((void *)((u8 *)work + 0x248), 0);
        ClearFrameAndWindow2((void *)((u8 *)work + 0x2D8), 1);
        ov37_021E78A4(work);
        CopyWindowToVram((Ov37Window *)((u8 *)work + 0x2C8));
        if (Comm_GetLocalNetId() == 0) {
            sub_02038C1C(Comm_CountConnected() + 1);
            U32_AT(work, 0x93F4) = 1;
        }
        break;
    default:
        break;
    }
    ov37_021E68AC(work);
    return state;
}
