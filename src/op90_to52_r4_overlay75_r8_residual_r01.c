#include "global.h"
#include "bg_window.h"
#include "pm_string.h"
#include "msgdata.h"
#include "text.h"
struct ListMenuCursor;
void ListMenuUpdateCursorObj(struct ListMenuCursor *cursor, Window *window, int x, int y);
#pragma force_active on
typedef struct Context {u32 pad;BgConfig *bg;u8 p[0x34-8];MsgData *msg;u8 p2[0x48-0x38];Window old;u8 p3[0xa8-0x58];int state,scroll,selection,total,result,frame,blink;Window first,second;struct ListMenuCursor *cursor;} Context;
typedef struct Position {u32 x,y;} Position;
typedef struct Offset {u32 y,nextX;} Offset;
Position _02249B20[2] = { { 4, 8 }, { 128, 8 } };
void ov75_02247C70(Context*);
// @fn ov75_02247C70
void ov75_02247C70(Context *owner) {
    String *text;
    FillWindowPixelBuffer(&owner->second, 15);
    text = NewString_ReadMsgData(owner->msg, 42);
    AddTextPrinterParameterized(&owner->second, 0, text, _02249B20[0].x + 12, _02249B20[0].y, 255, NULL);
    String_Delete(text);
    text = NewString_ReadMsgData(owner->msg, 43);
    AddTextPrinterParameterized(&owner->second, 0, text, _02249B20[1].x + 12, _02249B20[1].y, 255, NULL);
    String_Delete(text);
    ListMenuUpdateCursorObj(owner->cursor, &owner->second, _02249B20[owner->selection].x, _02249B20[owner->selection].y);
    CopyWindowToVram(&owner->second);
}
