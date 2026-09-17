#ifndef TO46_OVERLAY71_RESIDUAL7_PRIVATE_H
#define TO46_OVERLAY71_RESIDUAL7_PRIVATE_H

#include "global.h"

typedef struct Ov71Model {
    u32 unk00;
    u8 renderObj[0x60];
    u32 active;
    VecFx32 translation;
    VecFx32 scale;
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    u16 padding86;
    u32 polygonId;
} Ov71Model;

typedef struct Ov71ModelManager {
    u8 unk00[0x18];
    Ov71Model *models;
    u32 count;
} Ov71ModelManager;

Ov71ModelManager *ov71_0224744C(u32 count, void *a1, void *a2, void *a3);
void ov71_02247498(Ov71ModelManager *manager);
void ov71_022474CC(Ov71ModelManager *manager);
void ov71_02247514(Ov71Model *model);
void ov71_022475C4(Ov71Model *model);

void ov71_0224784C(Ov71ModelManager *, void *, void *, void *);
void ov71_022475F8(Ov71Model *);
void ov71_022478B8(Ov71ModelManager *);

#endif
