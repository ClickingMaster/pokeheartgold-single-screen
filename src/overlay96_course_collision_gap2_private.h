#ifndef TO52_R3_W17_PRIVATE_H
#define TO52_R3_W17_PRIVATE_H
#include "global.h"
#include "overlay96_course_collision_r87_private.h"

typedef struct Ov96PointW17 { s32 x, y; } Ov96PointW17;
void ov96_02206F1C(u8 *, fx32, fx32, void **, u8 *);
int ov96_0220703C(const VecFx32 *, const void *, int, const Ov96PointW17 *, VecFx32 *);
u8 ov96_02207300(u8 *, int, int, const VecFx32 *, const void *, VecFx32 *);
#endif
