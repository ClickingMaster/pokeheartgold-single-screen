#pragma force_active on
#include "global.h"
typedef struct {
    void *x0[0x70 / 4];
    void *x70;
    u8 pad[0x9f - 0x74];
    u8 x9f;
    u8 xa0;
} Obj;
typedef struct {
    u8 pad[0x1d];
    u8 x1d;
} Info;
void PokeathlonCourseObject_GroundToCenter(void *host, s32 x, s32 y, s32 *a, s32 *b);
void ov96_021FEBF0(void *course, Obj *obj, VecFx32 *pos, u8 a4, u8 a5, int a6, int flag, int a8);
void ov96_021FED3C(Obj *obj, VecFx32 *pos, u8 a4, u8 a5, int a6, int flag, int a8);
void ov96_021FEE60(Obj *obj, u8 a4, u8 a5, int a6, int flag, int a8);
void PokeathlonCourseSpritePoolSprite_SetPosition(void *x, VecFx32 *pos);
void ov96_021FEECC(void *course, Obj *obj, VecFx32 *in, Info *info, u8 a4, u8 a5, int a6, int a7, int a8, int a9);
// @fn ov96_021FEECC
void ov96_021FEECC(void *course, Obj *obj, VecFx32 *in, Info *info, u8 a4, u8 a5, int a6, int a7, int a8, int a9) {
    u16 changed = FALSE;
    u8 state = (info->x1d >> (a4 * 2)) & 3;
    s32 x, y;
    VecFx32 pos;
    BOOL flag;
    if (obj->x9f != state) {
        obj->x9f = state;
        obj->xa0 = 0;
        changed = TRUE;
    }
    PokeathlonCourseObject_GroundToCenter(obj->x0[a5], in->x / FX32_ONE, in->y / FX32_ONE, &x, &y);
    pos.x = x << FX32_SHIFT;
    pos.y = y << FX32_SHIFT;
    pos.z = 0;
    switch (obj->x9f) {
    case 1:
        ov96_021FEBF0(course, obj, &pos, a4, a5, a6, a7 && a9, a8);
        break;
    case 2:
        flag = a7 && a9;
        ov96_021FED3C(obj, &pos, a4, a5, a6, flag | a8, a8);
        break;
    case 0:
        if (changed) {
            ov96_021FEE60(obj, a4, a5, a6, (a7 && a9) | a8, a8);
        }
        PokeathlonCourseSpritePoolSprite_SetPosition(obj->x70, &pos);
        break;
    }
}
