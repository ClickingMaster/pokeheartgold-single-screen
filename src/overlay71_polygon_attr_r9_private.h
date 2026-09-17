#ifndef TO51_R4_W05_PRIVATE_H
#define TO51_R4_W05_PRIVATE_H
#include "global.h"

#include "nnsys/g3d/model_inline.h"
typedef struct Ov71CamR9 {
    u8 pad00[0x5C];
    NNSG3dResMdl *model;
    u8 pad60[0x88 - 0x60];
    u32 polygonId;
} Ov71CamR9;
void ov71_02247708(Ov71CamR9 *cam, u32 value);
#endif
