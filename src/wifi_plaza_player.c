#include "global.h"

#include "font.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sound_system.h"
#include "wifi_plaza_internal.h"

void WiFiPlazaWork_PlayBGM(WiFiPlazaWork *work) {
    if (work->eventSchedule.flag4 == 1 && work->eventSchedule.paradeDelay <= 0) {
        Sound_SetSceneAndPlayBGM(0x15, 0x481, 0);
        WiFiPlazaEventSchedule_ApplyVolume(&work->eventSchedule);
        return;
    }
    Sound_SetSceneAndPlayBGM(0x15, 0x47f, 0);
    WiFiPlazaEventSchedule_ApplyVolume(&work->eventSchedule);
}

void WiFiPlazaWork_SetLowVolume(WiFiPlazaWork *work, int value) {
    if (value == work->eventSchedule.lowVolume) {
        return;
    }
    work->eventSchedule.lowVolume = value;
    WiFiPlazaEventSchedule_ApplyVolume(&work->eventSchedule);
}

int WiFiPlazaWork_GetOwnSlot(void *unused) {
    return WiFiPlazaLobbyClient_IdToSlot(WiFiPlazaLobbyClient_GetOwnId());
}

void WiFiPlazaWork_GetOwnId(void) {
    WiFiPlazaLobbyClient_GetOwnId();
}

BOOL WiFiPlazaWork_IsSlotOccupied(void *unused, int index) {
    WiFiPlazaPlayerList list;
    WiFiPlazaLobbyClient_GetPlayerList(&list);
    return list.ids[index] != -1;
}

WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(WiFiPlazaWork *work, u32 index) {
    WiFiPlazaPlayerList list;
    int id;
    if (index >= 20) {
        GF_AssertFail();
    }
    WiFiPlazaLobbyClient_GetPlayerList(&list);
    id = list.ids[index];
    if (id == -1) {
        return NULL;
    }
    if (id == WiFiPlazaLobbyClient_GetOwnId()) {
        return &work->profile.player;
    }
    return WiFiPlazaLobbyClient_GetPlayer(id);
}

WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(WiFiPlazaWork *work) {
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
    }
    return &work->profile.player;
}

void WiFiPlazaWork_SetActivity(WiFiPlazaWork *work, int value) {
    if (value >= 15) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
        return;
    }
    if (work->profile.player.unk_43 == value) {
        return;
    }
    if (value != 9 && WiFiPlazaWork_IsTalking(work) == 1) {
        if (WiFiPlazaWork_WasAskedToTalk(work) == 0) {
            WiFiPlazaLobbyClient_SlotToId(work->talk.partnerSlot);
            WiFiPlazaWork_SendTalkEnd(work, work->talk.partnerSlot);
        }
        WiFiPlazaTalkSession_Abort(&work->talk);
    }
    switch (work->profile.player.unk_43) {
    case 2:
        WiFiPlazaHistory_Add(&work->history, 0x10, -1);
        break;
    case 3:
        WiFiPlazaHistory_Add(&work->history, 0x11, -1);
        break;
    case 4:
        WiFiPlazaHistory_Add(&work->history, 0x12, -1);
        break;
    case 5:
        WiFiPlazaHistory_Add(&work->history, 0x13, -1);
        break;
    case 6:
        WiFiPlazaHistory_Add(&work->history, 0x14, -1);
        break;
    case 7:
        WiFiPlazaHistory_Add(&work->history, 0x15, -1);
        break;
    case 8:
        WiFiPlazaHistory_Add(&work->history, 0x16, -1);
        break;
    }
    work->profile.player.unk_43 = value;
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
    WiFiPlazaWork_SendOwnRecord(work);
}

void WiFiPlazaWork_AddHistory(WiFiPlazaWork *work, int index, int value) {
    if (index >= 24) {
        GF_AssertFail();
    }
    WiFiPlazaHistory_Add(&work->history, index, value);
    WiFiPlazaWork_SendOwnRecord(work);
}

void WiFiPlazaWork_SetTapToy(WiFiPlazaWork *work, u32 value) {
    if (value >= 27) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
        return;
    }
    work->profile.player.unk_41 = value;
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
    WiFiPlazaWork_SendOwnRecord(work);
}

void WiFiPlazaWork_SetFavoriteTypes(WiFiPlazaWork *work, int a, int b) {
    if (a >= 18) {
        GF_AssertFail();
    }
    if (b >= 18) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
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
        WiFiPlazaWork_SendOwnRecord(work);
    }
}

void WiFiPlazaWork_SetSurveyAnswer(WiFiPlazaWork *work, u32 a, u32 b) {
    if (b >= 3) {
        GF_AssertFail();
    }
    if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
        work->hasError = 1;
        return;
    }
    if (b < 3) {
        work->profile.player.survey.surveyId = a;
        work->profile.player.survey.answer = b;
        work->eventSchedule.low = 1;
        ov45_0222B118(work, 6);
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        WiFiPlazaWork_SendOwnRecord(work);
    }
}

void WiFiPlazaPlayer_CopyToProfile(WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID) {
    BOOL invalid;
    int ok = WiFiPlaza_IsNameTerminated(player->name, 8, heapID);
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

int WiFiPlazaPlayer_GetActivity(WiFiPlazaPlayer *player) {
    u8 v = player->unk_43;
    return v < 14 ? v : 14;
}

#pragma force_active on
#pragma opt_common_subs off

u8 WiFiPlazaPlayer_GetHistoryId(WiFiPlazaPlayer *player, u32 index) {
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

void *WiFiPlazaPlayer_GetHistoryValue(WiFiPlazaPlayer *player, u32 index) {
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

u32 WiFiPlazaPlayer_GetLobbyId(WiFiPlazaPlayer *player) {
    return player->lobbyId;
}

u32 WiFiPlazaPlayer_GetTrainerID(WiFiPlazaPlayer *player) {
    return player->trainerId;
}

int WiFiPlazaPlayer_GetAvatarIndex(WiFiPlazaPlayer *player) {
    u16 v = WiFiPlazaPlayer_GetAvatar(player);
    int i;
    for (i = 0; i < 16; i++) {
        if (v == ov45_02254A84[i].avatar) {
            return i;
        }
    }
    return 24;
}

void *WiFiPlazaPlayer_GetLocalTime(WiFiPlazaPlayer *player) {
    return player->localTime;
}

u32 WiFiPlazaPlayer_GetGender(WiFiPlazaPlayer *player) {
    u32 result;
    u32 i;

    if (player->gender >= 2) {
        if (WiFiPlaza_IndexToAvatar(player->avatarIndex) != 0xFFFF) {
            for (i = 0; i < 16; i++) {
                if (ov45_02254A84[i].avatar == player->avatarIndex) {
                    result = ov45_02254A84[i].avatar;
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

u8 WiFiPlazaPlayer_GetRawLanguage(WiFiPlazaPlayer *player) {
    return player->language;
}

u16 WiFiPlazaPlayer_GetAvatar(WiFiPlazaPlayer *player) {
    u16 avatar = WiFiPlaza_IndexToAvatar(player->avatarIndex);
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
    if (WiFiPlaza_IsRegionValid(player->country, player->region) == 0) {
        return 0;
    }
    return player->country;
}

u8 WiFiPlazaPlayer_GetRegion(WiFiPlazaPlayer *player) {
    if (WiFiPlaza_IsRegionValid(player->country, player->region) == 0) {
        return 0;
    }
    return player->region;
}

#pragma force_active reset

u8 WiFiPlazaPlayer_GetTapToy(WiFiPlazaPlayer *player) {
    u8 v = player->unk_41;
    if (v == 0xFF) {
        return 0;
    }
    if (v >= 27) {
        return 0;
    }
    return v;
}

BOOL WiFiPlazaPlayer_HasTapToy(WiFiPlazaPlayer *player) {
    return player->unk_41 == 0xFF ? FALSE : TRUE;
}

#pragma force_active on

u16 WiFiPlazaPlayer_GetFavoriteType(WiFiPlazaPlayer *player, u32 index) {
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

void WiFiPlazaPlayer_GetSurvey(WiFiPlazaPlayer *player, WiFiPlazaPlayerSurvey *out) {
    *out = player->survey;
}

#pragma force_active reset
