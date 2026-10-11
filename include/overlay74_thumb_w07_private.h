#ifndef TO53_R3_W07_PRIVATE_H
#define TO53_R3_W07_PRIVATE_H

#include "global.h"

#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

void *sub_02034818(int);
void MysteryGiftComm_Free(void);
void WirelessStatusIcon_Destroy(void);
BOOL WindowIsInUse(void *);
void EraseFrameAndWindow1(void *, int);
void ClearFrameAndWindow2(void *, int);
void BeginNormalPaletteFade(int, int, int, int, int, int, int);
void sub_02014AA0(void);
void *sub_02014A60(void *);
void sub_02014AB0(void *, void *, int, int);
void sub_02014A38(void *);
void SysTask_Destroy(void *);
void *String_New(int, int);
void CopyU16ArrayToStringN(void *, const u16 *, int);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
void String_Delete(void *);
void *ReadMsgData_ExpandPlaceholders(void *, void *, int, int);
void ov74_0222FCC4(void *);
void *Heap_AllocAtEnd(int, u32);
void *memset(void *, int, u32);
void LoadFontPal0(int, int, int);
void ov74_0222D824(void *, int);
void Heap_Free(void *);
void *GfGfxLoader_LoadFromNarc(int, int, int, int, int);
void BG_LoadScreenTilemapData(void *, u8, const void *, u32);
void Heap_Create(int, int, u32);
void *OverlayManager_CreateAndGetData(void *, u32, int);
void *BgConfig_Alloc(int);
void SetMasterBrightnessByColor(int, int);
void ov74_022352A0(int);

int ov74_0222E85C(s32 *);
int ov74_0222E898(void);
void ov74_0222EB28(void *, int *, int);
void ov74_0222ECD4(void *, int);
void ov74_0222ECEC(void *, int);
u32 ov74_0222EE08(void);
u32 ov74_0222EF64(void);
void ov74_0222EFF0(void *, int, int, int *);
void ov74_0222F600(void *, void *);
void ov74_0222F688(void *);
int ov74_0222EE0C(void *, void *, int);
int ov74_0222EE58(void *, void *, int);
int ov74_0222EF18(void *, void *, int);
void ov74_0222EC08(void *, void *, int);
void ov74_0222F2D4(void *, int, int, int);
int ov74_0222F598(void *);

#endif
