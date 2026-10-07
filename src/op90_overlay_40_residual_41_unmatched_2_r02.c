#pragma force_active on
#include "global.h"
#include "sprite_system.h"

typedef struct Ov40Rect {
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} Ov40Rect;

typedef struct Ov40SpriteEntry {
    ManagedSprite *sprite;
    u8 unk4[0x18];
} Ov40SpriteEntry;

typedef struct Ov40Unk {
    u8 unk0[0x194];
    Ov40SpriteEntry entries[1];
    ManagedSprite *cursor;
    u8 unk1B4[0x84];
    Ov40Rect rects[1];
} Ov40Unk;

void ov40_02243EEC(Ov40Unk *data, int index);
void ov40_02243F38(Ov40Unk *data, int index, int entry);

// @fn ov40_02243F38
void ov40_02243F38(Ov40Unk *data, int index, int entry) {
    int x = (s16)((data->rects[index + 12].left + data->rects[index + 12].right) / 2);
    int y = (s16)((data->rects[index + 12].top + data->rects[index + 12].bottom) / 2);
    ManagedSprite_SetPositionXY(data->entries[entry].sprite, x, y - 8);
}
