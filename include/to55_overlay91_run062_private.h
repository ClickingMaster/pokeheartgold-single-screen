#ifndef TO55_T11_PRIVATE_H
#define TO55_T11_PRIVATE_H

#include "global.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include <nitro/fx/fx_mtx33.h>
#include <nitro/fx/fx_trig.h>

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct T11Entry {
    u8 pad00[0xF8];
    void *target;
    u8 padFC[0xC];
} T11Entry;

typedef struct T11Work {
    u8 pad000[0x790];
    T11Entry entries[0x60];
} T11Work;

extern const u16 ov91_02261D3C[][4];
int MTX_Inverse33(const MtxFx33 *, MtxFx33 *);
void ov91_02260830(void *, void *);
void *ov91_02260A50(void *);
void ov91_02260848(void *, void *, void *, void *);
void ov91_02260884(void *, void *, void *);
void ov91_02260A2C(void *);
void ov91_022608A8(void *, void *, void *);
void GF_AssertFail(void);
void sub_02018068(void *);
void sub_020181B0(void *, void *);
void sub_020182A0(void *, int);

void ov91_0226042C(const VecFx32 *, u16 *);
void ov91_0226045C(const VecFx32 *, VecFx32 *, int, int);
void ov91_022604A4(const VecFx32 *, VecFx32 *, int, int);
void ov91_0226066C(void *);
void ov91_02260728(void *, void *, void *, u16);
void ov91_02260754(void *, void *);
void ov91_0226078C(void *, void *);
void ov91_022607C4(void *, void *, enum HeapID);

#endif
