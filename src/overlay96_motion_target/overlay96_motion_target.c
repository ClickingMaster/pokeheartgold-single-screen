#include "private.h"

void ov96_02218DF8(Obj120 *p, const s16 *target) {
    VecFx32 vec = {0, 0, 0};
    BOOL valid = TRUE;
    if (p->state != 1 && p->state != 4) valid = FALSE;
    if (!valid) GF_AssertFail();
    if ((p->flags << 3) >> 31) GF_AssertFail();
    if ((p->flags << 5) >> 31) GF_AssertFail();
    p->flags &= 0xBFFFFFFF;
    vec.x = target[0] << 12;
    vec.y = target[1] << 12;
    VEC_Subtract(&vec, &p->vec2C, &p->vec38);
    VEC_Normalize(&p->vec38, &p->vec38);
    VEC_MultAdd(1 << 14, &p->vec38, &p->vec38, &p->vec44);
    if (p->vec44.x != 0 || p->vec44.y != 0) {
        p->flags = (p->flags & 0xFFF0FFFF) | (((u32)ov96_022186CC(&p->vec38) << 28) >> 12);
        p->unk59 = 4;
        ov96_02218578(p, 5);
    }
}
