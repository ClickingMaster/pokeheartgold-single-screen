#include "global.h"

#include "application/vs_recorder/vs_recorder_screens.h"
#include "msgdata/msg.naix"

#include "assert.h"
#include "msgdata.h"
#include "pm_string.h"
#include "save.h"
#include "save_arrays.h"
#include "system.h"

// gBattleRecord->header (0x84): type at 0x26, sealed flag at 0x27, magic at 0x48, video number at 0x58,
// CRC of the first 0x58 bytes at 0x60. gBattleRecord->data (0xE8) is encrypted with its CRC (at 0x1C64) as seed.
#define RECORD_HEADER_SEALED  0x27
#define RECORD_HEADER_MAGIC   0x48
#define RECORD_HEADER_NUMBER  0x58
#define RECORD_HEADER_CRC     0x60
#define RECORD_MAGIC          0xE281
#define RECORD_HEADER_CRC_LEN 0x58
#define RECORD_DATA_CRC       0x1C64

// msg_0013 #332, used for names the font can't show
void VsRecorder_LoadPlaceholderTrainerName(String *str, enum HeapID heapID) {
    MsgData *msgData;
    String_SetEmpty(str);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0013_bin, heapID);
    ReadMsgDataIntoString(msgData, 332, str);
    DestroyMsgData(msgData);
}

// writes the current recording into a save slot over several frames; sealed: the header gets the magic and its CRC
u32 VsRecorder_WriteRecordAsync(SaveData *saveData, u32 index, u32 sealed, u16 *state, void *writeState) {
    u16 crc;
    switch (*state) {
    case 0:
        if (gBattleRecord == NULL) {
            GF_AssertFail();
        }
        if (sealed == TRUE) {
            gBattleRecord->header[RECORD_HEADER_SEALED] = sealed;
            *(u16 *)(gBattleRecord->header + RECORD_HEADER_MAGIC) = RECORD_MAGIC;
            *(u16 *)(gBattleRecord->header + RECORD_HEADER_CRC) = SaveArray_CalcCRC16(saveData, gBattleRecord->header, RECORD_HEADER_CRC_LEN);
        }
        crc = *(u16 *)(gBattleRecord->data + RECORD_DATA_CRC);
        BattleRecord_Encrypt(gBattleRecord->data, RECORD_DATA_CRC, crc + (((u32)(crc ^ 0xFFFF)) << 16));
        (*state)++;
        break;
    case 1:
        return BattleRecord_WriteSlot(saveData, gBattleRecord, index, writeState);
    }
    return 0;
}

// before the recording is uploaded
void VsRecorder_EncryptRecord(SaveData *saveData) {
    u16 crc;
    if (gBattleRecord == NULL) {
        GF_AssertFail();
    }
    crc = *(u16 *)(gBattleRecord->data + RECORD_DATA_CRC);
    BattleRecord_Encrypt(gBattleRecord->data, RECORD_DATA_CRC, crc + (((u32)(crc ^ 0xFFFF)) << 16));
}

// after the upload: stores the server's video number in the recording and saves it into the first slot
int VsRecorder_SaveRecordNumber(SaveData *saveData, u32 numberLow, u32 numberHigh, u16 *state, u16 *writeState) {
    BattleRecord *record;
    switch (*state) {
    case 0:
        if (gBattleRecord == NULL) {
            GF_AssertFail();
        }
        record = gBattleRecord;
        *(u32 *)(record->header + RECORD_HEADER_NUMBER) = numberLow;
        *(u32 *)(record->header + RECORD_HEADER_NUMBER + 4) = numberHigh;
        (*state)++;
        break;
    case 1:
        return BattleRecord_WriteSlot(saveData, gBattleRecord, 0, writeState);
    }
    return 0;
}

// seals the current recording and saves it into a slot at once
int VsRecorder_WriteRecord(SaveData *saveData, int index) {
    int result;
    u16 crc;
    if (gBattleRecord == NULL) {
        GF_AssertFail();
    }
    gBattleRecord->header[RECORD_HEADER_SEALED] = TRUE;
    *(u16 *)(gBattleRecord->header + RECORD_HEADER_MAGIC) = RECORD_MAGIC;
    *(u16 *)(gBattleRecord->header + RECORD_HEADER_CRC) = SaveArray_CalcCRC16(saveData, gBattleRecord->header, RECORD_HEADER_CRC_LEN);
    crc = *(u16 *)(gBattleRecord->data + RECORD_DATA_CRC);
    BattleRecord_Encrypt(gBattleRecord->data, RECORD_DATA_CRC, crc + (((u32)(crc ^ 0xFFFF)) << 16));
    Sys_SetSoftResetDisableFlag(8);
    result = sub_02027134(saveData, gBattleRecord, index);
    if (result == 2) {
        result = SaveGameNormal(saveData);
    }
    Sys_ClearSoftResetDisableFlag(8);
    return result;
}
