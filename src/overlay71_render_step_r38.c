#include "global.h"
typedef struct Ov71R38 {
    u8 pad00[0x20];
    void *unk20;
    void *unk24;
    u8 pad28[2];
    s16 angle;
} Ov71R38;
void ov71_022476EC(void *, void *);
void ov71_022474CC(void *);
void G3X_Reset(void);
void RequestSwap3DBuffers(int, int);
void ov71_0224A9C4(Ov71R38 *work);
void ov71_0224A9C4(Ov71R38 *work) {
    if (work->unk20 != NULL) {
        work->angle += FX32_ONE;
        ov71_022476EC(work->unk24, &work->pad28);
        G3X_Reset();
        ov71_022474CC(work->unk20);
        RequestSwap3DBuffers(0, 0);
    }
}
