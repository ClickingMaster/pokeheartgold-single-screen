#pragma force_active on
#include "global.h"

typedef struct Ov49FramePair {
    u16 objIndex;
    u16 value;
} Ov49FramePair;

typedef struct Ov49Obj {
    u8 unk0[0x78];
} Ov49Obj;

typedef struct Ov49Unk {
    u8 unk0[2];
    s16 counter;
    u8 unk4[8];
    Ov49Obj objs[19];
    u8 unk8F4[0x60];
    int lastFrame;
} Ov49Unk;

extern const Ov49FramePair ov49_0226A678[][12];
extern const Ov49FramePair ov49_0226A67C[][12];

void ModelInstance_SetVisible(Ov49Obj *obj, int a1);
void ov49_02265BE8(void *a0, Ov49Unk *data, u32 index, int axis, fx32 value);
BOOL ov49_02267AF0(void *a0, Ov49Unk *data, int type);

// @fn ov49_02267AF0
BOOL ov49_02267AF0(void *a0, Ov49Unk *data, int type) {
    Ov49FramePair prev;
    Ov49FramePair cur;
    int frame = data->counter * 12 / 48;

    if (frame != data->lastFrame) {
        data->lastFrame = frame;
        if (frame != 0) {
            prev = ov49_0226A678[type][frame];
            ModelInstance_SetVisible(&data->objs[prev.objIndex], 0);
        }
        cur = ov49_0226A67C[type][frame];
        if (cur.value != 0) {
            ov49_02265BE8(a0, data, cur.objIndex, 0, FX32_CONST(cur.value));
            ov49_02265BE8(a0, data, cur.objIndex, 1, FX32_CONST(cur.value));
            ModelInstance_SetVisible(&data->objs[cur.objIndex], 1);
        } else {
            ModelInstance_SetVisible(&data->objs[cur.objIndex], 0);
        }
    }
    if (data->counter < 48) {
        data->counter++;
    }
    if (data->counter >= 48) {
        return TRUE;
    }
    return FALSE;
}
