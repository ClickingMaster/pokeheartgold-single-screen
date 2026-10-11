#include "global.h"
#include "constants/sndseq.h"

#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))

extern u8 ov74_0223C2D0[];
extern u8 ov74_0223C2E0[];
extern u8 ov74_0223C2F0[];
extern u8 gSystem[];

extern int WindowIsInUse(void *);
extern void EraseFrameAndWindow1(void *, int);
extern void ClearFrameAndWindow2(void *, int);
extern void Sprite_Delete(void *);
extern void ov74_022359BC(void);
extern int ov74_0222D11C(void *, void *, int, void *, ...);
extern void ov74_0222D1D4(void *, int);
extern void BufferIntegerAsString(void *, int, int, int, int, int);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern int SaveMysteryGift_CardTagIsValid(void *, int);
extern void *OverlayManager_GetData(void *);
extern int ListMenu_ProcessInput(void *);
extern void PlaySE(u32);
extern int sub_02014AA0(void);
extern void *sub_02014A60(void *);
extern void sub_02014AB0(void *, u32, int, int);
extern void sub_02014A38(void *);
extern void SysTask_Destroy(void *);
extern int ov74_022360B0(void *);
extern void ov74_0223563C(void);
extern void ov74_02235690(void);
extern void ov74_02235728(int, int, int, int, int, int);
extern void *ov74_02235930(int, void *, int, int, int);
extern void Sprite_SetPriority(void *, int);
extern void ov74_0222D448(void *);
extern int ov74_0222D9E0(void *, void *, int, int);
extern int SaveMysteryGift_HasAnyGift(void *, void *);
extern void ListMenuItems_Delete(void *);
extern void DestroyListMenu(void *, int, int);
extern void ClearWindowTilemapAndCopyToVram(void *);
extern void RemoveWindow(void *);
extern void *String_New(int, int);
extern void CopyU16ArrayToStringN(void *, void *, int);
extern void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, int);
extern void String_Delete(void *);
extern void *GfGfxLoader_LoadFromNarc(int, int, int, int, int, int);
extern void BG_LoadScreenTilemapData(void *, int, void *, int);
extern void Heap_Free(void *);
extern void Heap_Create(int, int, int);
extern void *OverlayManager_CreateAndGetData(void *, int, int);
extern void *memset(void *, int, u32);
extern void *BgConfig_Alloc(int);
extern void SetMasterBrightnessByColor(int, int);
extern void ov74_022352A0(int);
extern void GF_AssertFail(void);
extern void SaveSubstruct_AssertCRC(int);
extern void SaveSubstruct_UpdateCRC(int);
extern void ov74_022360A0(void *);
extern void BufferMonthNameAbbr(void *, int, int);
extern void RTC_ConvertDayToDate(RTCDate *, s32);
extern void GfGfxLoader_GXLoadPal(int, int, int, int, int, int);
extern void GfGfxLoader_LoadCharData(int, int, void *, int, int, int, int, int);
extern void GfGfxLoader_LoadScrnData(int, int, void *, int, int, int, int, int);
extern void BgTilemapRectChangePalette(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void *MessageFormat_New(int);
extern int GetFontAttribute(int, int);
extern void FillWindowPixelBuffer(void *, int);
extern void *ReadMsgData_ExpandPlaceholders(void *, void *, int, int);
extern void CopyWindowToVram(void *);
extern void DestroyMsgData(void *);
extern void MessageFormat_Delete(void *);

void ov74_0222D024(void *);
void ov74_0222D0EC(void *, int);
void ov74_0222D104(void *, int);
int ov74_0222D490(void);
int ov74_0222D774(void *, void *);
int ov74_0222D78C(void *, void *);
int ov74_0222D7A4(void *, void *);
int ov74_0222D7D0(void *);
void ov74_0222D7F0(void *, int, int, int *);
int ov74_0222DAF8(void *, int, int);
void ov74_0222DC60(void *, int *, int (*)(void *));
void ov74_0222DE68(void *, void *);
void ov74_0222DEF0(void *);
void ov74_0222E03C(void *);
void ov74_0222CFFC(void *);
void ov74_0222D098(void *);
int ov74_0222D2D4(void *);
int ov74_0222D308(void *);
int ov74_0222D414(void *);
void ov74_0222D448(void *);
int ov74_0222D494(void *, void *, int);
int ov74_0222D5C4(void *, void *, int);
void ov74_0222DB30(void *, int, int, int);
int ov74_0222DDFC(void *, int *);
void ov74_0222E060(void *);
void ov74_0222D1D4(void *, int);
int ov74_0222D4E4(void *, void *, int);
int ov74_0222D55C(void *);
int ov74_0222D614(void *, void *, int);
void ov74_0222D248(void *);

int ov74_0222D55C(void *p) {
    RTCDate date;
    u32 i = U32(p, 0x2bc0);
    void *entry = PTR((u8 *)p + i * 4, 0x2bb4);
    RTC_ConvertDayToDate(&date, U32(entry, 0x354));
    BufferIntegerAsString(PTR(p, 0x2a00), 0, date.year + 2000, 4, 2, 1);
    BufferMonthNameAbbr(PTR(p, 0x2a00), 1, date.month);
    BufferIntegerAsString(PTR(p, 0x2a00), 2, date.day, 2, 2, 1);
    return 1;
}
int ov74_0222D5C4(void *p, void *win, int color) {
    void *s = String_New(0xfb, 0x55);
    u32 i = U32(p, 0x2bc0);
    void *entry = PTR((u8 *)p + i * 4, 0x2bb4);
    CopyU16ArrayToStringN(s, (u8 *)entry + 0x154, 0xfa);
    AddTextPrinterParameterizedWithColor(win, 1, s, 0, 0, 0xff, color, 0);
    String_Delete(s);
    return 0;
}
int ov74_0222D614(void *p, void *win, int color) {
    u32 i = U32(p, 0x2bc0);
    void *entry = PTR((u8 *)p + i * 4, 0x2bb4);
    u8 n = *(u8 *)((u8 *)entry + 0x348);
    if (n == 0xff) {
        void *str = ReadMsgData_ExpandPlaceholders(PTR(p, 0x2a00), PTR(p, 0x2a04), 0x34, 0x55);
        AddTextPrinterParameterizedWithColor(win, 1, str, 0, 0, 0xff, color, 0);
        String_Delete(str);
        return 0;
    }
    if (n != 0) {
        BufferIntegerAsString(PTR(p, 0x2a00), 0, n, 3, 0, 1);
        return 1;
    }
    return 0;
}
