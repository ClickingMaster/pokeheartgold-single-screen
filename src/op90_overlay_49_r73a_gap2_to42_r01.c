#pragma force_active on
#include "global.h"
#include <nitro/fx/fx_trig.h>

typedef struct Curve {
    u8 data[40];
} Curve;

typedef struct Obj120 {
    u8 data[120];
} Obj120;

typedef struct Motion {
    u32 unk0;
    u32 unk4;
    void *main;
    Obj120 objs[19];
    u8 unk8F4[0x60];
    s8 stage;
    s8 count;
    s16 timer;
    int base;
    u8 unk95C[8];
    u8 tick;
    u8 unk965[3];
    VecFx32 positions[3];
    Curve curves[4];
} Motion;

typedef struct RootMotion {
    u32 unk0;
    void *context;
    void *ui;
} RootMotion;

void ov49_0225919C(void *main, int a1);
void ov49_02259160(void *main, void *a1);
void ov49_02259154(void *main, VecFx32 *pos);
void ov49_02259148(void *main, VecFx32 *pos);
void ov49_02259130(void *main, int a1);
void *ov49_02258E60(void *main, int a1);
int ov49_02265434(Curve *curve, int frame);
void ov49_022655E0(Curve *curve, fx32 *x, fx32 *y, fx32 *z);
void ov49_02265668(RootMotion *root, Motion *s, int se);
void *ov49_02258DAC(void *context);
void ov49_0225CC40(void *ui, void *main);
void ov49_02265628(VecFx32 *pos);
void ModelInstance_GetPosition(Obj120 *obj, fx32 *x, fx32 *y, fx32 *z);
void ModelInstance_SetPosition(Obj120 *obj, fx32 x, fx32 y, fx32 z);
void ov49_02265660(VecFx32 *pos, fx32 *offset);
int ov49_0226786C(RootMotion *root, Motion *s, int index, int a3);
void ov49_0226789C(RootMotion *root, Motion *s, int index);
int ov49_022670D4(RootMotion *root, Motion *s);

// @fn ov49_022670D4
int ov49_022670D4(RootMotion *root, Motion *s) {
    VecFx32 mainPos;
    VecFx32 movement;
    VecFx32 peripheral;
    fx32 offset;
    int i;
    int done;
    Curve *curve;
    VecFx32 *position;
    Obj120 *object;

    switch (s->stage) {
    case 0:
        s->timer++;
        if (s->timer >= 8) {
            s->stage = 1;
        }
        break;
    case 1:
        ov49_0225919C(s->main, 1);
        s->timer = 16;
        s->stage = 2;
        break;
    case 2:
        s->timer--;
        if (s->timer <= 0) {
            ov49_0225919C(s->main, 0);
            s->stage = 3;
            s->timer = 0;
            ov49_02259160(s->main, ov49_02258E60(s->main, 6));
        }
        break;
    case 3:
        done = ov49_02265434(&s->curves[3], s->timer);
        s->timer++;
        ov49_02259154(s->main, &mainPos);
        ov49_022655E0(&s->curves[3], &mainPos.x, &mainPos.y, &mainPos.z);
        ov49_02259148(s->main, &mainPos);
        if (done == 1) {
            s->stage = 4;
            s->timer = 0;
            s->base = mainPos.y;
            ov49_02265668(root, s, 0x5C2);
        }
        break;
    case 4: {
        u16 angle = 0x7FFF * s->timer / 10;
        fx32 value = FX_Mul(FX_SinIdx(angle), 0x8000);
        int next;
        ov49_02259154(s->main, &movement);
        movement.y = s->base + value;
        ov49_02259148(s->main, &movement);
        next = s->timer + 1;
        if (next > 10) {
            void *main;
            ov49_02259130(s->main, 1);
            main = s->main;
            if (main == ov49_02258DAC(root->context)) {
                ov49_0225CC40(root->ui, main);
            }
            return 1;
        }
        s->timer = next;
        break;
    }
    }
    s->tick++;
    i = 0;
    if (i < s->count) {
        curve = s->curves;
        position = s->positions;
        object = s->objs;
        do {
            if (ov49_02265434(curve, s->tick) == 0) {
                ov49_02265628(position);
                ModelInstance_GetPosition(object, &peripheral.x, &peripheral.y, &peripheral.z);
                ov49_022655E0(&s->curves[i], &peripheral.x, &peripheral.y, &peripheral.z);
                ov49_02265660(&s->positions[i], &offset);
                peripheral.x += offset;
                ModelInstance_SetPosition(&s->objs[i], peripheral.x, peripheral.y, peripheral.z);
            } else if (ov49_0226786C(root, s, i, 4) == 1) {
                ov49_02265668(root, s, 0x5A8);
            }
            ov49_0226789C(root, s, i);
            curve++;
            position++;
            object++;
        } while (++i < s->count);
    }
    return 0;
}
