#pragma force_active on
#include "global.h"
typedef struct { s16 x; s16 y; } Pt16;
typedef struct { void *unk0; u32 unk4; u32 idx:2; u32 unk8_2:2; u32 flag:1; u32 unk8_5:27; } Obj96;
extern const Pt16 ov96_0221D98C[];
extern const s16 ov96_0221D98E[];
extern void ov96_02215DEC(void *, u8, VecFx32 *);
extern void ov96_02215F80(void *, u8, const Pt16 *);
extern BOOL ov96_0221A034(VecFx32 *, VecFx32 *, fx32);
extern void ov96_02215F64(void *, u8);
void ov96_0221A314(Obj96 *o);
// @fn ov96_0221A314
void ov96_0221A314(Obj96 *o){const Pt16 *pt = &ov96_0221D98C[o->idx]; VecFx32 a = {0, 0, 0}; VecFx32 b = {0, 0, 0}; ov96_02215DEC(o->unk0, o->idx, &a); b.x = ov96_0221D98C[o->idx].x << 12; b.y = ov96_0221D98E[o->idx * 2] << 12; ov96_02215F80(o->unk0, o->idx, pt); if (ov96_0221A034(&a, &b, 0x40000)) { ov96_02215F64(o->unk0, o->idx); o->flag = 0; }}
