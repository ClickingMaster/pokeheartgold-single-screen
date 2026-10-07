#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct Sprite Sprite;
typedef struct TradeEntry {
    u16 unk0;
    u16 species;
    u8 unk4[8];
    u32 hasItem;
} TradeEntry;
typedef struct Cursor {
    u8 data[0x18];
} Cursor;
typedef struct TradeWork {
    u8 unk000[0x94];
    int selectedA;
    int selectedB;
    u8 unk09C[0x2A8];
    Sprite *cursorSprites[2];
    u8 unk34C[0x30];
    Sprite *sprites[3][12];
    u8 unk40C[0x38];
    Window windows[37];
    u8 unk694[8];
    TradeEntry entries[12];
    u8 unk75C[0x1AC4];
    int (*nextState)(struct TradeWork *);
    u8 unk2224[0xA4];
    u32 counter;
    u8 unk22CC[4];
    Cursor cursorA;
    Cursor cursorB;
} TradeWork;
extern int ov65_0221FF4C[][2];
extern int ov65_0221FF50[][2];
void Sprite_SetDrawFlag(Sprite *sprite, int flag);
void ov65_0221ED80(Cursor *cursor, int x, int y, int a3, int a4);
int ov65_0221EE18(TradeWork *work);
int ov65_0221EC34(TradeWork *work);

// @fn ov65_0221EC34
int ov65_0221EC34(TradeWork *work) {
    int i;
    for (i = 0; i < 6; i++) {
        if (i != work->selectedA) {
            Sprite_SetDrawFlag(work->sprites[0][i], FALSE);
            if (work->entries[i].species != 0) {
                Sprite_SetDrawFlag(work->sprites[1][i], FALSE);
            }
            if (work->entries[i].hasItem != 0) {
                Sprite_SetDrawFlag(work->sprites[2][i], FALSE);
            }
        }
        ClearWindowTilemapAndScheduleTransfer(&work->windows[i + 7]);
        if (i != work->selectedB - 6) {
            Sprite_SetDrawFlag(work->sprites[0][i + 6], FALSE);
            if (work->entries[i + 6].species != 0) {
                Sprite_SetDrawFlag(work->sprites[1][i + 6], FALSE);
            }
            if (work->entries[i + 6].hasItem != 0) {
                Sprite_SetDrawFlag(work->sprites[2][i + 6], FALSE);
            }
        }
        ClearWindowTilemapAndScheduleTransfer(&work->windows[i + 13]);
    }
    Sprite_SetDrawFlag(work->cursorSprites[0], FALSE);
    Sprite_SetDrawFlag(work->cursorSprites[1], FALSE);
    ov65_0221ED80(&work->cursorA, ov65_0221FF4C[work->selectedA][0] + 16, ov65_0221FF50[work->selectedA][0] - 6, 48, 32);
    ov65_0221ED80(&work->cursorB, ov65_0221FF4C[work->selectedB][0] + 16, ov65_0221FF50[work->selectedB][0] - 6, 176, 32);
    work->counter = 0;
    work->nextState = ov65_0221EE18;
    return 0;
}
