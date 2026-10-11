#ifndef TO55_OVERLAY91_MERGE_PRIVATE_H
#define TO55_OVERLAY91_MERGE_PRIVATE_H
#include "global.h"

#include "math_util.h"
void ModelResource_Free(void *);
void ModelAnimation_Free(void *, void *);
void ModelAnimation_AdvanceLooped(void *, u32);
void ModelAnimation_SetFrame(void *, u32);
void ModelInstance_AddAnimation(void *, void *);
void ModelInstance_RemoveAnimation(void *, void *);
void ModelInstance_Draw(void *);
void ModelInstance_SetVisible(void *, int);
void ModelInstance_SetPosition(void *, u32, u32, u32);
void ModelInstance_SetScale(void *, u32, u32, u32);
void ModelInstance_SetRotation(void *, u16, int);
u32 ModelAnimation_Advance(void *, u32, u32, u32);
void ov91_02260378(void *);
void GF_AssertFail(void);
u32 _u32_div_f(u32, u32);
s32 _s32_div_f(s32, s32);
extern const u8 ov91_0226274C[];
void ov91_022600FC(u8 *, void *);
void ov91_0226014C(u8 *, u8 *, int);
void ov91_022601AC(u8 *);
void ov91_022601C8(u8 *, u8 *);
void ov91_022601F4(u8 *, u8 *);
void ov91_02260218(u8 *, u8 *);
void ov91_0226023C(u8 *);
void ov91_02260254(u8 *, u16);
void ov91_02260298(u8 *, int);
u32 ov91_0226031C(u8 *, u32, u32, u32);
void ov91_02260334(u8 *, int);
#define TO55_T07_PRIVATE_H

typedef struct Sprite Sprite;
typedef struct TextOBJ TextOBJ;
typedef struct String String;

void TextOBJ_Destroy(TextOBJ *);
void SpriteTransfer_DeletePlttTransferTask(void *);
void SpriteTransfer_DeleteCharTransferTask(void *);
void DestroySingle2DGfxResObj(void *, void *);
void sub_02021B5C(void *);
void sub_02013938(void *);
void String_Delete(String *);
void Sprite_Delete(Sprite *);
void TextOBJ_SetSpritesDrawFlag(TextOBJ *, BOOL);
void Sprite_SetDrawFlag(Sprite *, BOOL);
void ov91_0225D46C(void *);
void Sprite_SetMatrix(Sprite *, const void *);
void sub_02013728(TextOBJ *);
void String16_FormatInteger(String *, int, u32, int, BOOL);
void AddTextWindowTopLeftCorner(void *, void *, int, int, int, int);
u8 AddTextPrinterParameterizedWithColor(void *, int, String *, int, int, int, u32, void *);
void TextOBJ_CopyFromBGWindow(TextOBJ *, void *, void *, int);
void RemoveWindow(void *);
void SetBgPriority(u8, u8);
void BgTilemapRectChangePalette(void *, u8, int, int, int, int, u8);
void BgCommitTilemapBufferToVram(void *, u8);
void ov91_022614FC(void *, int, u8);
void ov91_02261790(void *, void *);
void ov91_02261808(void *, void *, int);
void ov91_02261890(void *);
void ov91_022618B0(void *, void *, int);
void ov91_02261928(void *);

#endif
