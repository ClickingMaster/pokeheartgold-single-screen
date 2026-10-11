#ifndef TO45_OV41_R16_PRIVATE_H
#define TO45_OV41_R16_PRIVATE_H

#include <string.h>

#include "global.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void ov41_02248120(void *, void *, void *, s32, s32);
void BgSetPosTextAndCommit(void *, int, int, int);
void *Heap_Alloc(int, u32);
void *AllocMonZeroed(int);
void FashionMon_ToPokemon(void *, void *);
void ov41_02246544(void *, void *, int);
void ov41_022499B4(void *, int, int);
void *ov41_02245EA0(int, int);
void ov41_0224B848(void *, void *);
void ov41_0224B4E8(void *, void *, int);
void ov41_0224B888(void *, void *);
void *NARC_New(int, int);
void ov41_02246518(void *, void *, int);
void ov41_0224B938(void *, void *, void *, int);
void ov41_02246250(void *, void *);
void ov41_022463D4(void *);
void ov41_022465D8(void *, int, int, u16, void *);
void ov41_0224626C(void *);
void ov41_02246594(void *);
void ov41_022465C0(void *);
void NARC_Delete(void *);
void ov41_02247F3C(void *, void *);
int FashionMon_GetX(void *);
int FashionMon_GetY(void *);
int FashionMon_GetZ(void *);
void ov41_02247FAC(void *, void *, void *, int, int, int, int);
int FashionItem_GetId(void *);
int FashionItem_GetX(void *);
int FashionItem_GetY(void *);
int FashionItem_GetZ(void *);
void ov41_02248044(void *, int, int, int, int);
void ov41_0224B5D8(void *, s32, s32);
void *ov41_0224B630(void *);
void ov41_0224B6CC(void *, void *);
void ov41_0224B720(void *);
void ov41_0224B754(void *);
void ov41_0224B780(void *, void *);

#endif
