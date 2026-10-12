#ifndef OVERLAY40_TRAINER_CARD_PANELS_H
#define OVERLAY40_TRAINER_CARD_PANELS_H

#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define HALF(p, off) (*(u16 *)((u8 *)(p) + (off)))
#define BYTE(p, off) (*(u8 *)((u8 *)(p) + (off)))

#define BLEND(p, w, a, b) \
    PaletteData_BlendPalettes(PTR(p, 0x28), a, b, (u8)WORD(w, 8), (u16)WORD(p, 0x58))

typedef struct Ov40Window {
    u32 raw[4];
} Ov40Window;

typedef struct Ov40Quad {
    u32 v[4];
} Ov40Quad;

void PaletteData_BlendPalettes(void *palette, int a, int b, int coeff, int color);
void BgClearTilemapBufferAndCommit(void *bgConfig, int bgId);
void GfGfx_EngineATogglePlanes(int mask, int enable);
void GfGfx_EngineBTogglePlanes(int mask, int enable);
void TouchHitboxController_Destroy(void *controller);
void TouchHitboxController_IsTriggered(void *controller);
void Heap_Free(void *ptr);
BOOL System_GetTouchNew(void);

void InitWindow(void *window);
void AddWindowParameterized(void *bgConfig, void *window, int bgId, int x, int y, int width, int height, int palette, int baseTile);
void AddTextWindowTopLeftCorner(void *bgConfig, void *window, int width, int height, int a, int b);
void FillWindowPixelBuffer(void *window, int fillValue);
void AddTextPrinterParameterizedWithColor(void *window, int fontId, void *str, int x, int y, int speed, u32 color, void *callback);
void ScheduleWindowCopyToVram(void *window);
void RemoveWindow(void *window);
void TextOBJ_CopyFromBGWindow(void *a, void *b, void *window, int heapId);

void *String_New(u32 size, u32 heapId);
void String_Delete(void *str);
void *NewString_ReadMsgData(void *msgData, int msgId);
void BufferString(void *fmt, u32 fieldno, void *str, int a3, int a4, int a5);
void StringExpandPlaceholders(void *fmt, void *dst, void *src);
void MessageFormat_Delete(void *fmt);

void ManagedSprite_SetPositionXY(void *sprite, s16 x, s16 y);
void ManagedSprite_SetAnim(void *sprite, int anim);
void Sprite_DeleteAndFreeResources(void *sprite);
void TextOBJ_SetSpritesDrawFlag(void *textOBJ, int draw);
void TextOBJ_SetPosition(void *p, s32 a, s32 b);

void *BattleRecord_GetHeader(void);
u64 BattleRecordHeader_GetField(void *a, int b, int c);
void *FrontierCard_NewNameString(u32 a, int heapId);
int BattleRecord_Exists(void);
void BattleRecord_Delete(void);

void *VsRecorder_NewMessageFormat(int heapId);
void VsRecorder_SetScreenIfDone(void *p, u32 value, BOOL valid, u32 *out);
void VsRecorder_SetSubState(void *p, int value);
void ov40_0222C6C8(void *p, int a);
void VsRecorder_CreateTextSprite(void *dst, void *src, int a);
void VsRecorder_SetTextSpriteMsg(void *dst, void *src, int a);
void VsRecorder_FreeTextSprite(void *p);
void ov40_0222D78C(void *p, int a);
void *VsRecorder_NewButtonSprite(void *p, int a);
void VsRecorder_FreeButtonGfx(void *p);
void VsRecorder_RebuildMenuRowsParent(void *p);
int VsRecorder_StepBlend(int *a, int *b, int c, int d);
int VsRecorder_StepFade(int *p, int direction);
void VsRecorder_ResetFade(void *p);
void VsRecorder_ClearPaletteBlend(void *p);
void VsRecorder_ShowTopMessage(void *p, int a);
void VsRecorder_HideTopMessage(void *p);
void VsRecorder_FreeProfile(void *p, void *q);
void VsRecorder_StartFlipScreens(void *p, int a);
void *VsRecorder_IsFlipScreensDone(void *p);
void VsRecorder_FreeVideoInfo(void *p, void *q);
void VsRecorder_SetVBlankPaused(void *p, void *value);
void VsRecorder_SanitizeName(void *p, void *str);
BOOL VsRecorder_IsVideoSaved(void *p, u64 v);
void ov40_02241A34(void *p);
void ov40_022421FC(void *p);
BOOL ov40_0224222C(void *p);
BOOL ov40_02242378(void *p);
BOOL ov40_02242490(void *p);
BOOL ov40_0224253C(void *p);
BOOL ov40_022428D4(void *p);
BOOL ov40_02242AEC(void *p);
BOOL ov40_02242CFC(void *p);

extern const Ov40Quad ov40_02245684;
extern const Ov40Quad ov40_02245694;

void ov40_02240910(void *p);
void ov40_02240D50(void *obj, void *p, int idx);
void ov40_02240E28(void *p, int msgId, int unused);
void ov40_02240F24(void *p, int msgId, int idx);
void ov40_02241054(void *p);
void ov40_02241114(void *p);

int ov40_02240B40(void *p);
int ov40_02240B58(void *p);
int ov40_02240B70(void *p);
int ov40_02240B90(void *p);

#endif
