#pragma force_active on
#include "global.h"
#include "player_data.h"
#include "string_util.h"

typedef struct RecordData {
    u8 unk00;
    u8 unk01;
    u8 unk02[2];
    u32 unk04[11];
} RecordData;

typedef struct Record {
    u32 trainerId;
    u8 gender;
    u8 version;
    u8 language;
    u8 pad;
    u16 name[8];
    RecordData data;
} Record;

extern u8 *ov73_021EA664[];
BOOL ov73_021E7A8C(int idx, int val);
void ov73_021E7AC0(SaveData *save, Record *out, RecordData *in, int count);

// @fn ov73_021E7A8C
BOOL ov73_021E7A8C(int idx, int val) {
    int i;
    for (i = 0; ov73_021EA664[idx][i] != 0xFF; i++) {
        if (val == ov73_021EA664[idx][i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// @fn ov73_021E7AC0
void ov73_021E7AC0(SaveData *save, Record *out, RecordData *in, int count) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(save);
    int i;
    for (i = 0; i < count; i++, in++, out++) {
        MI_CpuClearFast(out, sizeof(Record));
        out->trainerId = PlayerProfile_GetTrainerID(profile);
        out->gender = PlayerProfile_GetTrainerGender(profile);
        out->version = GAME_VERSION;
        out->language = 2;
        CopyU16StringArrayN(out->name, PlayerProfile_GetNamePtr(profile), 8);
        out->data = *in;
        out->data.unk01 = 0;
        out->data.unk00 = 0;
    }
}
