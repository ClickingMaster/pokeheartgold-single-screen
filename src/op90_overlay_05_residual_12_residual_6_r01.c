#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"
#include "msgdata.h"
#include "message_format.h"
#include "bg_window.h"
#include "text.h"
typedef struct BattleParties {
    void *unk00;
    Party *playerParty;
    void *unk08;
    Party *enemyParty;
} BattleParties;
typedef struct MonStatus {
    u8 status;
    u8 fainted;
    u8 unk02[0x16];
} MonStatus;
typedef struct E624Work {
    BattleParties *parties;
    u8 unk04[0xC];
    Window windows[6][4];
    u8 unk190[0x91];
    MonStatus statuses[6];
} E624Work;
void ov05_0221E624(E624Work *work, MsgData *msgData, void *unused, MessageFormat *format, String *string, u32 index);

// @fn ov05_0221E624
void ov05_0221E624(E624Work *work, MsgData *msgData, void *unused, MessageFormat *format, String *string, u32 index) {
    u32 offset;
    u8 *windows = (u8 *)work->windows;
    offset = index * sizeof(work->windows[0]);
    Pokemon *mon;
    String *template;
    if (index < 3) {
        mon = Party_GetMonByIndex(work->parties->playerParty, index);
    } else {
        mon = Party_GetMonByIndex(work->parties->enemyParty, index - 3);
    }
    template = NewString_ReadMsgData(msgData, index + 8);
    BufferBoxMonNickname(format, 0, Mon_GetBoxMon(mon));
    StringExpandPlaceholders(format, string, template);
    String_Delete(template);
    AddTextPrinterParameterizedWithColor((Window *)(windows + offset), 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    if (work->statuses[index].fainted == 0) {
        if (work->statuses[index].status == 0) {
            ReadMsgDataIntoString(msgData, 27, string);
            AddTextPrinterParameterizedWithColor((Window *)(windows + offset), 0, string, 64, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
        } else if (work->statuses[index].status == 1) {
            ReadMsgDataIntoString(msgData, 28, string);
            AddTextPrinterParameterizedWithColor((Window *)(windows + offset), 0, string, 64, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
        }
    }
}
