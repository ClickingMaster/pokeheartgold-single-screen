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
void ov18_021EFFEC(void *p);
void PokedexApp_PutWindowTilemap(void *p, u32 index);

void PokedexApp_DrawSearchMenuPage(void *p) {
    u32 i;
    u8 *window;
    ov18_021E613C(p, 0);
    FillWindowPixelBuffer((u8 *)p + 0x14c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x15c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x16c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x17c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x18c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x19c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x1ac, 0);
    PokedexApp_DrawSearchOrderLabel(p, 0x14);
    PokedexApp_DrawSearchNameLabel(p, 0x15);
    PokedexApp_DrawSearchTypeLabel(p, 0x16);
    PokedexApp_DrawSearchHeightLabel(p, 0x17);
    PokedexApp_DrawSearchWeightLabel(p, 0x18);
    PokedexApp_DrawSearchAreaLabel(p, 0x19);
    PokedexApp_DrawSearchFormLabel(p, 0x1a);
    PokedexApp_DrawSearchButton(p, 0x11, 0x23);
    PokedexApp_DrawSearchButton(p, 0x12, 0x24);
    PokedexApp_DrawSearchButton(p, 0x13, 0x25);
    i = 0x14;
    window = (u8 *)p + 0x14c;
    for (; i <= 0x1a; i++) {
        ScheduleWindowCopyToVram(window);
        window += 0x10;
    }
}

void PokedexApp_DrawSearchCriteria(void *p) {
    u32 i;
    u8 *window;
    PokedexApp_DrawSearchOrderValue(p, 0x1b);
    PokedexApp_DrawSearchNameValue(p, 0x1c);
    PokedexApp_DrawSearchTypeValue(p, U32(p, 0x1870), 0x1d, 0x1d);
    PokedexApp_DrawSearchTypeValue(p, U32(p, 0x1874), 0x1e, 0x1d);
    PokedexApp_DrawHeightValue(p, ((Dims18 *)PTR(p, 0x1850))[U32(p, 0x1878)].height, 0x1f);
    PokedexApp_DrawHeightValue(p, ((Dims18 *)PTR(p, 0x1850))[U32(p, 0x187c)].height, 0x20);
    PokedexApp_DrawWeightValue(p, ((Dims18 *)PTR(p, 0x1850))[U32(p, 0x1880)].weight, 0x21);
    PokedexApp_DrawWeightValue(p, ((Dims18 *)PTR(p, 0x1850))[U32(p, 0x1884)].weight, 0x22);
    PokedexApp_DrawSearchAreaValue(p, 0x23);
    i = 0x1b;
    window = (u8 *)p + 0x1bc;
    for (; i <= 0x23; i++) {
        ScheduleWindowCopyToVram(window);
        window += 0x10;
    }
}

void PokedexApp_DrawSearchOrderPage(void *p) {
    u32 i;
    u8 *window;
    ov18_021E613C(p, 0);
    FillWindowPixelBuffer((u8 *)p + 0x24c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x26c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x27c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x28c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x29c, 0);
    FillWindowPixelBuffer((u8 *)p + 0x2ac, 0);
    FillWindowPixelBuffer((u8 *)p + 0x2bc, 0);
    PokedexApp_PrintMessage((u8 *)p + 0x24c, PTR(p, 0x65c), 0x1a, 0x2c, 0, 0, 0x20100, 2);
    PokedexApp_PrintMessage((u8 *)p + 0x26c, PTR(p, 0x65c), 0x29, 0x2c, 0, 4, 0x20100, 2);
    PokedexApp_PrintMessage((u8 *)p + 0x27c, PTR(p, 0x65c), 0x2a, 0x2c, 0, 4, 0x20100, 2);
    PokedexApp_PrintMessage((u8 *)p + 0x28c, PTR(p, 0x65c), 0x2b, 0x2c, 0, 4, 0x20100, 2);
    PokedexApp_PrintMessage((u8 *)p + 0x29c, PTR(p, 0x65c), 0x2c, 0x2c, 0, 4, 0x20100, 2);
    PokedexApp_PrintMessage((u8 *)p + 0x2ac, PTR(p, 0x65c), 0x2d, 0x2c, 0, 4, 0x20100, 2);
    PokedexApp_PrintMessage((u8 *)p + 0x2bc, PTR(p, 0x65c), 0x2e, 0x2c, 0, 4, 0x20100, 2);
    PokedexApp_DrawSearchButton(p, 0x11, 0x27);
    PokedexApp_DrawSearchButton(p, 0x13, 0x28);
    PokedexApp_DrawSearchOrderValue(p, 0x25);
    i = 0x24;
    window = (u8 *)p + 0x24c;
    for (; i <= 0x2b; i++) {
        ScheduleWindowCopyToVram(window);
        window += 0x10;
    }
}
