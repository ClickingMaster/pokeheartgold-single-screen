#ifndef SOL40_OV49_R52_PRIVATE_H
#define SOL40_OV49_R52_PRIVATE_H
#include "global.h"
typedef struct { void *unk0; u16 state; u16 pad6; void *unk8; } Ov49Small;
typedef struct { u8 raw[0x24]; } Ov49Entry;
typedef struct { int heap; void *ctx; Ov49Entry entries[20]; Ov49Entry extra; } Ov49Work;
void WiFiPlazaProp_SetAnimSpeed(void *, void *);
void WiFiPlazaModelSet_StartPropAnim(void *, void *, int, int);
void WiFiPlazaProp_SetVisible(void *, int);
int WiFiPlazaModelSet_IsPropAnimActive(void *, void *, int);
void WiFiPlazaProp_ClearAlpha(void *);
void WiFiPlazaProp_SetScale(void *, int, int, int);
void WiFiPlazaPropShow_StartFade(Ov49Small *);
int WiFiPlazaPropShow_UpdateFade(Ov49Small *);
u32 WiFiPlazaProp_GetPos(void *);
typedef struct { s16 x, y; } Ov49Pair;
void WiFiPlazaProp_SetPos(void *, Ov49Pair);
extern const s32 sWiFiPlazaPropTagShiftX[];
void *Heap_Alloc(int, u32);
void Heap_Free(void *);
void *memset(void *, int, u32);
void ov49_0225F068(void *, int, int);
void ov49_0225F074(void *);
void WiFiPlazaPropShow_ShowStage(Ov49Small *, void *);
void WiFiPlazaPropShow_UpdateStage(Ov49Small *, void *);
void WiFiPlazaPropShow_UpdateBurst(Ov49Small *, void *);
void WiFiPlazaPropShow_ShowBurst(Ov49Small *, void *);
void WiFiPlazaProp_ApplyTagShift(void *, int);
Ov49Work *ov49_0225EEAC(void *, int);
void ov49_0225EEF8(Ov49Work *);
#endif
