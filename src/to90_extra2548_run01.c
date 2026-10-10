#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
typedef struct Canvas Canvas;

typedef struct {
    void *a;
    u32 *b;
    u8 text[33];
    u8 count, flag;
} Text;

extern Text *ov13_0224F540;
extern u8 ov13_02245470[];
extern void ov13_02240D80(Canvas *, int);
extern void ov13_02240E2C(Canvas *);
extern void ov13_02240CC0(Canvas *, int, int, int, int, int, u32, const u16 *);
typedef char TextSize[(sizeof(Text) == 44) ? 1 : -1];
typedef char TextStorageOffset[(offsetof(Text, text) == 8) ? 1 : -1];

typedef struct {
    u16 x, y, width, height;
} Rect;

void ov13_022316B4(void);

void ov13_022316B4(void) {
    Rect rect = { 0 };
    u16 tile[2];
    int j, i;
    rect.y = 32;
    rect.width = 14;
    rect.height = 16;
    ov13_02240D80(ov13_0224F540->a, 0);
    tile[1] = 0;
    i = 0;
    j = 0;
    for (; i < 32; i++, j++) {
        u8 c;
        if (i == 16) {
            j = 0;
            rect.y = 49;
        }
        c = ov13_0224F540->text[i];
        if (c == 32) {
            tile[0] = 0xe01d;
        } else {
            tile[0] = c;
        }
        rect.x = ov13_02245470[j];
        ov13_02240CC0(ov13_0224F540->a, rect.x, rect.y, rect.width, rect.height, 2, 0x480, tile);
    }
    ov13_02240E2C(ov13_0224F540->a);
}
