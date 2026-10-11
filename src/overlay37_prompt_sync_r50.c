#include "global.h"

#include "overlay_37_helpers_internal.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern s32 YesNoPrompt_HandleInput(void *);
extern void sub_02037108(u32, u32, u32);
extern void BufferPlayersName(void *, u32, u32);

u32 ov37_021E6D14(Ov37Work *work, u32 state);

u32 ov37_021E6D14(Ov37Work *work, u32 state) {
    s32 input;

    input = YesNoPrompt_HandleInput(PTR_AT(work, 0x93F0));
    if (U32_AT(work, 0x318) != ov37_021E75C4() || U32_AT(work, 0x93B4) != 0) {
        ov37_021E68AC(work);
        return state;
    }
    switch (input) {
    case 1:
        ov37_021E7844(work, 0xF);
        sub_02037108(0x7F, 0, 0);
        BufferPlayersName(PTR_AT(work, 0xC), 0, sub_02034818(0));
        state = 2;
        ov37_021E78A4(work);
        break;
    case 2:
        ov37_021E7844(work, 4);
        ov37_021E6848((void *)((u8 *)work + 0x248), 0);
        ClearFrameAndWindow2((void *)((u8 *)work + 0x2D8), 1);
        ov37_021E78A4(work);
        if (Comm_GetLocalNetId() == 0) {
            sub_02038C1C(Comm_CountConnected() + 1);
            U32_AT(work, 0x93F4) = 1;
        }
        break;
    }
    ov37_021E68AC(work);
    return state;
}
