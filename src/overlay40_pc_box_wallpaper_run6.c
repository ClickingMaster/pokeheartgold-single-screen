#include "overlay40_pc_box_wallpaper_private.h"

void ov40_02244A84(void *str, u32 heapId) {
    void *msgData;
    String_SetEmpty(str);
    msgData = NewMsgDataFromNarc(1, 0x1B, 0xD, heapId);
    ReadMsgDataIntoString(msgData, 0x14C, str);
    DestroyMsgData(msgData);
}

int ov40_02244AB0(void *save, void *a2, int a3, u16 *state, int a5) {
    u16 h;
    switch (*state) {
    case 0:
        if (gBattleRecord == NULL) {
            GF_AssertFail();
        }
        if (a3 == 1) {
            BYTE(gBattleRecord, 0xAB) = a3;
            HALF(gBattleRecord, 0xCC) = 0xE281;
            HALF(gBattleRecord, 0xE4) = SaveArray_CalcCRC16(save, (u8 *)gBattleRecord + 0x84, 0x58);
        }
        h = HALF(gBattleRecord, 0x1D4C);
        BattleRecord_Encrypt((u8 *)gBattleRecord + 0xE8, 0x1C64, h + (((u32)(h ^ 0xFFFF)) << 16));
        (*state)++;
        break;
    case 1:
        return BattleRecord_WriteSlot(save, gBattleRecord, a2, a5);
    }
    return 0;
}

void ov40_02244B3C(void) {
    u16 h;
    if (gBattleRecord == NULL) {
        GF_AssertFail();
    }
    h = HALF(gBattleRecord, 0x1D4C);
    BattleRecord_Encrypt((u8 *)gBattleRecord + 0xE8, 0x1C64, h + (((u32)(h ^ 0xFFFF)) << 16));
}

int ov40_02244B70(void *save, void *a, void *b, u16 *state, int a5) {
    void *p;
    switch (*state) {
    case 0:
        if (gBattleRecord == NULL) {
            GF_AssertFail();
        }
        p = gBattleRecord;
        PTR(p, 0xDC) = a;
        PTR(p, 0xE0) = b;
        (*state)++;
        break;
    case 1:
        return BattleRecord_WriteSlot(save, gBattleRecord, NULL, a5);
    }
    return 0;
}

int ov40_02244BBC(void *save, int a) {
    int r;
    u16 h;
    if (gBattleRecord == NULL) {
        GF_AssertFail();
    }
    BYTE(gBattleRecord, 0xAB) = 1;
    HALF(gBattleRecord, 0xCC) = 0xE281;
    HALF(gBattleRecord, 0xE4) = SaveArray_CalcCRC16(save, (u8 *)gBattleRecord + 0x84, 0x58);
    h = HALF(gBattleRecord, 0x1D4C);
    BattleRecord_Encrypt((u8 *)gBattleRecord + 0xE8, 0x1C64, h + (((u32)(h ^ 0xFFFF)) << 16));
    sub_0201A728(8);
    r = sub_02027134(save, gBattleRecord, a);
    if (r == 2) {
        r = SaveGameNormal(save);
    }
    sub_0201A738(8);
    return r;
}
