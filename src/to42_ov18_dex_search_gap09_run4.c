#include "sol_r9_ov18_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8_(p, o) (*(u8 *)((u8 *)(p) + (o)))

typedef struct Dims18 {
    u16 height;
    u16 weight;
} Dims18;

void *GfGfxLoader_GetCharDataFromOpenNarc(void *narc, s32 memberNo, BOOL isCompressed, void **ppCharData, u32 heapID);
void BlitBitmapRect(void *window, void *src, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, u16 destX, u16 destY, u16 destWidth, u16 destHeight, u16 colorKey);
void Heap_Free(void *ptr);
u8 GetWindowWidth(void *window);
u8 GetWindowHeight(void *window);
u8 GetWindowX(void *window);
u8 GetWindowY(void *window);
u16 GetWindowBaseTile(void *window);
void CopyWindowPixelsToVram_TextMode(void *window);
u16 *BgCellManager_GetCellTilemap(void *, u32);
void BufferIntegerAsString(void *fmt, u32 field, u32 value, u32 digits, u32 mode, u32 charset);
void PokedexApp_PrintFormattedMessage(void *, void *, u32, u32, s32, u32, u32, u32, u32);
u16 ov18_021E7698(u32);

void PokedexApp_DrawSearchOrderPage(void *);
void PokedexApp_DrawSearchNamePage(void *);
void PokedexApp_DrawSearchTypePage(void *);
void PokedexApp_DrawSearchHeightPage(void *);
void PokedexApp_DrawSearchWeightPage(void *);
void PokedexApp_DrawSearchAreaPage(void *);
void PokedexApp_DrawSearchOrderValue(void *, u32);
void PokedexApp_DrawSearchNameValue(void *, u32);
void PokedexApp_DrawSearchTypeValue(void *, u32, u32, s32);
void PokedexApp_DrawHeightValue(void *, u32, u32);
void PokedexApp_DrawWeightValue(void *, u32, u32);
void PokedexApp_DrawSearchAreaValue(void *, u32);
void PokedexApp_DrawSearchCriteriaSummary(void *);
void ov18_021EFFEC(void *);
void PokedexApp_PutWindowTilemap(void *, u32);

extern const u8 sSearchTypeMsgIds[];
extern const u8 sSizeCheckWindowTemplates[];

void PokedexApp_DrawSearchButton(void *p, u32 index, u32 message);
void PokedexApp_DrawSearchMenuPage(void *p);
void PokedexApp_DrawSearchCriteria(void *p);
void PokedexApp_DrawSearchOrderPage(void *p);
void PokedexApp_DrawSearchNamePage(void *p);
void PokedexApp_DrawSearchTypePage(void *p);
void PokedexApp_DrawSearchHeightPage(void *p);
void PokedexApp_DrawSearchWeightPage(void *p);
void PokedexApp_DrawSearchAreaPage(void *p);
void PokedexApp_DrawSearchFormPage(void *p);
void PokedexApp_DrawSearchOrderValue(void *p, u32 index);
void PokedexApp_DrawSearchNameValue(void *p, u32 index);
void PokedexApp_DrawSearchTypeValue(void *p, u32 entry, u32 index, s32 x);
void PokedexApp_DrawHeightValue(void *p, u32 height, u32 index);
void PokedexApp_DrawWeightValue(void *p, u32 weight, u32 index);
void PokedexApp_DrawSearchAreaValue(void *p, u32 index);
void PokedexApp_DrawSearchCriteriaSummary(void *p);
void PokedexApp_PutWindowTilemap(void *p, u32 index);

void ov18_021EFFEC(void *p) {
    BgCellManager_GetCellTilemap(PTR(p, 8), 0xf);
    ov18_021EE44C(p, 0x5f, 0);
    ov18_021EE44C(p, 0x60, 1);
    PokedexApp_DrawCountNumber(p, U8_(p, 0x185d) + 0x61, U16(p, 0x102c));
    PokedexApp_DrawCountNumber(p, U8_(p, 0x185d) + 0x63, U16(p, 0x102e));
    PokedexApp_PutWindowTilemap(p, 0x5f);
    PokedexApp_PutWindowTilemap(p, 0x60);
    PokedexApp_PutWindowTilemap(p, U8_(p, 0x185d) + 0x61);
    PokedexApp_PutWindowTilemap(p, U8_(p, 0x185d) + 0x63);
    U8_(p, 0x185d) = 1 ^ U8_(p, 0x185d);
}
