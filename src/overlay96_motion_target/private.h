#ifndef W13_R7_PRIVATE_H
#define W13_R7_PRIVATE_H
#include "global.h"
#include <nitro/fx/fx_vec.h>
typedef struct Obj120 {
    void *unk00; void *unk04; void *unk08; void *unk0C; void *unk10;
    s32 state; u8 pad18[8]; VecFx32 vec20; VecFx32 vec2C; VecFx32 vec38; VecFx32 vec44;
    s32 unk50; s32 unk54; s8 unk58; s8 unk59; s8 unk5A; s8 unk5B; u8 unk5C; s8 unk5D; s8 unk5E; u8 unk5F; u32 flags;
} Obj120;
void GF_AssertFail(void);
void ov96_02218578(Obj120 *, int);
int ov96_022186CC(const VecFx32 *);
void ov96_02218DF8(Obj120 *, const s16 *);
#endif
