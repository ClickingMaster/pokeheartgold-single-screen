#ifndef TO47_OV49_R13_PRIVATE_H
#define TO47_OV49_R13_PRIVATE_H

#include "global.h"

#include "overlay49_entry40.h"

extern const u8 ov49_02269658[];
extern const u8 ov49_0226965C[];

s32 ov42_02228188(struct UnkStruct_ov42_02228110 *, s32);
void ov45_0223089C(void *, int);
void ov45_022308E4(void *, Ov49R13Position *);
void ov45_02230908(void *, Ov49R13Position *);
void ov45_0223093C(void *, int);
void ov45_02230968(void *);
void ov49_02258E7C(void *, Ov49R13Work *, int, int);
u8 ov49_02259294(const u8 *, u32);
BOOL ov49_022592A8(void *, Ov49R13Work *, int, int);
void ov49_02259320(Ov49R13Interpolation *, u32, u32, u32);
BOOL ov49_0225932C(Ov49R13Interpolation *, u32);
u32 ov49_022593BC(Ov49R13Interpolation *);
void PlaySE(u16);

void ov49_022594D8(void *, void *);
void ov49_022595CC(void *, void *);
void ov49_0225967C(void *, void *);

#endif
