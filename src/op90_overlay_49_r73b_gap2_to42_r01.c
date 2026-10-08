#pragma force_active on
#include "global.h"

typedef struct Ov49Anim {
    u8 unk0[0xC];
} Ov49Anim;

typedef struct Ov49Obj {
    u8 unk0[0x28];
} Ov49Obj;

typedef struct Ov49Unk {
    s16 unk0;
    u8 unk2[2];
    u16 count;
    u8 unk6[2];
    Ov49Obj objs[4];
    Ov49Anim anims[4];
    void *models[4];
} Ov49Unk;

void ov49_02259154(void *a0, VecFx32 *pos);
void ov49_0226540C(Ov49Obj *obj, fx32 x0, fx32 x1, fx32 y0, fx32 y1, fx32 z0, fx32 z1, int a7);
void ov49_022655F4(Ov49Anim *anim, int rot, int a2, int a3);
void sub_020182A8(void *model, fx32 x, fx32 y, fx32 z);
void ov49_02267F94(Ov49Unk *data, void *a1);

// @fn ov49_02267F94
void ov49_02267F94(Ov49Unk *data, void *a1) {
    VecFx32 pos;
    u16 rot;
    int i;
    fx32 x, y, z;

    ov49_02259154(a1, &pos);
    pos.x += FX32_CONST(8);
    pos.y += FX32_CONST(16);
    pos.z -= FX32_CONST(8);
    for (i = 0; i < data->count; i++) {
        switch (i) {
        case 0:
            x = pos.x - FX32_CONST(12);
            y = pos.y + FX32_CONST(16);
            z = pos.z;
            rot = 0;
            break;
        case 1:
            x = pos.x + FX32_CONST(12);
            y = pos.y + FX32_CONST(16);
            z = pos.z;
            rot = 0x7FFF;
            break;
        case 2:
            x = pos.x - FX32_CONST(8);
            y = pos.y + FX32_CONST(16);
            z = pos.z - FX32_CONST(6);
            rot = 0x7FFF;
            break;
        case 3:
            x = pos.x + FX32_CONST(8);
            y = pos.y + FX32_CONST(16);
            z = pos.z - FX32_CONST(6);
            rot = 0;
            break;
        }
        ov49_0226540C(&data->objs[i], pos.x, x, pos.y, y, pos.z, z, 18);
        ov49_022655F4(&data->anims[i], rot, 0xCCC, FX32_CONST(6));
        sub_020182A8(data->models[i], pos.x, pos.y, pos.z);
    }
}
