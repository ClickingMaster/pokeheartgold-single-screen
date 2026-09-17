#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "text.h"

typedef struct Ov102Work30 {
    BgConfig *bgConfig;
    u8 pad_004[4];
    void *menu;
} Ov102Work30;

u32 ov102_021EC4A4(void);
void ov102_021EC4A8(u32 word, String *dest);
void ov102_021EC4CC(u32 word, u32 *x, u32 *y);
u32 sub_02091C74(void *menu, u32 group);
u32 ov102_021EB880(Ov102Work30 *work, BgConfig *bgConfig, u32 tile);

u32 ov102_021EB880(Ov102Work30 *work, BgConfig *bgConfig, u32 tile) {
    Window window;
    String *string;
    u32 count;
    u32 i;

    GF_ASSERT(tile < 0x338);
    AddWindowParameterized(bgConfig, &window, 1, 0x22, 10, 0x19, 8, 11, (u16)tile);
    string = String_New(4, HEAP_ID_35);
    FillWindowPixelBuffer(&window, 15);
    count = ov102_021EC4A4();
    for (i = 0; i < count; i++) {
        u32 x;
        u32 y;
        u32 colors;
        ov102_021EC4A8(i, string);
        ov102_021EC4CC(i, &x, &y);
        colors = sub_02091C74(work->menu, i) != 0 ? 0x1020F : 0x3040F;
        AddTextPrinterParameterizedWithColor(&window, 0, string, x + 3, y, 0xFF, colors, NULL);
    }
    CopyWindowPixelsToVram_TextMode(&window);
    PutWindowTilemap(&window);
    String_Delete(string);
    RemoveWindow(&window);
    return tile + 0xC8;
}
