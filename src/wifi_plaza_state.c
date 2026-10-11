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

// What ov45_022320C4 reports: the lobby time first.
typedef struct WiFiPlazaLobbyTime {
    s64 time;
    u8 unk_08[16];
} WiFiPlazaLobbyTime;

void ov45_0222B0BC(WiFiPlazaWork *work) {
    if (ov45_0222AFF8(work) == TRUE) {
        ov45_0222BDCC(&work->unk_1c0, work->unk_1c0.unk_04);
    }
}

void ov45_0222B0D8(WiFiPlazaWork *work, int slot) {
    ov45_0222BDCC(&work->unk_1c0, slot);
}

void ov45_0222B0E8(WiFiPlazaWork *work, int slot) {
    ov45_0222BD94(&work->unk_1c0, slot);
}

void IncrementGameStat47(WiFiPlazaWork *work) {
    GameStats_Inc(Save_GameStats_Get(work->save), GAME_STAT_UNK47);
}

void IncrementGameStat119(WiFiPlazaWork *work) {
    GameStats_Inc(Save_GameStats_Get(work->save), GAME_STAT_UNK119);
}

void ov45_0222B118(WiFiPlazaWork *work, int slot) {
    if (slot >= 8) {
        GF_AssertFail();
    }
    work->unk_3a0.flags[slot] = 1;
}

int ov45_0222B134(WiFiPlazaWork *work) {
    int i;
    int ok;
    for (i = 0; i < 8; i++) {
        if (work->unk_3a0.flags[i] == 0) {
            switch (i) {
            case 0:
            case 1:
            case 2:
                if (ov45_0222A394(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 3:
            case 4:
            case 5:
                if (ov45_0222A330(work) == 1) {
                    ok = 0;
                } else {
                    ok = 1;
                }
                break;
            case 6:
                if (ov45_0222A3A0(work) == 1) {
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

int ov45_0222B1B4(WiFiPlazaWork *work) {
    int result = ov45_0222C900(&work->groupWeights);
    int value = ov45_0222A53C(work);
    if (ov45_0222AB28(work, value) == 1) {
        result += 2;
    }
    return result;
}

u32 ov45_0222B1DC(WiFiPlazaWork *work) {
    return ov45_0222CB40(&work->timer);
}

u32 ov45_0222B1EC(WiFiPlazaWork *work) {
    return ov45_0222CB3C(&work->timer);
}

void ov45_0222B1FC(WiFiPlazaWork *work, const WiFiPlazaTallyCache *tally) {
    work->unk_508.hasTally = TRUE;
    work->unk_508.tally = *tally;
}

BOOL ov45_0222B21C(WiFiPlazaWork *work, WiFiPlazaTallyCache *tally) {
    if (work->unk_508.hasTally == TRUE) {
        *tally = work->unk_508.tally;
        return TRUE;
    }
    return FALSE;
}

void ov45_0222B244(WiFiPlazaWork *work) {
    if (work->timer.unk_1c != NULL) {
        GF_AssertFail();
    }
    work->timer.unk_1c = WiFiPlazaPlayerInfo_New(Save_PlayerData_GetProfile(work->save), work->heapID);
}

void ov45_0222B270(WiFiPlazaWork *work) {
    if (work->timer.unk_1c != NULL) {
        WiFiPlazaPlayerInfo_Free(work->timer.unk_1c);
        work->timer.unk_1c = NULL;
    }
}

// TRUE if the name holds a terminator (0xFFFF) within count characters; the heap ID is not used.
BOOL ov45_0222B28C(const u16 *name, u32 count, enum HeapID heapID) {
    u32 i;
    for (i = 0; i < count; i++) {
        if (name[i] == 0xffff) {
            return TRUE;
        }
    }
    return FALSE;
}

// The original objects of the four lobby callbacks below and of ov45_0222B79C / ov45_0222BB00 were force-active.
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
        ov45_0222BCE4(&work->unk_508.profile, &work->profile, work->heapID);
        if (!WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save)) {
            work->unk_52c = 1;
        }
        ov45_0222D8C8(work->news, ov45_0222EC68(id), work->profile.player.gender, 3);
        work->profile.player.unk_00 = id;
        ov45_0222EA4C(id, &time);
        ov45_0222D500(&work->profile.player.unk_18, &time);
        country = WiFiPlazaPlayer_GetCountry(&work->profile.player);
        region = WiFiPlazaPlayer_GetRegion(&work->profile.player);
        valid = WiFiPlazaPlayer_IsLanguageValid(&work->profile.player);
        if (country && valid == TRUE) {
            *(WiFiPlazaTime *)&work->profile.player.unk_1c = ov45_02230F94((u16)country, (u8)region, work->profile.player.unk_18, work->heapID);
        } else {
            ((u8 *)&work->profile.player.unk_1c)[0] = 12;
            ((u8 *)&work->profile.player.unk_1c)[1] = 0;
            ((u8 *)&work->profile.player.unk_1c)[2] = 0;
        }
        WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
        ov45_0222BA3C(work);
    } else {
        country = WiFiPlazaLobbyClient_GetOldestPlayer();
        region = ov45_0222EC68(id);
        ov45_0222D8C8(work->news, region, WiFiPlazaPlayer_GetGender(player), 0);
        if (country == id) {
            ov45_0222D8F0(work->news, region);
        }
        if (ov45_0222CBD0(&work->history, id, -1)) {
            ov45_0222BA3C(work);
        }
    }
    index = ov45_0222EC68(id);
    work->unk_f8[0] |= 1 << index;
    if (ov45_0222F484(id) == TRUE) {
        ov45_0222C944(work->unk_3cc, index, 1);
        slot = index;
        ov45_0222DC08(work->news, &slot, work->unk_3cc);
        country = ov45_0222F4AC(id);
        if (country) {
            ov45_0222C9A0(&work->wordSet, index, 1, country);
        } else {
            ov45_0222C9A0(&work->wordSet, index, 0, country);
        }
    } else {
        ov45_0222C944(work->unk_3cc, index, 0);
        ov45_0222C9A0(&work->wordSet, index, 0, 0);
    }
}

#pragma force_active reset

// Lobby callback: a player left the plaza.
void WiFiPlaza_OnPlayerLeave(int id, WiFiPlazaWork *work) {
    int slot;
    if (id == WiFiPlazaLobbyClient_GetOwnId()) {
        return;
    }
    slot = ov45_0222EC68(id);
    if (ov45_0222AFF8(work) == TRUE && slot == ov45_0222B020(work)) {
        ov45_0222BD5C(&work->unk_1c0);
    }
    if (ov45_0222D8D4(work->news, slot) != 0) {
        ov45_0222D8F0(work->news, ov45_0222EC68(WiFiPlazaLobbyClient_GetOldestOtherPlayer(id)));
    }
    ov45_0222C944(work->unk_3cc, slot, 0);
    ov45_0222BDE8(&work->unk_1c0, slot);
    ov45_0222BDB0(&work->unk_1c0, slot);
    if (ov45_0222CBD0(&work->history, id, -1) != 0) {
        ov45_0222BA3C(work);
    }
    ov45_0222C9A0(&work->wordSet, slot, 0, 0);
    work->unk_f8[1] |= 1 << ov45_0222EC68(id);
}

// Lobby callback: the record of a player changed.
void WiFiPlaza_OnPlayerUpdate(int id, WiFiPlazaPlayer *player, WiFiPlazaWork *work) {
    int slot = ov45_0222EC68(id);
    work->unk_f8[3] |= 1 << slot;
    switch (ov45_0222A920(player)) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        ov45_0222BDE8(&work->unk_1c0, slot);
        break;
    }
    if (ov45_0222AADC(player) == TRUE) {
        ov45_0222C8C8(&work->groupWeights, ov45_0222AAC8(player), slot);
    }
}

#pragma force_active on

// Lobby callback: a lobby event (0: the lobby opened); most of them start a timer in WiFiPlazaWork.unk_1fc.
void WiFiPlaza_OnLobbyEvent(u32 event, WiFiPlazaWork *work) {
    WiFiPlazaLobbyTime now;
    int notify = 0;
    int which;
    ov45_022320C4(&now);
    ov45_0222D500(&work->unk_d4, &now.time);
    work->unk_d8 = now.time;
    switch (event) {
    case 0:
        ov45_0222D8BC(work->news, &work->unk_d4);
        work->unk_e0 = work->unk_d8;
        work->unk_1fc.flag0 = 1;
        work->unk_f8[2] |= 1;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (!work->unk_08) {
            work->unk_08 = 1;
            notify = 1;
            which = 0;
            work->unk_1fc.unk_06 = 300;
        }
    case 1:
        work->unk_1fc.unk_01 = event;
        work->unk_f8[2] |= 2;
        break;
    case 8:
    case 9:
    case 10:
        if (!work->unk_08) {
            work->unk_08 = 1;
            notify = 1;
            which = 0;
            work->unk_1fc.unk_06 = 300;
        }
    case 7:
        work->unk_1fc.unk_02 = event;
        work->unk_f8[2] |= 4;
        break;
    case 12:
    case 13:
    case 14:
        if (!work->unk_08) {
            work->unk_08 = 1;
            notify = 1;
            which = 0;
            work->unk_1fc.unk_06 = 300;
        }
    case 11:
        work->unk_1fc.unk_03 = event;
        work->unk_f8[2] |= 8;
        break;
    case 16: {
        int a;
        int b;
        notify = 1;
        which = 1;
        a = ov45_0222ECDC(6);
        b = ov45_0222ECDC(7);
        ov45_0222CA7C(&work->timer, a, b);
        work->unk_1fc.unk_08 = 300;
    } break;
    case 17:
        work->unk_1fc.mode2 = 2;
        work->unk_f8[2] |= 32;
        break;
    case 18:
        notify = 1;
        which = 2;
        work->unk_1fc.unk_0a = 300;
        break;
    case 19:
        notify = 1;
        which = 4;
        work->unk_1fc.flag7 = 1;
        work->unk_1fc.unk_04 = 900;
        break;
    case 15:
        work->unk_1fc.mode5 = 1;
        work->unk_f8[2] |= 16;
        break;
    }
    if (notify) {
        ov45_0222C370(work, which);
    }
}

#pragma force_active reset

// Lobby callback: the own record as the lobby stores it.
void ov45_0222B75C(const WiFiPlazaPlayer *data, u32 size, WiFiPlazaWork *work) {
    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->unk_52c = 1;
        return;
    }
    ov45_0222BB00(&work->profile, data, size);
    WiFiPlazaProfile_UpdateChecksum(&work->profile, work->save);
}

#pragma force_active on

// Fills out (200 bytes) with the country and region of the own player, of every player in the lobby and of the
// 20 lobby entries of ov45_0222EDC4 / ov45_0222EDF0.
void ov45_0222B79C(WiFiPlazaProfile *profile, WiFiPlazaPlace *out) {
    WiFiPlazaPlayerList list;
    int country;
    int region;
    int i;
    WiFiPlazaPlayer *player;
    memset(out, 0, 200);
    ov45_0222EC10(&list);
    player = &profile->player;
    country = WiFiPlazaPlayer_GetCountry(player);
    region = WiFiPlazaPlayer_GetRegion(player);
    ov45_0222D638(out, (u16)country, (u8)region, 0);
    for (i = 0; i < 20; i++) {
        if (list.ids[i] != -1) {
            player = ov45_0222EA2C(list.ids[i]);
            country = WiFiPlazaPlayer_GetCountry(player);
            region = WiFiPlazaPlayer_GetRegion(player);
            ov45_0222D638(out, (u16)country, (u8)region, 0);
        }
    }
    for (i = 0; i < 20; i++) {
        u8 entryCountry = (u8)ov45_0222EDC4((u8)i);
        ov45_0222D638(out, entryCountry, ov45_0222EDF0((u8)i), 1);
    }
}

#pragma force_active reset

void ov45_0222B840(WiFiPlazaWork *work) {
    if (work->unk_1fc.flag0 == 1 && work->unk_1fc.flag7 == 0 && ov45_0222E96C() == 2) {
        s64 duration = (u32)ov45_0222AD4C(work);
        if (duration + work->unk_e0 < work->unk_d8) {
            work->unk_1fc.flag7 = 1;
            work->unk_1fc.unk_04 = 900;
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
    profile->player.unk_00 = -1;
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
    profile->player.unk_3a = PlayerProfile_GetAvatar(playerProfile);
    profile->player.unk_3a = ov45_0222CCE4(profile->player.unk_3a);
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
    profile->player.unk_8c.unk_00 = -1;
    profile->player.unk_8c.unk_04 = 3;
    WiFiPlazaProfile_UpdateChecksum(profile, save);
}

void ov45_0222BA3C(WiFiPlazaWork *work) {
    int i;
    if (WiFiPlazaProfile_IsChecksumValid(&work->profile, work->save) == FALSE) {
        work->unk_52c = 1;
        return;
    }
    for (i = 0; i < 12; i++) {
        work->profile.player.slots[i] = ov45_0222CC50(&work->history, i);
        work->profile.player.slotData[i] = (void *)ov45_0222CC7C(&work->history, i);
    }
    MI_CpuCopy8(work->profile.unk_10, work->profile.player.name, sizeof(work->profile.player.name));
    ov45_0222E9F8(&work->profile.player);
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

void ov45_0222BB00(WiFiPlazaProfile *profile, const WiFiPlazaPlayer *src, u32 size) {
    profile->player = *src;
    if (size <= sizeof(WiFiPlazaPlayer)) {
        MI_CpuCopy8(src, &profile->player, size);
    } else {
        MI_CpuCopy8(src, &profile->player, sizeof(WiFiPlazaPlayer));
    }
    MI_CpuCopy8(src->name, profile->unk_10, sizeof(profile->unk_10));
    MI_CpuCopy8(profile->name, profile->player.name, sizeof(profile->player.name));
}

#pragma force_active reset

void ov45_0222BB58(u32 *flags) {
    flags[2] = 0;
}

// Counts down the four lobby timers of state; each one raises a flag in flags[2] when it runs out.
void ov45_0222BB60(WiFiPlazaUnk1FC *state, u32 *flags, WiFiPlazaUnk20C *a2, WiFiPlazaTimer *a3) {
    if (state->unk_04 - 1 >= 0) {
        state->unk_04--;
        if (state->unk_04 == 840) {
            GF_SndStartFadeOutBGM(0, 127);
            state->unk_0e = 1;
        }
    } else if (state->unk_04 == 0) {
        state->flag1 = 1;
        flags[2] |= 0x80;
        state->unk_04--;
    }

    if (state->unk_06 - 1 >= 0) {
        state->unk_06--;
    } else if (state->unk_06 == 0) {
        flags[2] |= 4;
        flags[2] |= 8;
        flags[2] |= 2;
        state->unk_06--;
    }

    if (state->unk_08 - 1 >= 0) {
        state->unk_08--;
    } else if (state->unk_08 == 0) {
        state->mode2 = 1;
        flags[2] |= 0x20;
        state->unk_08--;
        ov45_0222CA8C(a3);
    }

    if (state->unk_0a - 1 >= 0) {
        state->unk_0a--;
    } else if (state->unk_0a == 0) {
        ov45_0222C3A8(a2);
        if (state->unk_0c == 0) {
            PlayBGM(SEQ_GS_WIFIPARADE);
        }
        state->flag4 = 1;
        flags[2] |= 0x40;
        state->unk_0a--;
    }
}

void ov45_0222BC3C(WiFiPlazaUnk1FC *state) {
    state->flag0 = 0;
    state->flag1 = 0;
    state->mode2 = 0;
    state->flag4 = 0;
    state->mode5 = 0;
    state->flag7 = 0;
    state->unk_01 = 1;
    state->unk_02 = 7;
    state->unk_03 = 11;
    state->unk_04 = -1;
    state->unk_06 = -1;
    state->unk_08 = -1;
    state->unk_0a = -1;
}

void ov45_0222BC84(WiFiPlazaUnk1FC *state) {
    if (state->unk_0d == 1) {
        GF_SndHandleSetPlayerVolume(7, 42);
    } else {
        GF_SndHandleSetPlayerVolume(7, 127);
    }
}

void ov45_0222BCA0(WiFiPlazaUnk3A0 *a0) {
    a0->timer = 900;
}

int ov45_0222BCA8(WiFiPlazaUnk3A0 *a0) {
    return a0->timer > 0;
}

void ov45_0222BCB8(WiFiPlazaUnk3A0 *a0) {
    if (a0->timer > 0) {
        a0->timer--;
    }
}

void ov45_0222BCC8(PlayerProfile **profile, enum HeapID heapID) {
    *profile = PlayerProfile_New(heapID);
}

void ov45_0222BCD8(PlayerProfile **profile) {
    Heap_Free(*profile);
}

// Copies the own lobby record into *profile (with the name the lobby has).
void ov45_0222BCE4(PlayerProfile **profile, WiFiPlazaProfile *src, enum HeapID heapID) {
    WiFiPlazaPlayer *player = Heap_Alloc(heapID, sizeof(WiFiPlazaPlayer));
    MIi_CpuCopyFast((const u32 *)&src->player, (u32 *)player, sizeof(WiFiPlazaPlayer));
    MI_CpuCopy8(src->unk_10, player->name, sizeof(player->name));
    WiFiPlazaPlayer_CopyToProfile(player, *profile, heapID);
    Heap_Free(player);
}

void ov45_0222BD24(WiFiPlazaUnk3A0 *a0) {
    a0->unk_02 = 1;
}

u8 ov45_0222BD2C(WiFiPlazaUnk3A0 *a0) {
    return a0->unk_02;
}
