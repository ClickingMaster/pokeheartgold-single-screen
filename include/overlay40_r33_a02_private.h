#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define BYTE(p, off) (*(u8 *)((u8 *)(p) + (off)))

typedef struct Ov40PositionTable {
    s16 xy[30][2];
} Ov40PositionTable;

extern Ov40PositionTable ov40_0224557C;

void ManagedSprite_SetPositionXY(void *sprite, s16 x, s16 y);
void ManagedSprite_SetAnim(void *sprite, int anim);
void TextOBJ_SetSpritesDrawFlag(void *obj, BOOL draw);
void TextOBJ_SetPosition(void *obj, int x, int y);
void ov40_0222D6EC(void *p, int which);
void *ov40_0222D800(void *p, int which);
void VsRecorder_CreateTextSprite(void *dst, void *src, int id);
void VsRecorder_SetTextSpriteMsg(void *dst, void *src, int id);
void ov40_0223B4BC(void *p, int value);

void *VsRecorder_NewMessageFormat(int heapId);
void *String_New(u32 maxLength, int heapId);
void *FrontierCard_NewNameString(u32 value, int heapId);
void VsRecorder_SanitizeName(void *p, void *value);
void *NewString_ReadMsgData(void *msgData, int msgId);
void BufferString(void *messageFormat, u32 idx, void *string, u32 a3, u32 a4, u32 a5);
void StringExpandPlaceholders(void *messageFormat, void *dest, void *src);
void String_Delete(void *string);
void MessageFormat_Delete(void *messageFormat);
void FillWindowPixelBuffer(void *window, u8 fillValue);
u8 AddTextPrinterParameterizedWithColor(void *window, int fontId, void *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
void ScheduleWindowCopyToVram(void *window);
BOOL ov40_0223D5CC(void *p);
void BgClearTilemapBufferAndCommit(void *bgConfig, int bgId);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *obj, int value);
void VsRecorder_SetProfileDrawFlag(void *obj, int value);
int VsRecorder_StepFade(int *p, int direction);
BOOL VsRecorder_StepBlend(int *a, int *b, int c, int d);
void PaletteData_BlendPalettes(void *palette, int buffer, int mask, int coeff, int color);
void VsRecorder_ShowTopMessage(void *p, int msgId);
void VsRecorder_PlaceOrbitCursor(void *p, void *obj, int x, int y);
void OrbitCursor_SetVisible(void *obj, int value);
void OrbitCursor_SetRadius(void *obj, int x, int y);
void PlaySE(u16 seqNo);
void StopSE(u16 seqNo, int fadeFrames);
void *ov40_0223D540(void *p);
BOOL GlobalTerminalNet_RequestVideoDownload(void *result, void *a, void *b);
BOOL GlobalTerminalNet_GetError(void *p, void **result);
void VsRecorder_HideTopMessage(void *p);
void VsRecorder_ShowMessageScreen(void *p, int kind, u32 a, u32 b);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void VsRecorder_SetSubState(void *p, int state);
void VsRecorder_SetBattleResult(void *p, int value);

void ov40_0223A3BC(void *p);
void ov40_0223A430(void *p);
void ov40_0223A510(void *p, int msgId, int index);
int ov40_0223A640(void *p);
