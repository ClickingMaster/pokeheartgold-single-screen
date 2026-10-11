#include "overlay70_menu_helpers_private.h"

void ov70_02242E58(Ov70R36 *work) {
    int i;
    int off;
    void *str;
    BgCellManager_LoadCellTilemap(work->listMenu, 0, 0x64, 0x1f, 1);
    BgCellManager_GetCellTilemap(work->listMenu, 0);
    ov70_0224190C(work, 1);
    i = 0;
    off = i;
    do {
        str = NewString_ReadMsgData(work->msgData, i + 0x7c);
        ov70_02242FC4(work->listMenu, work->windows + off, str, 2, 0xf0e02);
        String_Delete(str);
        i++;
        off += 0x10;
    } while (i < 3);
    ov70_02243EB8(work->listMenu, work->msgData, work->windows + 0xe0, 0x44);
    BgCellManager_DrawCellAt(work->listMenu, 0, 0x20, 0);
    BgCellManager_DrawCell(work->listMenu, 0);
    BgCellManager_StartCellMove(work->listMenu, 0, -4, 0, 4);
}

void ov70_02242EE4(Ov70R36 *work) {
    int cursor;
    BgCellManager_LoadCellTilemap(work->listMenu, 0, 0x64, 0x23, 1);
    BgCellManager_GetCellTilemap(work->listMenu, 0);
    ov70_0224190C(work, 2);
    if (work->mode == 1) {
        work->count = (int)ov70_0223F7E4((u8 *)work + 0x34, work->msgData, 1);
    } else if (work->mode == 0) {
        work->count = (int)ov70_0223F7E4((u8 *)work + 0x34, work->msgData, 0);
    }
    ov70_022434C0(work, 0, work->count);
    work->page = 0;
    ov70_02243F00(work->listMenu, PTR(work, 0x38), work->windows + 0x40, work->page, ov70_02242508(work->count, 4));
    ov70_02243EB8(work->listMenu, work->msgData, work->windows + 0xe0, 0x44);
    BgCellManager_DrawCellAt(work->listMenu, 0, 0x20, 0);
    Sprite_SetAnimCtrlSeq(work->cursorSprite, 0x31);
    work->cursor = 0;
    work->savedCursor = 0;
    cursor = *(volatile int *)&work->cursor;
    Gts_SetSpritePosition(work->cursorSprite, ov70_02245D76[cursor * 2], ov70_02245D77[cursor * 2]);
    BgCellManager_DrawCell(work->listMenu, 0);
    BgCellManager_StartCellMove(work->listMenu, 0, -4, 0, 4);
    work->state = 0x20;
}

void ov70_02242FC4(void *menu, void *window, void *str, int font, int color) {
    AddTextPrinterParameterizedWithColor(window, 0, str, font, 0, 0xff, color, 0);
    CopyWindowPixelsToVram_TextMode(window);
    BgCellManager_PutWindowTiles(menu, 0, window);
}
