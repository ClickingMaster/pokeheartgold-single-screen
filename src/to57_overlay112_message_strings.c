#include "global.h"

#include "msgdata.h"
#include "pm_string.h"

typedef struct Ov112Work42 {
    u8 pad_00000[0x1E44C];
    MsgData *msgData;
    u8 pad_1E450[8];
    String *str_1E458;
    String *str_1E45C;
    String *str_1E460;
    String *str_1E464;
    String *str_1E468;
    String *str_1E46C;
    String *str_1E470;
    String *str_1E474;
    u8 pad_1E478[0x1C];
    String *str_1E494;
    String *str_1E498;
    String *str_1E49C;
    String *str_1E4A0;
    String *str_1E4A4;
    String *str_1E4A8;
    String *str_1E4AC;
    String *str_1E4B0;
    String *str_1E4B4;
    u8 pad_1E4B8[4];
    String *str_1E4BC;
    u8 pad_1E4C0[0x4C];
    String *str_1E50C;
    String *str_1E510;
    String *str_1E514;
    String *str_1E518;
} Ov112Work42;

void ov112_021E9C98(Ov112Work42 *p);
void ov112_021E9E30(Ov112Work42 *p);

void ov112_021E9C98(Ov112Work42 *p) {
    p->str_1E458 = NewString_ReadMsgData(p->msgData, 0x4C);
    p->str_1E45C = NewString_ReadMsgData(p->msgData, 0x49);
    p->str_1E460 = NewString_ReadMsgData(p->msgData, 0x4A);
    p->str_1E464 = NewString_ReadMsgData(p->msgData, 0x4B);
    p->str_1E468 = NewString_ReadMsgData(p->msgData, 0x4E);
    p->str_1E46C = NewString_ReadMsgData(p->msgData, 0x4D);
    p->str_1E470 = NewString_ReadMsgData(p->msgData, 0x50);
    p->str_1E474 = NewString_ReadMsgData(p->msgData, 0x4F);
    p->str_1E494 = NewString_ReadMsgData(p->msgData, 0x3F);
    p->str_1E498 = NewString_ReadMsgData(p->msgData, 0x40);
    p->str_1E49C = NewString_ReadMsgData(p->msgData, 0x41);
    p->str_1E4A0 = NewString_ReadMsgData(p->msgData, 0x42);
    p->str_1E4A4 = NewString_ReadMsgData(p->msgData, 0x47);
    p->str_1E4A8 = NewString_ReadMsgData(p->msgData, 0x3F);
    p->str_1E4AC = NewString_ReadMsgData(p->msgData, 0x47);
    p->str_1E4B0 = NewString_ReadMsgData(p->msgData, 0x4F);
    p->str_1E4B4 = NewString_ReadMsgData(p->msgData, 0x3B);
    p->str_1E4BC = NewString_ReadMsgData(p->msgData, 0x3D);
    p->str_1E50C = NewString_ReadMsgData(p->msgData, 0x52);
    p->str_1E510 = NewString_ReadMsgData(p->msgData, 0x55);
    p->str_1E514 = NewString_ReadMsgData(p->msgData, 0x94);
    p->str_1E518 = NewString_ReadMsgData(p->msgData, 0x95);
}

void ov112_021E9E30(Ov112Work42 *p) {
    String_Delete(p->str_1E458);
    String_Delete(p->str_1E45C);
    String_Delete(p->str_1E460);
    String_Delete(p->str_1E464);
    String_Delete(p->str_1E468);
    String_Delete(p->str_1E46C);
    String_Delete(p->str_1E470);
    String_Delete(p->str_1E474);
    String_Delete(p->str_1E494);
    String_Delete(p->str_1E49C);
    String_Delete(p->str_1E498);
    String_Delete(p->str_1E4A0);
    String_Delete(p->str_1E4A4);
    String_Delete(p->str_1E4A8);
    String_Delete(p->str_1E4AC);
    String_Delete(p->str_1E4B0);
    String_Delete(p->str_1E4B4);
    String_Delete(p->str_1E4BC);
    String_Delete(p->str_1E50C);
    String_Delete(p->str_1E510);
    String_Delete(p->str_1E514);
    String_Delete(p->str_1E518);
}
