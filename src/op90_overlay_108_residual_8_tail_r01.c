#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "yes_no_prompt.h"
typedef struct State {
    u32 heap;
    u8 pad[0x340 - 4];
    BgConfig *bg;
    u8 pad2[0x3b4 - 0x344];
    Window windows[15];
    Window text;
    void *menuItems[2];
    u8 pad4bc[4];
    YesNoPrompt *prompt;
} State;
extern const WindowTemplate ov108_021EA7A8[5];
extern const u32 ov108_021EA724[2];
void ov108_021E7080(State *s);
// @fn ov108_021E7080
void ov108_021E7080(State *s) {
    int i;
    u8 y;
    int index;
    u8 height;
    int base;
    for (i = 0; i <= 4; i++) {
        AddWindow(s->bg, &s->windows[i], &ov108_021EA7A8[i]);
        FillWindowPixelBuffer(&s->windows[i], 0);
    }
    base = 0x3a6;
    for (i = 0; i < 5; i++) {
        index = i * 2 + 5;
        y = i * 4 + 1;
        height = (u8)(1 ^ (i / 4)) * 2 + 2;
        base -= 22 * height;
        AddWindowParameterized(s->bg, &s->windows[index], 7, 1, y, 22, height, 5, base);
        FillWindowPixelBuffer(&s->windows[index], (ov108_021EA724[i % 2] & 0xFF));
        base -= 22 * height;
        AddWindowParameterized(s->bg, &s->windows[index + 1], 7, 26, y + (u8)(1 ^ (i / 4)), 5, 2, 5, base);
        FillWindowPixelBuffer(&s->windows[index + 1], (ov108_021EA724[i % 2] & 0xFF));
        base -= 10;
    }
    InitWindow(&s->text);
    AddTextWindowTopLeftCorner(s->bg, &s->text, 6, 2, 0x2e1, 0);
    FillWindowPixelBufferText_AssumeTileSize32(&s->text, 0);
    s->prompt = YesNoPrompt_Create((enum HeapID)s->heap);
}
