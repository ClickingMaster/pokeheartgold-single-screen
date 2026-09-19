#ifndef TO52_SOL_R3_W19_PRIVATE_H
#define TO52_SOL_R3_W19_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
void GX_LoadBGPltt(const void *, u32, u32);
void GXS_LoadBGPltt(const void *, u32, u32);
void AddWindowParameterized(void *, void *, int, int, int, int, int, int, int);
void FillWindowPixelBuffer(void *, int);
void ov39_0222899C(void *, void *, int, int, u32, int);
void *NARC_New(int, int);
void NARC_Delete(void *);
void GfGfxLoader_GXLoadPalFromOpenNarc(void *, int, int, int, int, int);
void LoadFontPal1(int, int, int);
void *Save_PlayerData_GetOptionsAddr(void *);
int Options_GetFrame(void *);
void LoadUserFrameGfx2(void *, int, int, int, int, int);
void LoadUserFrameGfx1(void *, int, int, int, int, int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void BG_SetMaskColor(int, int);
void *GfGfxLoader_GetPlttDataFromOpenNarc(void *, int, void *, int);
void Heap_Free(void *);
void BlendPalette(const void *, void *, int, int, int);
void GF_AssertFail(void);
void *SysTask_CreateOnVBlankQueue(void *, void *, int);
void ov39_022285CC(void *);
void ov39_022287F8(void *, void *);
void ov39_022288A0(void *);
#endif
