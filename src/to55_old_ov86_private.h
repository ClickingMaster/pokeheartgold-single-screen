#ifndef W18_R4_PRIVATE_H
#define W18_R4_PRIVATE_H
#include "global.h"
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern const u8 ov86_021E81DC[];
extern const u8 ov86_021E7E9C[];
extern u8 gSystem[];
u32 GridInputHandler_HandleInput_NoHold(void *);
u32 GridInputHandler_GetNextInput(void *);
int TouchscreenHitbox_FindRectAtTouchNew(const void *);
void PlaySE(u32);
void ov86_021E71C0(void *);
void ov86_021E7B68(void *);
void ov86_021E7CF8(void *);
void ov86_021E78B8(void *, int);
void ov86_021E792C(void *, u8, u8);
u8 ov86_021E7984(void *);
#endif
