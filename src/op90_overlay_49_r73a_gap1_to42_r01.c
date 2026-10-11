#pragma force_active on
#include "global.h"

typedef struct Ov49Obj {
    u8 unk0[0x78];
} Ov49Obj;

typedef struct Ov49Anim {
    u8 unk0[0xC];
} Ov49Anim;

typedef struct Ov49Obj2 {
    u8 unk0[0x28];
} Ov49Obj2;

typedef struct Ov49Unk {
    u8 pad0[0x8];
    void *unk8;
    Ov49Obj objs[19];
    u8 pad8F4[0x60];
    u8 unk954;
    s8 count;
    u16 unk956;
    u8 pad958[0x8];
    u8 dirs[4];
    u8 unk964;
    u8 pad965[0x3];
    Ov49Anim anims[3];
    Ov49Obj2 objs2[4];
} Ov49Unk;

extern const u8 ov49_0226A450[];

void ov49_02259154(void *a0, VecFx32 *pos);
void ov49_0226540C(Ov49Obj2 *obj, fx32 x0, fx32 x1, fx32 y0, fx32 y1, fx32 z0, fx32 z1, int a7);
void ModelInstance_GetPosition(Ov49Obj *obj, fx32 *x, fx32 *y, fx32 *z);
void ov49_022655F4(Ov49Anim *anim, int rot, int a2, int a3);
void ov49_0226786C(void *a0, Ov49Unk *data, int index, int a3);
void ov49_02266F14(void *a0, Ov49Unk *data);

// @fn ov49_02266F14
void ov49_02266F14(void *a0, Ov49Unk *data) {
    VecFx32 pos;
    fx32 x, y, z;
    int i;

    ov49_02259154(data->unk8, &pos);
    ov49_0226540C(&data->objs2[3], pos.x, pos.x, pos.y, pos.y - FX32_CONST(ov49_0226A450[data->count - 1]), pos.z, pos.z, 10);
    data->unk956 = 0;
    data->unk954 = 0;
    for (i = 0; i < data->count; i++) {
        ModelInstance_GetPosition(&data->objs[i], &pos.x, &pos.y, &pos.z);
        switch (data->dirs[i]) {
        case 0:
            x = pos.x;
            y = pos.y + FX32_CONST(64);
            z = pos.z - FX32_CONST(64);
            break;
        case 1:
            x = pos.x + FX32_CONST(64);
            y = pos.y + FX32_CONST(64);
            z = pos.z;
            break;
        case 2:
            x = pos.x - FX32_CONST(64);
            y = pos.y + FX32_CONST(64);
            z = pos.z;
            break;
        }
        ov49_0226540C(&data->objs2[i], pos.x, x, pos.y, y, pos.z, z, 26);
        ov49_022655F4(&data->anims[i], 0, 0xAAA, FX32_CONST(6));
        ov49_0226786C(a0, data, i, 3);
    }
    data->unk964 = 0;
}
