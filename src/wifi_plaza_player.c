#include "global.h"

#include "font.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sound_system.h"
#include "wifi_plaza_internal.h"

void ov45_0222A4D0(WiFiPlazaWork *work) {
    if (work->unk_1fc.flag4 == 1 && work->unk_1fc.unk_0a <= 0) {
        Sound_SetSceneAndPlayBGM(0x15, 0x481, 0);
        ov45_0222BC84(&work->unk_1fc);
        return;
    }
    Sound_SetSceneAndPlayBGM(0x15, 0x47f, 0);
    ov45_0222BC84(&work->unk_1fc);
}

void ov45_0222A520(WiFiPlazaWork *work, int value) {
    if (value == work->unk_1fc.unk_0d) {
        return;
    }
    work->unk_1fc.unk_0d = value;
    ov45_0222BC84(&work->unk_1fc);
}

int ov45_0222A53C(void *unused) {
    return ov45_0222EC68((s32)ov45_0222E9E0());
}

void ov45_0222A548(void) {
    ov45_0222E9E0();
}

BOOL ov45_0222A550(void *unused, int index) {
    WiFiPlazaPlayerList list;
    ov45_0222EC10(&list);
    return list.ids[index] != -1;
}

WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *work, u32 index) {
    WiFiPlazaPlayerList list;
    int id;
    if (index >= 20) {
        GF_AssertFail();
    }
    ov45_0222EC10(&list);
    id = list.ids[index];
    if (id == -1) {
        return NULL;
    }
    if (id == (s32)ov45_0222E9E0()) {
        return &work->profile.player;
    }
    return ov45_0222EA2C(id);
}

WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *work) {
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->unk_52c = 1;
    }
    return &work->profile.player;
}

void ov45_0222A5E8(WiFiPlazaWork *work, int value) {
    if (value >= 15) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->unk_52c = 1;
        return;
    }
    if (work->profile.player.unk_43 == value) {
        return;
    }
    if (value != 9 && ov45_0222AFF8(work) == 1) {
        if (ov45_0222B00C(work) == 0) {
            ov45_0222EC90(work->unk_1c0.unk_04);
            ov45_0222BE28(work, work->unk_1c0.unk_04);
        }
        ov45_0222BD5C(&work->unk_1c0);
    }
    switch (work->profile.player.unk_43) {
    case 2:
        ov45_0222CB74(&work->history, 0x10, -1);
        break;
    case 3:
        ov45_0222CB74(&work->history, 0x11, -1);
        break;
    case 4:
        ov45_0222CB74(&work->history, 0x12, -1);
        break;
    case 5:
        ov45_0222CB74(&work->history, 0x13, -1);
        break;
    case 6:
        ov45_0222CB74(&work->history, 0x14, -1);
        break;
    case 7:
        ov45_0222CB74(&work->history, 0x15, -1);
        break;
    case 8:
        ov45_0222CB74(&work->history, 0x16, -1);
        break;
    }
    work->profile.player.unk_43 = value;
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
    ov45_0222BA3C(work);
}

void ov45_0222A704(WiFiPlazaWork *work, int index, int value) {
    if (index >= 24) {
        GF_AssertFail();
    }
    ov45_0222CB74(&work->history, index, value);
    ov45_0222BA3C(work);
}

void ov45_0222A72C(WiFiPlazaWork *work, u8 value) {
    if (value >= 27) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->unk_52c = 1;
        return;
    }
    work->profile.player.unk_41 = value;
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
    ov45_0222BA3C(work);
}

void ov45_0222A770(WiFiPlazaWork *work, int a, int b) {
    if (a >= 18) {
        GF_AssertFail();
    }
    if (b >= 18) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->unk_52c = 1;
        return;
    }
    if (a < 18 && b < 18) {
        if (a == 0) {
            work->profile.player.unk_88[0] = b;
            work->profile.player.unk_88[1] = 0;
        } else {
            work->profile.player.unk_88[0] = a;
            work->profile.player.unk_88[1] = b;
        }
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        ov45_0222BA3C(work);
    }
}

void ov45_0222A7DC(WiFiPlazaWork *work, u32 a, u32 b) {
    if (b >= 3) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->unk_52c = 1;
        return;
    }
    if (b < 3) {
        work->profile.player.unk_8c.unk_00 = a;
        work->profile.player.unk_8c.unk_04 = b;
        work->unk_1fc.low = 1;
        ov45_0222B118(work, 6);
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        ov45_0222BA3C(work);
    }
}

void WiFiPlazaPlayer_CopyToProfile(WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID) {
    BOOL invalid;
    int ok = ov45_0222B28C(player->name, 8, heapID);
    String *b;
    String *a;
    invalid = FALSE;
    if (ok == 1) {
        Save_Profile_PlayerName_Set(profile, player->name);
        a = String_New(0x20, heapID);
        b = String_New(0x20, heapID);
        PlayerName_FlatToString(profile, b);
        if (!FontID_String_AllCharsValid((FontID)0, b, a)) {
            invalid = TRUE;
        }
        String_Delete(a);
        String_Delete(b);
    } else {
        invalid = TRUE;
    }
    if (invalid) {
        MsgData *msg = NewMsgDataFromNarc((MsgDataLoadType)1, (NarcId)0x1b, 0x309, heapID);
        String *name = NewString_ReadMsgData(msg, 0x40);
        PlayerName_StringToFlat(profile, name);
        String_Delete(name);
        DestroyMsgData(msg);
    }
    PlayerProfile_SetTrainerID(profile, WiFiPlazaPlayer_GetTrainerID(player));
    PlayerProfile_SetTrainerGender(profile, WiFiPlazaPlayer_GetGender(player));
    PlayerProfile_SetAvatar(profile, WiFiPlazaPlayer_GetAvatar(player));
    PlayerProfile_SetLanguage(profile, WiFiPlazaPlayer_GetLanguage(player));
    PlayerProfile_SetGameClearFlag(profile);
}

int ov45_0222A920(WiFiPlazaPlayer *player) {
    u8 v = player->unk_43;
    return v < 14 ? v : 14;
}

#pragma force_active on
#pragma opt_common_subs off

u8 ov45_0222A92C(WiFiPlazaPlayer *player, u32 index) {
    int count;
    int i;

    GF_ASSERT(index < 12);
    count = 0;
    for (i = 0; i < 12; i++) {
        if (player->slots[i] < 24) {
            count++;
        }
        if (count - 1 == index) {
            return player->slots[i];
        }
    }
    return 24;
}

#pragma opt_common_subs reset
#pragma force_active reset

void *ov45_0222A964(WiFiPlazaPlayer *player, u32 index) {
    int seen, i;
    if (index >= 12) {
        GF_AssertFail();
    }
    seen = 0;
    for (i = 0; i < 12; i++) {
        u8 v = player->slots[i];
        if (v < 24) {
            seen++;
        }
        if (seen - 1 == index) {
            return player->slotData[i];
        }
    }
    return (void *)-1;
}

u32 ov45_0222A99C(WiFiPlazaPlayer *player) {
    return player->unk_00;
}

u32 WiFiPlazaPlayer_GetTrainerID(WiFiPlazaPlayer *player) {
    return player->trainerId;
}

int ov45_0222A9A4(WiFiPlazaPlayer *player) {
    u16 v = WiFiPlazaPlayer_GetAvatar(player);
    int i;
    for (i = 0; i < 16; i++) {
        if (v == ov45_02254A84[i].unk_00) {
            return i;
        }
    }
    return 24;
}

void *ov45_0222A9C8(WiFiPlazaPlayer *player) {
    return player->unk_1c;
}

u32 WiFiPlazaPlayer_GetGender(WiFiPlazaPlayer *player) {
    u32 result;
    u32 i;

    if (player->gender >= 2) {
        if (ov45_0222CD04(player->unk_3a) != 0xFFFF) {
            for (i = 0; i < 16; i++) {
                if (ov45_02254A84[i].unk_00 == player->unk_3a) {
                    result = ov45_02254A84[i].unk_00;
                }
            }
        } else {
            result = 1;
        }
    } else {
        result = player->gender;
    }
    return result;
}

u32 WiFiPlazaPlayer_GetLanguage(WiFiPlazaPlayer *player) {
    if (WiFiPlazaPlayer_IsLanguageValid(player) == TRUE) {
        return player->language;
    }
    return 2;
}

BOOL WiFiPlazaPlayer_IsLanguageValid(WiFiPlazaPlayer *player) {
    switch (player->language) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        return TRUE;
    default:
        return FALSE;
    }
}

u8 ov45_0222AA54(WiFiPlazaPlayer *player) {
    return player->language;
}

u16 WiFiPlazaPlayer_GetAvatar(WiFiPlazaPlayer *player) {
    u16 avatar = ov45_0222CD04(player->unk_3a);
    if (avatar == 0xFFFF) {
        if (WiFiPlazaPlayer_GetGender(player) == 0) {
            return 3;
        }
        return 6;
    }
    return avatar;
}

#pragma force_active on

u16 WiFiPlazaPlayer_GetCountry(WiFiPlazaPlayer *player) {
    if (player->country >= 234) {
        return 0;
    }
    if (ov45_0222D79C(player->country, player->region) == 0) {
        return 0;
    }
    return player->country;
}

u8 WiFiPlazaPlayer_GetRegion(WiFiPlazaPlayer *player) {
    if (ov45_0222D79C(player->country, player->region) == 0) {
        return 0;
    }
    return player->region;
}

#pragma force_active reset

u8 ov45_0222AAC8(WiFiPlazaPlayer *player) {
    u8 v = player->unk_41;
    if (v == 0xFF) {
        return 0;
    }
    if (v >= 27) {
        return 0;
    }
    return v;
}

BOOL ov45_0222AADC(WiFiPlazaPlayer *player) {
    return player->unk_41 == 0xFF ? FALSE : TRUE;
}

#pragma force_active on

u16 ov45_0222AAEC(WiFiPlazaPlayer *player, u32 index) {
    u16 value;
    if (index >= 2) {
        GF_AssertFail();
    }
    value = player->unk_88[index];
    if (value >= 18) {
        value = 0;
    }
    return value;
}

void ov45_0222AB0C(WiFiPlazaPlayer *player, WiFiPlazaPlayerUnk8C *out) {
    *out = player->unk_8c;
}

#pragma force_active reset
