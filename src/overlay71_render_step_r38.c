#include "global.h"
typedef struct Ov71R38 {
    u8 pad00[0x20];
    void *unk20;
    void *unk24;
    u8 pad28[2];
    s16 angle;
} Ov71R38;
void TradeSequence_SetModelRotation(void *, void *);
void TradeSequence_DrawModels(void *);
void G3X_Reset(void);
void RequestSwap3DBuffers(int, int);
void ov71_0224A9C4(Ov71R38 *work);
void ov71_0224A9C4(Ov71R38 *work) {
    if (work->unk20 != NULL) {
        work->angle += FX32_ONE;
        TradeSequence_SetModelRotation(work->unk24, &work->pad28);
        G3X_Reset();
        TradeSequence_DrawModels(work->unk20);
        RequestSwap3DBuffers(0, 0);
    }
}
