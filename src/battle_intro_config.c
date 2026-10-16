#include "battle_intro_config.h"

#include "global.h"

#include "constants/flags.h"
#include "constants/flypoints.h"
#include "constants/maps.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/trainer_class.h"

#include "field/encounter_effect.h"

#include "assert.h"
#include "map_header.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "script_pokemon_util.h"
#include "sys_flags.h"

typedef struct BattleIntroMusic {
    u16 transition;
    u16 music;
} BattleIntroMusic;

typedef struct PackedBattleParam {
    u16 key : 10;
    u16 param : 6;
} PackedBattleParam;

static u32 BattleSetup_GetTransitionAndMusicParam(BattleSetup *setup);
static u32 BattleStartGetTransition(u32 param, BattleSetup *setup);
static u32 BattleStartGetMusic(u32 param, BattleSetup *setup);
static BOOL BattleIntroConfig_AlwaysTrue(u32 param);
static u32 NPCTrainerGetBattleIntroAndMusicParam(u32 trainerClass);
static u32 WildPokemonGetBattleIntroAndMusicParam(Party *party, u32 mapId);

static const PackedBattleParam sWildBattleIntroParams[11] = {
    { SPECIES_RAIKOU,   22 },
    { SPECIES_ENTEI,    23 },
    { SPECIES_SUICUNE,  24 },
    { SPECIES_LUGIA,    26 },
    { SPECIES_HO_OH,    25 },
    { SPECIES_GROUDON,  27 },
    { SPECIES_KYOGRE,   27 },
    { SPECIES_RAYQUAZA, 27 },
    { SPECIES_MEWTWO,   28 },
    { SPECIES_LATIOS,   28 },
    { SPECIES_LATIAS,   28 },
};

static const PackedBattleParam sTrainerBattleIntroParams[32] = {
    { TRAINERCLASS_LEADER_FALKNER,   0  },
    { TRAINERCLASS_LEADER_BUGSY,     1  },
    { TRAINERCLASS_LEADER_WHITNEY,   2  },
    { TRAINERCLASS_LEADER_MORTY,     3  },
    { TRAINERCLASS_LEADER_JASMINE,   4  },
    { TRAINERCLASS_LEADER_CHUCK,     5  },
    { TRAINERCLASS_LEADER_PRYCE,     6  },
    { TRAINERCLASS_LEADER_CLAIR,     7  },
    { TRAINERCLASS_LEADER_BROCK,     8  },
    { TRAINERCLASS_LEADER_MISTY,     9  },
    { TRAINERCLASS_LEADER_LT_SURGE,  10 },
    { TRAINERCLASS_LEADER_ERIKA,     11 },
    { TRAINERCLASS_LEADER_JANINE,    12 },
    { TRAINERCLASS_LEADER_SABRINA,   13 },
    { TRAINERCLASS_LEADER_BLAINE,    14 },
    { TRAINERCLASS_LEADER_BLUE,      15 },
    { TRAINERCLASS_ELITE_FOUR_WILL,  16 },
    { TRAINERCLASS_ELITE_FOUR_KOGA,  17 },
    { TRAINERCLASS_ELITE_FOUR_BRUNO, 18 },
    { TRAINERCLASS_ELITE_FOUR_KAREN, 19 },
    { TRAINERCLASS_CHAMPION,         20 },
    { TRAINERCLASS_RIVAL,            21 },
    { TRAINERCLASS_PASSERBY,         21 },
    { TRAINERCLASS_EXECUTIVE_ARCHER, 32 },
    { TRAINERCLASS_EXECUTIVE_ARIANA, 33 },
    { TRAINERCLASS_EXECUTIVE_PROTON, 31 },
    { TRAINERCLASS_EXECUTIVE_PETREL, 30 },
    { TRAINERCLASS_ROCKET_BOSS,      34 },
    { TRAINERCLASS_TEAM_ROCKET,      29 },
    { TRAINERCLASS_TEAM_ROCKET_F,    29 },
    { TRAINERCLASS_KIMONO_GIRL,      43 },
    { TRAINERCLASS_PKMN_TRAINER_RED, 44 },
};

static const BattleIntroMusic sBattleIntroEffectsAndMusic[45] = {
    { 12,     SEQ_GS_VS_GYMREADER       },
    { 13,     SEQ_GS_VS_GYMREADER       },
    { 14,     SEQ_GS_VS_GYMREADER       },
    { 15,     SEQ_GS_VS_GYMREADER       },
    { 16,     SEQ_GS_VS_GYMREADER       },
    { 17,     SEQ_GS_VS_GYMREADER       },
    { 18,     SEQ_GS_VS_GYMREADER       },
    { 19,     SEQ_GS_VS_GYMREADER       },
    { 20,     SEQ_GS_VS_GYMREADER_KANTO },
    { 21,     SEQ_GS_VS_GYMREADER_KANTO },
    { 22,     SEQ_GS_VS_GYMREADER_KANTO },
    { 23,     SEQ_GS_VS_GYMREADER_KANTO },
    { 24,     SEQ_GS_VS_GYMREADER_KANTO },
    { 25,     SEQ_GS_VS_GYMREADER_KANTO },
    { 26,     SEQ_GS_VS_GYMREADER_KANTO },
    { 27,     SEQ_GS_VS_GYMREADER_KANTO },
    { 29,     SEQ_GS_VS_GYMREADER       },
    { 30,     SEQ_GS_VS_GYMREADER       },
    { 31,     SEQ_GS_VS_GYMREADER       },
    { 32,     SEQ_GS_VS_GYMREADER       },
    { 33,     SEQ_GS_VS_CHAMP           },
    { 28,     SEQ_GS_VS_RIVAL           },
    { 0xffff, SEQ_GS_VS_RAIKOU          },
    { 0xffff, SEQ_GS_VS_ENTEI           },
    { 0xffff, SEQ_GS_VS_SUICUNE         },
    { 35,     SEQ_GS_VS_HOUOU           },
    { 36,     SEQ_GS_VS_LUGIA           },
    { 34,     SEQ_GS_VS_KODAI           },
    { 34,     SEQ_GS_VS_NORAPOKE_KANTO  },
    { 39,     SEQ_GS_VS_ROCKET          },
    { 40,     SEQ_GS_VS_ROCKET          },
    { 41,     SEQ_GS_VS_ROCKET          },
    { 42,     SEQ_GS_VS_ROCKET          },
    { 43,     SEQ_GS_VS_ROCKET          },
    { 44,     SEQ_GS_VS_ROCKET          },
    { 37,     SEQ_GS_VS_TRAINER         },
    { 37,     SEQ_GS_VS_TRAINER         },
    { 38,     SEQ_GS_VS_TRAINER         },
    { 38,     SEQ_GS_VS_NORAPOKE        },
    { 37,     SEQ_GS_BA_BRAIN           },
    { 38,     SEQ_GS_VS_GYMREADER       },
    { 0xffff, SEQ_GS_VS_TRAINER         },
    { 0xffff, SEQ_GS_VS_NORAPOKE        },
    { 45,     SEQ_GS_VS_TRAINER         },
    { 46,     SEQ_GS_VS_CHAMP           },
};

static u32 BattleSetup_GetTransitionAndMusicParam(BattleSetup *setup) {
    u32 battleType = setup->battleType;
    u32 param;
    if (battleType & BATTLE_TYPE_TRAINER) {
        param = NPCTrainerGetBattleIntroAndMusicParam(setup->trainer[BATTLER_ENEMY].data.trainerClass);
        if (battleType & BATTLE_TYPE_FRONTIER) {
            if (param == 39) {
                return param;
            }
            return (battleType & BATTLE_TYPE_DOUBLES) ? 37 : 35;
        }
        if (param >= 29 && param <= 34) {
            return param;
        }
        if (param == 43) {
            return param;
        }
        if (param == 44) {
            return param;
        }
        if (battleType & BATTLE_TYPE_DOUBLES) {
            return 37;
        }
        if (battleType & BATTLE_TYPE_LINK) {
            return 36;
        }
    } else {
        param = WildPokemonGetBattleIntroAndMusicParam(setup->party[BATTLER_ENEMY], setup->mapNumber);
        if (param >= 42 && (battleType & BATTLE_TYPE_DOUBLES)) {
            return 38;
        }
    }
    return param;
}

static u32 BattleStartGetTransition(u32 param, BattleSetup *setup) {
    u32 result;
    GF_ASSERT(param < NELEMS(sBattleIntroEffectsAndMusic));
    result = sBattleIntroEffectsAndMusic[param].transition;
    if (result == 0xffff) {
        result = EncounterEffect_GetDefaultEffect(setup);
    }
    return result;
}

static u32 BattleStartGetMusic(u32 param, BattleSetup *setup) {
    GF_ASSERT(param < NELEMS(sBattleIntroEffectsAndMusic));
    return sBattleIntroEffectsAndMusic[param].music;
}

int BattleSetup_GetWildTransitionEffect(BattleSetup *setup) {
    return BattleStartGetTransition(BattleSetup_GetTransitionAndMusicParam(setup), setup);
}

static BOOL BattleIntroConfig_AlwaysTrue(u32 param) {
    return TRUE;
}

// Once Vermilion City has been visited, battles on Kanto maps use the Kanto versions of the wild, trainer and
// gym leader themes.
int BattleSetup_GetWildBattleMusic(BattleSetup *setup) {
    u32 param = BattleSetup_GetTransitionAndMusicParam(setup);
    u32 music = BattleStartGetMusic(param, setup);
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(setup->saveData);
    if (Save_VarsFlags_FlypointFlagAction(varsFlags, FLAG_ACTION_CHECK, FLYPOINT_VERMILION) == TRUE && MapHeader_GetRegionNo(setup->mapNumber) != 0) {
        if (music == SEQ_GS_VS_NORAPOKE) {
            music = SEQ_GS_VS_NORAPOKE_KANTO;
        } else if (music == SEQ_GS_VS_TRAINER) {
            music = SEQ_GS_VS_TRAINER_KANTO;
        } else if (music == SEQ_GS_VS_GYMREADER && !BattleIntroConfig_AlwaysTrue(param)) {
            music = SEQ_GS_VS_GYMREADER_KANTO;
        }
    }
    return music;
}

static u32 NPCTrainerGetBattleIntroAndMusicParam(u32 trainerClass) {
    u32 i;
    for (i = 0; i < NELEMS(sTrainerBattleIntroParams); i++) {
        if (sTrainerBattleIntroParams[i].key == trainerClass) {
            return sTrainerBattleIntroParams[i].param;
        }
    }
    return 41;
}

static u32 WildPokemonGetBattleIntroAndMusicParam(Party *party, u32 mapId) {
    u32 i;
    u32 species = GetMonData(GetFirstAliveMonInParty_CrashIfNone(party), MON_DATA_SPECIES, NULL);
    if (mapId == MAP_PAL_PARK) {
        return 42;
    }
    for (i = 0; i < NELEMS(sWildBattleIntroParams); i++) {
        if (sWildBattleIntroParams[i].key == species) {
            return sWildBattleIntroParams[i].param;
        }
    }
    return 42;
}
