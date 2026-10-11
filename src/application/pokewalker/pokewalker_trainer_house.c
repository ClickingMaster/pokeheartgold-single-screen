// Pokewalker connection app: event log check, and the Trainer House sets exchanged by the app. A set received from
// the other side replaces the same trainer's set or, when all ten are in use, the oldest one; the per-set daily flags
// (FLAG_DAILY_TRAINER_HOUSE_0..) move with the sets.

#include "application/pokewalker/pokewalker_trainer_house.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/species.h"

#include "application/pokewalker/pokewalker_modes2.h"

#include "frontier_data_access.h"
#include "mail_message.h"
#include "math_util.h"
#include "player_data.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "string_util.h"

extern const u16 sTrainerHouseDailyFlags[MAX_NUM_TRAINER_HOUSE_SETS];

BOOL PokewalkerApp_IsEventLogInvalid(PokewalkerIrRecord *records) {
    u16 maxSpecies = NATIONAL_DEX_COUNT;
    u16 minusSeven = -7;
    int i;
    for (i = 0; i < 24; i++, records++) {
        if (records->eventType != 0) {
            u16 species;
            if (records->eventType >= 30) {
                return TRUE;
            }
            species = records->unk_0A;
            if (species == SPECIES_NONE) {
                return TRUE;
            }
            if ((u16)(records->unk_08 + minusSeven) <= 1) { // 7 or 8
                if (species > maxSpecies || records->unk_0C > maxSpecies) {
                    return TRUE;
                }
            }
            if (records->unk_76 >= 8) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void PokewalkerApp_BuildTrainerHouseData(SaveData *saveData, void *dst) {
    PokewalkerTrainerHouseData *data = dst;
    PlayerProfile *profile = Save_PlayerData_GetProfile(saveData);
    Party *party = SaveArray_Party_Get(saveData);

    MI_CpuClearFast(data, sizeof(PokewalkerTrainerHouseData));
    TrainerHouseSet_SetZero(&data->set);
    PokewalkerApp_FillTrainerHouseTrainer(&data->set.trainer, saveData);
    PokewalkerApp_FillTrainerHouseParty(data->set.party, party);
    data->crc = GF_CalcCRC16(data, offsetof(PokewalkerTrainerHouseData, crc));
}

#pragma force_active on
void PokewalkerApp_SaveTrainerHouseData(SaveData *saveData, PokewalkerTrainerHouseData *data) {
    u8 dailyFlags[12];
    BOOL found = FALSE;
    u8 dailyFlag = FALSE;
    BOOL shift = FALSE;
    int count;
    int index;
    TrainerHouse *trainerHouse = Save_TrainerHouse_Get(saveData);

    if (!TrainerHouseSet_CheckHasData(&data->set)) {
        PokewalkerApp_CountTrainerHouseSets(trainerHouse);
        return;
    }
    if (data->crc == GF_CalcCRC16(data, offsetof(PokewalkerTrainerHouseData, crc))) {
        count = PokewalkerApp_CountTrainerHouseSets(trainerHouse);
        PokewalkerApp_GetTrainerHouseDailyFlags(saveData, dailyFlags);
        index = PokewalkerApp_FindTrainerHouseSet(trainerHouse, &data->set.trainer, &found);
        if (count >= MAX_NUM_TRAINER_HOUSE_SETS || found) {
            shift = TRUE;
            count--;
            dailyFlag = dailyFlags[index];
        }
        if (shift && index < count) {
            for (; index < count; index++) {
                trainerHouse->sets[index] = trainerHouse->sets[index + 1];
                dailyFlags[index] = dailyFlags[index + 1];
            }
        }
        MI_CpuCopy8(&data->set, &trainerHouse->sets[count], sizeof(TrainerHouseSet));
        dailyFlags[count] = dailyFlag;
        PokewalkerApp_SetTrainerHouseDailyFlags(saveData, dailyFlags);
    }
}
#pragma force_active reset

void PokewalkerApp_FillTrainerHouseTrainer(TrainerHouseTrainer *trainer, SaveData *saveData) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(saveData);
    int i;

    trainer->id = PlayerProfile_GetTrainerID(profile);
    trainer->gender = PlayerProfile_GetTrainerGender(profile);
    trainer->language = PlayerProfile_GetLanguage(profile);
    trainer->version = PlayerProfile_GetVersion(profile);
    trainer->sprite = PlayerProfile_GetAvatar(profile);
    CopyU16StringArrayN(trainer->otName, PlayerProfile_GetNamePtr(profile), PLAYER_NAME_LENGTH);
    for (i = 0; i < 3; i++) {
        Save_Frontier_GetTowerMessage(saveData, i);
        MailMsg_Copy(&(&trainer->introMessage)[i], Save_Frontier_GetTowerMessage(saveData, i));
    }
}

#pragma force_active on
// Party Pokemon that are not eggs, Sky Forme Shaymin as Land Forme.
void PokewalkerApp_FillTrainerHouseParty(TrainerHouseMon *mons, Party *party) {
    int i;
    int count = Party_GetCount(party);
    int written = 0;
    int j;
    Pokemon *mon;
    u32 bits;
    BOOL restoreForm;
    u16 nickname[POKEMON_NAME_LENGTH + 1];
    u8 *move;
    int shift;

    for (i = 0; i < count; i++) {
        restoreForm = FALSE;
        mon = Party_GetMonByIndex(party, i);
        if (GetMonData(mon, MON_DATA_CHECKSUM_FAILED, NULL) || GetMonData(mon, MON_DATA_IS_EGG, NULL)) {
            continue;
        }
        mons->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        mons->form = GetMonData(mon, MON_DATA_FORM, NULL);
        if (mons->species == SPECIES_SHAYMIN && mons->form == 1) {
            Mon_UpdateShayminForm(mon, 0);
            mons->form = 0;
            restoreForm = TRUE;
        }
        mons->item = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        mons->otid = GetMonData(mon, MON_DATA_OT_ID, NULL);
        mons->pid = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
        mons->language = GetMonData(mon, MON_DATA_LANGUAGE, NULL);
        mons->ability = GetMonData(mon, MON_DATA_ABILITY, NULL);
        mons->friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);
        mons->level = GetMonData(mon, MON_DATA_LEVEL, NULL);
        GetMonData(mon, MON_DATA_NICKNAME, nickname);
        CopyU16StringArrayN(mons->nickname, nickname, POKEMON_NAME_LENGTH);
        // matching: the moves are written through a byte pointer to the entry stepped by 2 per move
        j = 0;
        bits = 0;
        move = (u8 *)mons;
        shift = j;
        for (; j < MAX_MON_MOVES; j++, shift += 2, move += sizeof(u16)) {
            *(u16 *)(move + offsetof(TrainerHouseMon, moves)) = GetMonData(mon, MON_DATA_MOVE1 + j, NULL);
            bits |= GetMonData(mon, MON_DATA_MOVE1_PP_UPS + j, NULL) << shift;
        }
        mons->ppUp = bits;
        bits = 0;
        for (j = 0, shift = 0; j < NUM_STATS; j++, shift += 5) {
            bits |= GetMonData(mon, MON_DATA_HP_IV + j, NULL) << shift;
            (&mons->hpEv)[j] = GetMonData(mon, MON_DATA_HP_EV + j, NULL);
        }
        mons->ivsWord = bits;
        if (restoreForm) {
            Mon_UpdateShayminForm(mon, 1);
        }
        mons++;
        if (++written >= PARTY_SIZE) {
            break;
        }
    }
}
#pragma force_active reset

int PokewalkerApp_CountTrainerHouseSets(TrainerHouse *trainerHouse) {
    int i;
    int count;
    for (i = 0, count = 0; i < MAX_NUM_TRAINER_HOUSE_SETS; i++, count++) {
        if (!TrainerHouseSet_CheckHasData(&trainerHouse->sets[i])) {
            break;
        }
    }
    return count;
}

int PokewalkerApp_FindTrainerHouseSet(TrainerHouse *trainerHouse, TrainerHouseTrainer *trainer, BOOL *found) {
    int i;
    for (i = 0; i < MAX_NUM_TRAINER_HOUSE_SETS; i++) {
        if (TrainerHouseSet_CheckHasData(&trainerHouse->sets[i]) && TrainerHouseTrainer_Compare(&trainerHouse->sets[i].trainer, trainer)) {
            *found = TRUE;
            return i;
        }
    }
    *found = FALSE;
    return 0;
}

void PokewalkerApp_GetTrainerHouseDailyFlags(SaveData *saveData, u8 *flags) {
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(saveData);
    int i;
    for (i = 0; i < MAX_NUM_TRAINER_HOUSE_SETS; i++) {
        flags[i] = Save_VarsFlags_CheckFlagInArray(varsFlags, sTrainerHouseDailyFlags[i]);
    }
}

void PokewalkerApp_SetTrainerHouseDailyFlags(SaveData *saveData, const u8 *flags) {
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(saveData);
    int i;
    for (i = 0; i < MAX_NUM_TRAINER_HOUSE_SETS; i++) {
        if (flags[i]) {
            Save_VarsFlags_SetFlagInArray(varsFlags, sTrainerHouseDailyFlags[i]);
        } else {
            Save_VarsFlags_ClearFlagInArray(varsFlags, sTrainerHouseDailyFlags[i]);
        }
    }
}
