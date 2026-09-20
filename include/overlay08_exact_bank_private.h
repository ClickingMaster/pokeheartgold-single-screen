#ifndef POKEHEARTGOLD_OVERLAY08_EXACT_BANK_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY08_EXACT_BANK_PRIVATE_H

#include "global.h"

typedef struct Ov08SmallState {
    void *owner;
    const u8 *entries;
    u8 state;
    u8 index;
    u8 selection;
    u8 padding;
    s32 result;
} Ov08SmallState;

typedef struct Ov08WorkMode {
    u8 padding[0x2077];
    u8 mode : 4;
    u8 upper : 4;
} Ov08WorkMode;

extern int TouchscreenHitbox_FindRectAtTouchNew(const void *hitboxes);
extern void ClearFrameAndWindow2(void *window, BOOL dontCopy);
extern u8 TextPrinterCheckActive(u8 printerId);
extern void WindowArray_Delete(void *windows, s32 count);
extern void *Heap_Alloc(u32 heapId, u32 size);
extern void Heap_Free(void *ptr);

extern void ov08_0221D840(void *work, int state);
extern void ov08_02223300(void *work, int state);
extern void ov08_022201C0(void *work);
extern void ov08_0221D81C(void *work);
extern void ScheduleBgTilemapBufferTransfer(void *bgConfig, int bgId);
extern void ov08_0222377C(void *work);
extern void ov08_022238F4(void *work);
extern void ov08_022237C4(void *work);
extern u32 BattleSystem_GetBattleType(void *battleSystem);
extern void ManagedSprite_SetDrawFlag(void *sprite, BOOL draw);
extern void ManagedSprite_SetPositionXY(void *sprite, s16 x, s16 y);
extern void ManagedSprite_SetAnim(void *sprite, u32 anim);
extern u32 ManagedSprite_GetActiveAnim(void *sprite);
extern void ManagedSprite_SetAnimationFrame(void *sprite, int frame);
extern void ov08_02221294(void *work);
extern void ov08_02221328(void *work);
extern void DestroyMsgData(void *msgData);
extern void MessagePrinter_Delete(void *printer);
extern void MessageFormat_Delete(void *format);
extern void String_Delete(void *string);
extern void BattleCursor_Disable(void *cursor);

int ov08_0221C918(void *work);
int ov08_0221C924(void *work);
int ov08_0221C930(void *work);
int ov08_0221C93C(void *work);
int ov08_0221C948(void *work);
int ov08_0221CA08(u8 *work);
int ov08_0221CA20(u8 *work);
int ov08_0221CA78(const u8 *work);
int ov08_0221CA34(const u8 *work);
int ov08_0221D5D0(void *unused, const void *hitboxes);
BOOL ov08_0221DAE4(void *work);
BOOL ov08_0221DB04(void *work);
void ov08_0221DBB4(u8 *work);
void ov08_0221DD28(u8 *work);
int ov08_02222D78(void *work);
int ov08_02222D84(void *work);
int ov08_02222D90(void *work);
int ov08_02222D9C(u8 *work);
int ov08_02222DAC(const u8 *work);
int ov08_02222DEC(const u8 *work);
void ov08_02220A8C(void *sprite, int x, int y);
void ov08_02220C3C(BOOL enabled, void *sprite, int x, int y);
void ov08_022213C8(Ov08WorkMode *work);
void ov08_022213E4(void *sprite, u32 anim);
void ov08_02223228(u8 *work);
int ov08_02223368(void *unused, const void *hitboxes);
void ov08_02223454(u8 *work);
void ov08_022239B4(void *work);
void ov08_02223F74(void *sprite, int x, int y);
void ov08_0222417C(u8 *work);
Ov08SmallState *ov08_02224B64(u32 heapId);
void ov08_02224B7C(Ov08SmallState *state);
void *ov08_02224B84(const Ov08SmallState *state);
u8 ov08_02224B88(const Ov08SmallState *state);
u8 ov08_02224B8C(const Ov08SmallState *state);
void ov08_02224B90(Ov08SmallState *state, u8 value);
void ov08_02224B94(Ov08SmallState *state, void *owner);
void ov08_02224BC0(Ov08SmallState *state);
void ov08_02224BF8(Ov08SmallState *state, s32 value);

#endif
