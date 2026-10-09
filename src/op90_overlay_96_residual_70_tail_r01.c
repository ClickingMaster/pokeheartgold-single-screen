#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {
    int a[8][5];
} Params;
typedef struct {
    u16 x0;
    u16 x2;
    u16 x4;
    u16 x6;
    u8 x8;
    u8 pad9[3];
    f32 xc;
    u8 pad10[4];
    u16 x14;
    u16 x16;
    u8 slot;
    u8 pad19[7];
    void *x20;
    void *x24;
    u8 pad28[0x14];
    int x3c;
    int x40;
    void *x44;
    void *x48;
    void *x4c;
    f32 x50;
    f32 x54;
    u8 x58;
    u8 pad59[3];
    u16 x5c;
    u16 x5e;
    u8 pad60[4];
    f32 x64;
    u16 x68;
} Obj;
typedef struct {
    u8 pad[4];
    u16 big;
} Data;
void *ov96_021FC0D0(void *a, u32 b);
void *ov96_021EAA04(void *a, u8 b);
void *ov96_021EAA20(void *h);
Data *ov96_021E8BB0(void *a);
BOOL ov96_021E5F24(void *course);
u8 *ov96_021E60D8(void *course, int a, int b);
void *ov96_021E60C0(void *course, int a, u8 b);
int ov96_021E6138(void *a);
void ov96_021EAF70(void *h, u32 a, u32 b);
int ov96_021E6104();
void ov96_021EAF6C(void *h, u32 v);
void ov96_021EAF94(void *h, int x, int y);
void ov96_021EAC0C(void *h, u32 v);
void ov96_021EABA8(void *h, u32 v);
void ov96_021EABE0(void *h, int v);
void ov96_021EABF4(void *h, const VecFx32 *v);
void *ov96_021EB4F4(void *mgr, u8 a, u8 b);
Sprite *ov96_021EB5B8(void *obj);
void ov96_021EB52C(void *obj, int a, int b);
void ov96_021FA0E8(void *course, Params *params, u32 (*table)[2], void *a3, void *mgr, void *a5, u8 slot, Obj *obj);
// @fn ov96_021FA0E8
void ov96_021FA0E8(void *course, Params *params, u32 (*table)[2], void *a3, void *mgr, void *a5, u8 slot, Obj *obj) {
    VecFx32 scale;
    VecFx32 pos;
    VecFx32 pos2;
    VecFx32 pos3;
    void *h;
    int x;
    int k;
    void *t;
    Data *data;
    u8 *info;
    int n;
    Sprite *sprite;
    obj->x0 = 0;
    obj->x2 = 0;
    obj->x4 = 0x30;
    obj->x6 = 0x140;
    obj->slot = slot;
    obj->x68 = 1;
    obj->x64 = 1.0f;
    obj->x8 = 0;
    obj->x20 = ov96_021FC0D0(a3, slot);
    switch (slot) {
    case 0:
        obj->x14 = 0;
        obj->x16 = 4;
        break;
    case 1:
        obj->x14 = 1;
        obj->x16 = 5;
        break;
    case 2:
        obj->x14 = 2;
        obj->x16 = 6;
        break;
    }
    switch (slot) {
    case 0:
        k = 0;
        x = 0x30;
        break;
    case 1:
        k = 1;
        x = 0x80;
        break;
    case 2:
        k = 2;
        x = 0xd0;
        break;
    default:
        GF_AssertFail();
        return;
    }
    h = ov96_021EAA04(a5, k);
    t = ov96_021EAA20(h);
    obj->x24 = h;
    data = ov96_021E8BB0(t);
    info = ov96_021E60D8(course, ov96_021E5F24(course), slot);
    obj->x5c = params->a[0][info[2]];
    obj->x5e = params->a[1][info[2]];
    obj->x58 = info[2];
    n = ov96_021E6138(ov96_021E60C0(course, ov96_021E5F24(course), slot)) - 1;
    ov96_021EAF70(h, table[n][0], table[n][1]);
    ov96_021EAF6C(h, ov96_021E6104());
    obj->x50 = params->a[3][info[1]] / 10.0f;
    obj->x54 = params->a[2][info[4]] / 100.0f;
    if (data->big) {
        scale.y = scale.x = 0x20000;
    } else {
        scale.y = scale.x = 0x10000;
    }
    obj->x3c = 0x78;
    ov96_021EAF94(h, x, obj->x3c + 0x110);
    ov96_021EAC0C(h, 1);
    ov96_021EABA8(h, 4);
    ov96_021EABE0(h, 2);
    ov96_021EABF4(h, &scale);
    obj->x44 = ov96_021EB4F4(mgr, 0x66, 5);
    sprite = ov96_021EB5B8(obj->x44);
    obj->x40 = obj->x3c + 0x120;
    pos.z = 0;
    pos.x = x << FX32_SHIFT;
    pos.y = obj->x40 << FX32_SHIFT;
    Sprite_SetMatrix(sprite, &pos);
    ov96_021EB52C(obj->x44, 1, 0);
    Sprite_SetDrawPriority(sprite, 3);
    obj->x48 = ov96_021EB4F4(mgr, 0x68, 7);
    sprite = ov96_021EB5B8(obj->x48);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    pos2.z = 0;
    pos2.y = 0x1ac000;
    pos2.x = x << FX32_SHIFT;
    Sprite_SetMatrix(sprite, &pos2);
    Sprite_SetDrawPriority(sprite, 4);
    Sprite_SetAffineOverwriteMode(sprite, 2);
    obj->x4c = ov96_021EB4F4(mgr, 0x69, 8);
    sprite = ov96_021EB5B8(obj->x4c);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    pos3.z = 0;
    pos3.y = 0x188000;
    pos3.x = x << FX32_SHIFT;
    Sprite_SetMatrix(sprite, &pos3);
    ov96_021EB52C(obj->x4c, 1, 1);
    Sprite_SetDrawPriority(sprite, 2);
    obj->xc = 1.0f;
}
