#pragma force_active on
#include "global.h"
typedef struct Work { int state; int kind; u8 pad8[0x20 - 8]; int idx; int pad24; u8 *obj; } Work;
extern const VecFx32 ov01_02209334;
extern const VecFx32 ov01_02209354[][2];
extern void Math_CalcRotationMatrixXYZDeg(MtxFx33 *mtx, u16 x, u16 y, u16 z);
extern void sub_02068DB8(void *, VecFx32 *);
extern void RenderObj_Draw(void *a0, VecFx32 *pos, VecFx32 *scale, MtxFx33 *mtx);
void ov01_02200A08(void *obj, Work *w);
// @fn ov01_02200A08
void ov01_02200A08(void *obj, Work *w){MtxFx33 mtx; VecFx32 pos; VecFx32 scale; const VecFx32 *ofs; void *dest; int angle; if (w->state == 1 || w->kind == -1) { return; } angle = 0; scale = ov01_02209334; dest = w->obj + 0x18; ofs = &ov01_02209354[w->kind][w->idx]; switch (w->kind) { case 0: angle = 180; break; case 1: break; case 2: angle = 270; break; case 3: angle = 90; break; } Math_CalcRotationMatrixXYZDeg(&mtx, 0, angle, 0); sub_02068DB8(obj, &pos); pos.x += ofs->x; pos.y += ofs->y; pos.z += ofs->z; RenderObj_Draw(dest, &pos, &scale, &mtx);}
