#ifndef TO52_CARD_AGENT3_PRIVATE_H
#define TO52_CARD_AGENT3_PRIVATE_H

#include "global.h"

#include "heap.h"
#include "overlay_43.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "system.h"

#define PTR_AT(p, o) ((void *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))

extern u8 SDK_OVERLAY_OVY_42_ID[];

void ov43_0222A148(void *work);
void ov43_0222A290(void *sub, void *args, u32 heapId);
void ov43_0222A2F0(void *sub, void *args);
void ov43_0222A48C(void *sub, void *shared, u32 heapId);
void ov43_0222A500(void *sub);
void ov43_0222A520(void *sub);
void ov43_0222AE5C(void *sub, void *shared, void *base, u32 heapId);
int ov43_0222AE64(void *sub, void *shared, void *base, u32 heapId);
void ov43_0222B098(void *sub, void *shared, void *base);
void ov43_0222B5A8(void *sub, void *shared, void *base, u32 heapId);
int ov43_0222B5D0(void *sub, void *shared, void *base, u32 heapId);
void ov43_0222B93C(void *sub, void *shared, void *base);
void ov43_0222CBAC(void *sub, void *shared, void *base, u32 heapId);
int ov43_0222CBB4(void *sub, void *work, void *shared, void *base, u32 heapId);
void ov43_0222CD34(void *sub, void *shared, void *base);
void ov43_0222CD90(void *sub, void *shared, void *base, u32 heapId);
int ov43_0222CD98(void *sub, void *shared, void *base);
void ov43_0222CE0C(void *sub, void *shared, void *base);
void ov43_0222D134(void *sub, void *shared, void *base, u32 heapId);
int ov43_0222D15C(void *sub, void *shared, void *base, u32 heapId);
void ov43_0222D228(void *sub, void *shared, void *base);

#endif
