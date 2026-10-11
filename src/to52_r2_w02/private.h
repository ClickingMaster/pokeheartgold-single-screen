#ifndef TO52_W02_OV73_H
#define TO52_W02_OV73_H
#include "global.h"
#pragma require_prototypes off
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
typedef struct { u32 v[4]; } W02Modes;
typedef struct { u8 pad00[0x10]; u32 index; } W02Work;
typedef struct { int (*f)(void *); u32 pad[2]; } W02Fn1;
typedef struct { int (*f)(void *, int); u32 pad[2]; } W02Fn2;
extern void *_021EA940[];
extern W02Modes ov73_021EA68C;
extern W02Fn1 ov73_021EA83C[];
extern W02Fn1 ov73_021EA840[];
extern W02Fn2 ov73_021EA844[];
void Main_SetVBlankIntrCB(void *, void *);
void HBlankInterruptDisable(void);
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void Heap_Create(int, int, u32);
void *OverlayManager_CreateAndGetData(void *, u32, int);
void *OverlayManager_GetData(void *);
void *BgConfig_Alloc(int);
void SetBothScreensModesAndDisable(void *);
void *MessageFormat_New_Custom(int, int, int);
void *NewMsgDataFromNarc(int, int, int, int);
void SetKeyRepeatTimers(int, int);
void ov73_021E8148(void *, void *);
void ov73_021E8454(void *);
void Sound_SetSceneAndPlayBGM(int, int, int);
void *Heap_Alloc(int, u32);
void LoadDwcOverlay(void);
void LoadOVY38(void);
void sub_02034D8C(void);
void ov00_021ECB40(void);
void NintendoWfcHttp_Update(void);
int sub_02034DB8(void);
void *ov73_021E83F4(void *, u32, s32);
void ov73_021E841C(void *, void *);
void ov00_021EC294(void *, void *);
int IsPaletteFadeFinished(void);
void SpriteList_RenderAndAnimateSprites(void *);
#endif
