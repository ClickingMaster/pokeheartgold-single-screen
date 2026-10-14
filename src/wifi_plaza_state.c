#include "global.h"

#include "constants/game_stats.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "game_stats.h"
#include "party.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon.h"
#include "sav_system_info.h"
#include "save_wifi_history.h"
#include "sound.h"
#include "sound_system.h"
#include "wifi_plaza_internal.h"

// What PPW_LobbyGetTimeInfo reports: the lobby time first.
typedef struct WiFiPlazaLobbyTime {
    s64 time;
    u8 unk_08[16];
} WiFiPlazaLobbyTime;

void WiFiPlazaWork_IncPartnerTalkCount(WiFiPlazaWork *work) {
    if (WiFiPlazaWork_IsTalking(work) == TRUE) {
        WiFiPlazaTalkSession_IncTalkCount(&work->talk, work->talk.partnerSlot);
    }
}

void WiFiPlazaWork_IncTalkCount(WiFiPlazaWork *work, int slot) {
    WiFiPlazaTalkSession_IncTalkCount(&work->talk, slot);
}

void WiFiPlazaWork_SetTalkedWith(WiFiPlazaWork *work, int slot) {
    WiFiPlazaTalkSession_SetTalkedWith(&work->talk, slot);
}

void IncrementGameStat47(WiFiPlazaWork *work) {
    GameStats_Inc(Save_GameStats_Get(work->save), GAME_STAT_UNK47);
}

void IncrementGameStat119(WiFiPlazaWork *work) {
    GameStats_Inc(Save_GameStats_Get(work->save), GAME_STAT_UNK119);
}

void WiFiPlazaWork_MarkActivityDone(WiFiPlazaWork *work, int slot) {
    if (slot >= 8) {
        GF_AssertFail();
    }
    work->unk_3a0.flags[slot] = 1;
}

int WiFiPlazaWork_GetNextUndoneActivity(WiFiPlazaWork *work) {
    int i;
    int ok;
    for (i = 0; i < 8; i++) {
        if (work->unk_3a0.flags[i] == 0) {
            switch (i) {
            case 0:
            case 1:
            case 2:
                if (WiFiPlazaWork_GetEventScheduleMode5(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 3:
            case 4:
            case 5:
                if (WiFiPlazaWork_IsClosing(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 6:
                if (WiFiPlazaWork_IsLightingStarted(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 7:
            default:
                ok = 1;
                break;
            }
            if (ok == 1) {
                return i;
            }
        }
    }
    return 8;
}

int WiFiPlazaWork_PickTapToy(WiFiPlazaWork *work) {
    int result = WiFiPlazaGroupWeights_PickGroup(&work->groupWeights);
    int value = WiFiPlazaWork_GetOwnSlot(work);
    if (WiFiPlazaWork_IsSpecialVisitor(work, value) == 1) {
        result += 2;
    }
    return result;
}

u32 WiFiPlazaWork_GetFireworksPhase(WiFiPlazaWork *work) {
    return WiFiPlazaTimer_GetPhase(&work->timer);
}

u32 WiFiPlazaWork_GetFireworksCounter(WiFiPlazaWork *work) {
    return WiFiPlazaTimer_GetCounter(&work->timer);
}

void WiFiPlazaWork_SetSurveyTally(WiFiPlazaWork *work, const WiFiPlazaTallyCache *tally) {
    work->unk_508.hasTally = TRUE;
    work->unk_508.tally = *tally;
}

BOOL WiFiPlazaWork_GetSurveyTally(WiFiPlazaWork *work, WiFiPlazaTallyCache *tally) {
    if (work->unk_508.hasTally == TRUE) {
        *tally = work->unk_508.tally;
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaWork_NewPlayerInfo(WiFiPlazaWork *work) {
    if (work->timer.playerInfo != NULL) {
        GF_AssertFail();
    }
    work->timer.playerInfo = WiFiPlazaPlayerInfo_New(Save_PlayerData_GetProfile(work->save), work->heapID);
}

void WiFiPlazaWork_FreePlayerInfo(WiFiPlazaWork *work) {
    if (work->timer.playerInfo != NULL) {
        WiFiPlazaPlayerInfo_Free(work->timer.playerInfo);
        work->timer.playerInfo = NULL;
    }
}

// TRUE if the name holds a terminator (0xFFFF) within count characters; the heap ID is not used.
BOOL WiFiPlaza_IsNameTerminated(const u16 *name, u32 count, enum HeapID heapID) {
    u32 i;
    for (i = 0; i < count; i++) {
        if (name[i] == 0xffff) {
            return TRUE;
        }
    }
    return FALSE;
}

// The original objects of the four lobby callbacks below and of WiFiPlaza_FillPlaces / WiFiPlazaProfile_SetFromLobbyRecord were force-active.
#pragma force_active on

// Lobby callback: a player entered the plaza (isOwn: the own player, whose profile is synchronised).
void WiFiPlaza_OnPlayerEnter(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work, BOOL isOwn) {
    s64 time;
    int country;
    int region;
    int slot;
    int index;
    int valid;
    if (isOwn) {
        WiFiPlazaProfile_CopyToPlayerProfile(&work->unk_508.profile, &work->profile, work->heapID);
        if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
            work->hasError = 1;
        }
        WiFiPlazaNews_AddPlayer(work->news, WiFiPlazaLobbyClient_IdToSlot(id), work->profile.player.gender, 3);
        work->profile.player.lobbyId = id;
        WiFiPlazaLobbyClient_GetEntryTime(id, &time);
        WiFiPlaza_SecondsToTime(&work->profile.player.entryTime, &time);
        country = WiFiPlazaPlayer_GetCountry(&work->profile.player);
        region = WiFiPlazaPlayer_GetRegion(&work->profile.player);
        valid = WiFiPlazaPlayer_IsLanguageValid(&work->profile.player);
        if (country && valid == TRUE) {
            *(WiFiPlazaTime *)&work->profile.player.localTime = WiFiPlaza_GetLocalTime((u16)country, (u8)region, work->profile.player.entryTime, work->heapID);
        } else {
            ((u8 *)&work->profile.player.localTime)[0] = 12;
            ((u8 *)&work->profile.player.localTime)[1] = 0;
            ((u8 *)&work->profile.player.localTime)[2] = 0;
        }
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        WiFiPlazaWork_SendOwnRecord(work);
    } else {
        country = WiFiPlazaLobbyClient_GetOldestPlayer();
        region = WiFiPlazaLobbyClient_IdToSlot(id);
        WiFiPlazaNews_AddPlayer(work->news, region, WiFiPlazaPlayer_GetGender(player), 0);
        if (country == id) {
            WiFiPlazaNews_SetOldestPlayer(work->news, region);
        }
        if (WiFiPlazaHistory_ReplaceValue(&work->history, id, -1)) {
            WiFiPlazaWork_SendOwnRecord(work);
        }
    }
    index = WiFiPlazaLobbyClient_IdToSlot(id);
    work->changeFlags[0] |= 1 << index;
    if (WiFiPlazaLobbyClient_IsSpecialVisitor(id) == TRUE) {
        WiFiPlaza_SetSlotFlag(work->specialVisitors, index, 1);
        slot = index;
        WiFiPlazaNews_AddSpecialVisitorEvent(work->news, &slot, work->specialVisitors);
        country = WiFiPlazaLobbyClient_GetSpecialVisitorCode(id);
        if (country) {
            WiFiPlazaWordSet_Set(&work->wordSet, index, 1, country);
        } else {
            WiFiPlazaWordSet_Set(&work->wordSet, index, 0, country);
        }
    } else {
        WiFiPlaza_SetSlotFlag(work->specialVisitors, index, 0);
        WiFiPlazaWordSet_Set(&work->wordSet, index, 0, 0);
    }
}

#pragma force_active reset

// Lobby callback: a player left the plaza.
void WiFiPlaza_OnPlayerLeave(int id, WiFiPlazaWork *work) {
    int slot;
    if (id == WiFiPlazaLobbyClient_GetOwnId()) {
        return;
    }
    slot = WiFiPlazaLobbyClient_IdToSlot(id);
    if (WiFiPlazaWork_IsTalking(work) == TRUE && slot == WiFiPlazaWork_GetTalkPartner(work)) {
        WiFiPlazaTalkSession_Abort(&work->talk);
    }
    if (WiFiPlazaNews_RemovePlayer(work->news, slot) != 0) {
        WiFiPlazaNews_SetOldestPlayer(work->news, WiFiPlazaLobbyClient_IdToSlot(WiFiPlazaLobbyClient_GetOldestOtherPlayer(id)));
    }
    WiFiPlaza_SetSlotFlag(work->specialVisitors, slot, 0);
    WiFiPlazaTalkSession_ResetTalkCount(&work->talk, slot);
    WiFiPlazaTalkSession_ClearTalkedWith(&work->talk, slot);
    if (WiFiPlazaHistory_ReplaceValue(&work->history, id, -1) != 0) {
        WiFiPlazaWork_SendOwnRecord(work);
    }
    WiFiPlazaWordSet_Set(&work->wordSet, slot, 0, 0);
    work->changeFlags[1] |= 1 << WiFiPlazaLobbyClient_IdToSlot(id);
}

// Lobby callback: the record of a player changed.
void WiFiPlaza_OnPlayerUpdate(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work) {
    int slot = WiFiPlazaLobbyClient_IdToSlot(id);
    work->changeFlags[3] |= 1 << slot;
    switch (WiFiPlazaPlayer_GetActivity(player)) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        WiFiPlazaTalkSession_ResetTalkCount(&work->talk, slot);
        break;
    }
    if (WiFiPlazaPlayer_HasTapToy(player) == TRUE) {
        WiFiPlazaGroupWeights_ReportPlace(&work->groupWeights, WiFiPlazaPlayer_GetTapToy(player), slot);
    }
}

#pragma force_active on

// Lobby callback: a lobby event (0: the lobby opened); most of them start a timer in WiFiPlazaWork.eventSchedule.
void WiFiPlaza_OnLobbyEvent(u32 event, WiFiPlazaWork *work) {
    WiFiPlazaLobbyTime now;
    int notify = 0;
    int which;
    PPW_LobbyGetTimeInfo(&now);
    WiFiPlaza_SecondsToTime(&work->currentTime, &now.time);
    work->lobbyTime = now.time;
    switch (event) {
    case 0:
        WiFiPlazaNews_SetOpenTime(work->news, &work->currentTime);
        work->lobbyOpenTime = work->lobbyTime;
        work->eventSchedule.flag0 = 1;
        work->changeFlags[2] |= 1;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (!work->lightingStarted) {
            work->lightingStarted = 1;
            notify = 1;
            which = 0;
            work->eventSchedule.lightingDelay = 300;
        }
    case 1:
        work->eventSchedule.colorEvent = event;
        work->changeFlags[2] |= 2;
        break;
    case 8:
    case 9:
    case 10:
        if (!work->lightingStarted) {
            work->lightingStarted = 1;
            notify = 1;
            which = 0;
            work->eventSchedule.lightingDelay = 300;
        }
    case 7:
        work->eventSchedule.light1Event = event;
        work->changeFlags[2] |= 4;
        break;
    case 12:
    case 13:
    case 14:
        if (!work->lightingStarted) {
            work->lightingStarted = 1;
            notify = 1;
            which = 0;
            work->eventSchedule.lightingDelay = 300;
        }
    case 11:
        work->eventSchedule.thirdEvent = event;
        work->changeFlags[2] |= 8;
        break;
    case 16: {
        int a;
        int b;
        notify = 1;
        which = 1;
        a = WiFiPlazaLobbyClient_GetScheduleValue(6);
        b = WiFiPlazaLobbyClient_GetScheduleValue(7);
        WiFiPlazaTimer_Init(&work->timer, a, b);
        work->eventSchedule.fireworksDelay = 300;
    } break;
    case 17:
        work->eventSchedule.mode2 = 2;
        work->changeFlags[2] |= 32;
        break;
    case 18:
        notify = 1;
        which = 2;
        work->eventSchedule.paradeDelay = 300;
        break;
    case 19:
        notify = 1;
        which = 4;
        work->eventSchedule.flag7 = 1;
        work->eventSchedule.closeTimer = 900;
        break;
    case 15:
        work->eventSchedule.mode5 = 1;
        work->changeFlags[2] |= 16;
        break;
    }
    if (notify) {
        WiFiPlazaWork_AddLobbyEventNotice(work, which);
    }
}

#pragma force_active reset

// Lobby callback: the own record as the lobby stores it.
void WiFiPlaza_OnOwnRecord(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work) {
    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->hasError = 1;
        return;
    }
    WiFiPlazaProfile_SetFromLobbyRecord(&work->profile, data, size);
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
}

#pragma force_active on

// Fills out (200 bytes) with the country and region of the own player, of every player in the lobby and of the
// 20 lobby entries of WiFiPlazaLobbyClient_GetLocationCountry / WiFiPlazaLobbyClient_GetLocationRegion.
void WiFiPlaza_FillPlaces(WiFiPlazaProfile *profile, WiFiPlazaPlace *out) {
    WiFiPlazaPlayerList list;
    int country;
    int region;
    int i;
    WiFiPlazaPlayer *player;
    memset(out, 0, 200);
    WiFiPlazaLobbyClient_GetPlayerList(&list);
    player = &profile->player;
    country = WiFiPlazaPlayer_GetCountry(player);
    region = WiFiPlazaPlayer_GetRegion(player);
    WiFiPlazaPlaces_Add(out, (u16)country, (u8)region, 0);
    for (i = 0; i < 20; i++) {
        if (list.ids[i] != -1) {
            player = WiFiPlazaLobbyClient_GetPlayer(list.ids[i]);
            country = WiFiPlazaPlayer_GetCountry(player);
            region = WiFiPlazaPlayer_GetRegion(player);
            WiFiPlazaPlaces_Add(out, (u16)country, (u8)region, 0);
        }
    }
    for (i = 0; i < 20; i++) {
        u8 entryCountry = (u8)WiFiPlazaLobbyClient_GetLocationCountry((u8)i);
        WiFiPlazaPlaces_Add(out, entryCountry, WiFiPlazaLobbyClient_GetLocationRegion((u8)i), 1);
    }
}

#pragma force_active reset

void WiFiPlazaWork_CheckClosingTime(WiFiPlazaWork *work) {
    if (work->eventSchedule.flag0 == 1 && work->eventSchedule.flag7 == 0 && WiFiPlazaLobbyClient_GetState() == 2) {
        s64 duration = (u32)WiFiPlazaWork_GetOpenDuration(work);
        if (duration + work->lobbyOpenTime < work->lobbyTime) {
            work->eventSchedule.flag7 = 1;
            work->eventSchedule.closeTimer = 900;
        }
    }
}

void WiFiPlazaProfile_InitFromSave(WiFiPlazaProfile *profile, SaveData *save, enum HeapID heapID) {
    PlayerProfile *playerProfile;
    Party *party;
    SaveWiFiHistory *history;
    Pokedex *pokedex;
    SysInfo_RTC *rtc;
    String *name;
    int count;
    int i;
    int j;
    playerProfile = Save_PlayerData_GetProfile(save);
    party = SaveArray_Party_Get(save);
    pokedex = Save_Pokedex_Get(save);
    history = Save_WiFiHistory_Get(save);
    rtc = Save_SysInfo_RTC_Get(save);
    name = PlayerProfile_GetPlayerName_NewString(playerProfile, heapID);
    CopyStringToU16Array(name, profile->player.name, 8);
    CopyStringToU16Array(name, profile->name, 8);
    String_Delete(name);
    profile->player.lobbyId = -1;
    profile->player.trainerId = PlayerProfile_GetTrainerID(playerProfile);
    count = Party_GetCount(party);
    for (i = 0; i < PARTY_SIZE; i++) {
        if (i < count) {
            Pokemon *mon = Party_GetMonByIndex(party, i);
            profile->player.species[i] = GetMonData(mon, MON_DATA_SPECIES, NULL);
            profile->player.forms[i] = GetMonData(mon, MON_DATA_FORM, NULL);
            profile->player.isEgg[i] = GetMonData(mon, MON_DATA_IS_EGG, NULL);
        } else {
            profile->player.species[i] = SPECIES_BAD_EGG; // empty party slot
        }
    }
    profile->player.gender = PlayerProfile_GetTrainerGender(playerProfile);
    profile->player.language = PlayerProfile_GetLanguage(playerProfile);
    profile->player.avatarIndex = PlayerProfile_GetAvatar(playerProfile);
    profile->player.avatarIndex = WiFiPlaza_AvatarToIndex(profile->player.avatarIndex);
    profile->player.country = WifiHistory_GetPlayerCountry(history);
    profile->player.region = WiFiHistory_GetPlayerRegion(history);
    profile->player.hasNationalDex = Pokedex_GetNatDexFlag(pokedex);
    profile->player.gameCleared = PlayerProfile_GetGameClearFlag(playerProfile);
    profile->player.unk_41 = 0xff;
    profile->player.unk_43 = 0;
    profile->player.version = GAME_VERSION;
    profile->player.unk_44 = rtc->seconds_since_nitro_epoch;
    for (i = 0; i < 12; i++) {
        profile->player.slots[i] = 24;
        profile->player.slotData[i] = (void *)-1;
    }
    for (j = 0; j < 2; j++) {
        profile->player.unk_88[j] = 0;
    }
    profile->player.survey.surveyId = -1;
    profile->player.survey.answer = 3;
    WiFiPlazaProfile_UpdateChecksum(profile, save);
}

void WiFiPlazaWork_SendOwnRecord(WiFiPlazaWork *work) {
    int i;
    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->hasError = 1;
        return;
    }
    for (i = 0; i < 12; i++) {
        work->profile.player.slots[i] = WiFiPlazaHistory_GetId(&work->history, i);
        work->profile.player.slotData[i] = (void *)WiFiPlazaHistory_GetValue(&work->history, i);
    }
    MI_CpuCopy8(work->profile.lobbyName, work->profile.player.name, sizeof(work->profile.player.name));
    WiFiPlazaLobbyClient_SetOwnPlayer(&work->profile.player);
    MI_CpuCopy8(work->profile.name, work->profile.player.name, sizeof(work->profile.player.name));
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
}

void WiFiPlazaProfile_UpdateChecksum(WiFiPlazaProfile *profile, SaveData *save) {
    profile->crc = SaveArray_CalcCRC16(save, &profile->player, sizeof(WiFiPlazaPlayer));
}

BOOL WiFiPlazaProfile_IsChecksumValid(WiFiPlazaProfile *profile, SaveData *save) {
    u16 crc = SaveArray_CalcCRC16(save, &profile->player, sizeof(WiFiPlazaPlayer));
    if (crc != profile->crc) {
        GF_AssertFail();
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

void WiFiPlazaProfile_SetFromLobbyRecord(WiFiPlazaProfile *profile, const WiFiPlazaPlayer *src, u32 size) {
    profile->player = *src;
    if (size <= sizeof(WiFiPlazaPlayer)) {
        MI_CpuCopy8(src, &profile->player, size);
    } else {
        MI_CpuCopy8(src, &profile->player, sizeof(WiFiPlazaPlayer));
    }
    MI_CpuCopy8(src->name, profile->lobbyName, sizeof(profile->lobbyName));
    MI_CpuCopy8(profile->name, profile->player.name, sizeof(profile->player.name));
}

#pragma force_active reset

void WiFiPlaza_ClearEventFlags(u32 *flags) {
    flags[2] = 0;
}

// Counts down the four lobby timers of state; each one raises a flag in flags[2] when it runs out.
void WiFiPlazaEventSchedule_Update(WiFiPlazaEventSchedule *state, u32 *flags, WiFiPlazaGroups *groups, WiFiPlazaTimer *timer) {
    if (state->closeTimer - 1 >= 0) {
        state->closeTimer--;
        if (state->closeTimer == 840) {
            GF_SndStartFadeOutBGM(0, 127);
            state->bgmFadedOut = 1;
        }
    } else if (state->closeTimer == 0) {
        state->flag1 = 1;
        flags[2] |= 0x80;
        state->closeTimer--;
    }

    if (state->lightingDelay - 1 >= 0) {
        state->lightingDelay--;
    } else if (state->lightingDelay == 0) {
        flags[2] |= 4;
        flags[2] |= 8;
        flags[2] |= 2;
        state->lightingDelay--;
    }

    if (state->fireworksDelay - 1 >= 0) {
        state->fireworksDelay--;
    } else if (state->fireworksDelay == 0) {
        state->mode2 = 1;
        flags[2] |= 0x20;
        state->fireworksDelay--;
        WiFiPlazaTimer_Start(timer);
    }

    if (state->paradeDelay - 1 >= 0) {
        state->paradeDelay--;
    } else if (state->paradeDelay == 0) {
        WiFiPlazaGroups_Start(groups);
        if (state->unk_0c == 0) {
            PlayBGM(SEQ_GS_WIFIPARADE);
        }
        state->flag4 = 1;
        flags[2] |= 0x40;
        state->paradeDelay--;
    }
}

void WiFiPlazaEventSchedule_Init(WiFiPlazaEventSchedule *state) {
    state->flag0 = 0;
    state->flag1 = 0;
    state->mode2 = 0;
    state->flag4 = 0;
    state->mode5 = 0;
    state->flag7 = 0;
    state->colorEvent = 1;
    state->light1Event = 7;
    state->thirdEvent = 11;
    state->closeTimer = -1;
    state->lightingDelay = -1;
    state->fireworksDelay = -1;
    state->paradeDelay = -1;
}

void WiFiPlazaEventSchedule_ApplyVolume(WiFiPlazaEventSchedule *state) {
    if (state->lowVolume == 1) {
        GF_SndHandleSetPlayerVolume(7, 42);
    } else {
        GF_SndHandleSetPlayerVolume(7, 127);
    }
}

void WiFiPlazaUnk3A0_StartCooldown(WiFiPlazaUnk3A0 *a0) {
    a0->timer = 900;
}

int WiFiPlazaUnk3A0_IsCoolingDown(WiFiPlazaUnk3A0 *a0) {
    return a0->timer > 0;
}

void WiFiPlazaUnk3A0_UpdateCooldown(WiFiPlazaUnk3A0 *a0) {
    if (a0->timer > 0) {
        a0->timer--;
    }
}

void WiFiPlaza_NewPlayerProfile(PlayerProfile **profile, enum HeapID heapID) {
    *profile = PlayerProfile_New(heapID);
}

void WiFiPlaza_FreePlayerProfile(PlayerProfile **profile) {
    Heap_Free(*profile);
}

// Copies the own lobby record into *profile (with the name the lobby has).
void WiFiPlazaProfile_CopyToPlayerProfile(PlayerProfile **profile, WiFiPlazaProfile *src, enum HeapID heapID) {
    WiFiPlazaPlayer *player = Heap_Alloc(heapID, sizeof(WiFiPlazaPlayer));
    MIi_CpuCopyFast((const u32 *)&src->player, (u32 *)player, sizeof(WiFiPlazaPlayer));
    MI_CpuCopy8(src->lobbyName, player->name, sizeof(player->name));
    WiFiPlazaPlayer_CopyToProfile(player, *profile, heapID);
    Heap_Free(player);
}

void WiFiPlazaUnk3A0_SetAppQuitForClosing(WiFiPlazaUnk3A0 *a0) {
    a0->unk_02 = 1;
}

u8 WiFiPlazaUnk3A0_DidAppQuitForClosing(WiFiPlazaUnk3A0 *a0) {
    return a0->unk_02;
}
