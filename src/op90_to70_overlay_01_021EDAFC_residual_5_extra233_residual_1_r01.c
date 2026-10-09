#pragma force_active on
#include "field_move_tutor_menu_internal.h"
MoveTutorMenu *ov01_021EDC28(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 cancellable, u16 *ret_p, MessageFormat *msgFmt, Window *window, MsgData *msgData);
void ov01_021EE754(MoveTutorMenu *menu, u32 msgId, u32 x, u32 y);
void ov01_021EE7B8(SysTask *task, MoveTutorMenu *menu);
#pragma opt_propagation off
// @fn PrintCurFloorInNewWindow
void PrintCurFloorInNewWindow(FieldSystem *fieldSystem, u8 x, u8 y, u16 *p_ret, MessageFormat *msgFmt, u16 floor) {
    u32 tiles;
    u8 flag;
    MoveTutorMenu *menu;
    u32 width;
    u32 msg;

    menu = ov01_021EDC28(fieldSystem, x, y, 0, 0, p_ret, msgFmt, NULL, NULL);
    {
        u8 fontId = 0;
        width = GetFontAttribute(fontId, 0) * 8;
    }
    if (width % 8 == 0) {
        tiles = width / 8;
    } else {
        u32 roundedUp = 1 + width / 8;
        tiles = roundedUp;
    }
    AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window, 3, menu->x, menu->y, tiles, 4, 13, 221);
    LoadUserFrameGfx1(menu->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 985, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menu->window, TRUE, 985, 11);
    FillWindowPixelRect(&menu->window, 15, 0, 0, tiles * 8, 32);
    ov01_021EE754(menu, 16, 0, 0);
    msg = ov01_021EE934(fieldSystem->location->mapId, floor, &flag);
    ov01_021EE754(menu, (u16)msg, flag, 16);
    menu->menu2d.window = &menu->window;
    CopyWindowToVram(&menu->window);
    menu->task = SysTask_CreateOnMainQueue((SysTaskFunc)ov01_021EE7B8, menu, 0);
}
