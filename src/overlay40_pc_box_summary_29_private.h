#ifndef OVERLAY40_PC_BOX_SUMMARY_29_PRIVATE_H
#define OVERLAY40_PC_BOX_SUMMARY_29_PRIVATE_H

#include "global.h"

#define PTR(p, off)   (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off)  (*(s32 *)((u8 *)(p) + (off)))
#define UWORD(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define HALF(p, off)  (*(u16 *)((u8 *)(p) + (off)))
#define BYTE(p, off)  (*(u8 *)((u8 *)(p) + (off)))

typedef void (*HitboxCallback)(u32 a, int b, void *p);

void *Heap_Alloc(u32 heapId, u32 size);
void Heap_Free(void *ptr);
void BgClearTilemapBufferAndCommit(void *bgConfig, int bgId);
void *TouchHitboxController_Create(const void *hitboxes, int count, HitboxCallback cb, void *param, int heapId);
void *SaveArray_PCStorage_Get(void *saveData);
BOOL PlayCry(u16 species, u8 form);

void *FrontierCard_New(int heapId);
void FrontierCard_InitFromSave(void *a, void *b);
BOOL OrbitCursor_SetTarget(void *p, s16 x, s16 y);
void OrbitCursor_SetVisible(void *p, int a);
void OrbitCursor_SetRadius(void *p, int a, int b);

void GlobalTerminalNet_PackBoxData(void *a, void *b, u32 c, void *d, int heapId);

void VsRecorder_SetSubState(void *p, int value);
void ov40_0222D73C(void *p, int a);
void ov40_0222D7DC(void *p);
void VsRecorder_InitBgBlend(int *a, int *b, int c);
void VsRecorder_OnTouchConfirm(void *p);
void VsRecorder_SetVBlankPaused(void *p, int value);
void VsRecorder_CreateLabeledSprite(void *p, void *q, int a, int b, int c, int d, int e, int f, int g, int h);
void VsRecorder_FreeLabeledSprite(void *p);
int ov40_022361C8(void *p);
int ov40_022371D4(void *p, u32 mask);
void ov40_022371E4(void *p, int idx);
void ov40_02237474(void *p);
void ov40_02237564(void *p);

extern const u8 ov40_0224526C[];
extern const u8 ov40_02245284[];
extern const u8 ov40_022452CC[];
extern const u8 ov40_022452F4[];

void ov40_022361E0(void *w);
void ov40_02236230(u32 a, int b, void *p);
void ov40_022362E4(u32 a, int b, void *p);
void ov40_02236320(u32 a, int b, void *p);
void ov40_0223635C(u32 a, int b, void *p);
void ov40_022363BC(u32 a, int b, void *p);
void ov40_022364D0(void *p);
void ov40_02236534(void *p);
void ov40_0223655C(void *w);
void ov40_02236578(void *a, void *b, u32 c, void *d);
int ov40_022365A0(void *p);
void ov40_02237030(void *p, int a);

#endif
