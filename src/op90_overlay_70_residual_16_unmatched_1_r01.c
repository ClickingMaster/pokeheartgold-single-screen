#pragma force_active on
#include "global.h"
#include "msgdata.h"
#include "message_format.h"
#include "item.h"
#include "pokemon.h"
#include "bg_window.h"
typedef struct TradeInfo {
    s16 message;
    s8 type;
    s8 level;
} TradeInfo;
extern int ov70_02245910[];
extern u32 ov70_0224649C[];
void ov70_02245084(Window *window, String *string, int x, int y, int a4, u32 color);
void ov70_0223A578(MsgData *msgData, MsgData *msgData2, MessageFormat *format, Window *windows, BoxPokemon *boxMon, TradeInfo *info);

// @fn ov70_0223A578
void ov70_0223A578(MsgData *msgData, MsgData *msgData2, MessageFormat *format, Window *windows, BoxPokemon *boxMon, TradeInfo *info) {
    String *messageStr;
    String *typeStr;
    String *label;
    String *levelStr;
    String *title;
    String *footer;
    String *nickname;
    String *itemName;
    int type;
    int message;
    int level;
    int item;
    int i;

    nickname = String_New(22, HEAP_ID_61);
    itemName = String_New(18, HEAP_ID_61);
    GetBoxMonData(boxMon, MON_DATA_NICKNAME_STRING, nickname);
    message = info->message;
    type = info->type;
    level = info->level;
    item = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM, NULL);
    title = NewString_ReadMsgData(msgData, 73);
    typeStr = NewString_ReadMsgData(msgData, ov70_02245910[type]);
    label = NewString_ReadMsgData(msgData, 106);
    BufferIntegerAsString(format, 3, level, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    levelStr = ReadMsgData_ExpandPlaceholders(format, msgData, 107, HEAP_ID_61);
    messageStr = NewString_ReadMsgData(msgData2, message);
    GetItemNameIntoString(itemName, item, HEAP_ID_61);
    footer = NewString_ReadMsgData(msgData, 59);
    for (i = 0; i < 6; i++) {
        FillWindowPixelBuffer(&windows[i], 0);
    }
    ov70_02245084(&windows[0], nickname, 0, 0, 0, 0x10200);
    if (type != 3) {
        ov70_02245084(&windows[0], typeStr, 64, 0, 0, ov70_0224649C[type]);
    }
    ov70_02245084(&windows[1], messageStr, 0, 0, 0, 0x10200);
    ov70_02245084(&windows[2], label, 0, 0, 0, 0x10200);
    ov70_02245084(&windows[3], levelStr, 0, 0, 0, 0x10200);
    ov70_02245084(&windows[4], title, 0, 0, 0, 0xF0200);
    ov70_02245084(&windows[5], itemName, 0, 0, 0, 0x10200);
    ov70_02245084(&windows[6], footer, 0, 0, 0, 0xF0200);
    String_Delete(title);
    String_Delete(itemName);
    String_Delete(label);
    String_Delete(levelStr);
    String_Delete(typeStr);
    String_Delete(nickname);
    String_Delete(messageStr);
    String_Delete(footer);
}
