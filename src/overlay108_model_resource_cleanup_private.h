#include "global.h"
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
void Heap_Free(void *);
void MI_CpuFill8(void *, u8, u32);
void ov108_021E91D4(void *, void *);
void ov108_021E9204(void *, void *);
void ov108_021E9210(void *);
void ov108_021E90C4(void *, void *);
void ov108_021E9144(void *);
void MTX_Identity33_(MtxFx33 *);
void GF3dRender_DrawModel(void *, const VecFx32 *, const MtxFx33 *, const VecFx32 *);
