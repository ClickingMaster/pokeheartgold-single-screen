#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_AGB_CARTRIDGE_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_AGB_CARTRIDGE_H

#include "global.h"

#include "pokemon_types_def.h"

// Access to a Pokemon Ruby/Sapphire/Emerald/FireRed/LeafGreen cartridge in the GBA slot for Migrate from
// Ruby/Sapphire/... (Pal Park): identification, the cartridge's save (2 save slots of 14 flash sectors) and its
// boxed Pokemon (src/application/main_menu/agb_cartridge.c).

// Results of PmAgbCartridgeGetOffsets / PmAgbCartridge_LoadSave / the save functions
#define PM_AGB_RESULT_OK                 0
#define PM_AGB_RESULT_NO_CARTRIDGE       1 // no GBA cartridge inserted
#define PM_AGB_RESULT_UNKNOWN_GAME       2 // game code not in sPmAgbRomCodeMappings
#define PM_AGB_RESULT_NO_FLASH           3 // no flash backup chip
#define PM_AGB_RESULT_BAD_HEADER         4 // Nintendo logo or header checksum mismatch / save sectors missing
#define PM_AGB_RESULT_SAVE_DAMAGED       5
#define PM_AGB_RESULT_SAVE_CORRUPT       6
#define PM_AGB_RESULT_NO_SAVE            7
#define PM_AGB_RESULT_WRITE_FAILED       8
#define PM_AGB_RESULT_WRITE_STARTING     9
#define PM_AGB_RESULT_WRITE_BUSY         10
#define PM_AGB_RESULT_WRITE_LAST         11 // only the last sector is left
#define PM_AGB_RESULT_ALREADY_IDENTIFIED 12

#define AGB_SAVE_SECTORS_PER_SLOT 14
#define AGB_SAVE_SECTOR_SIZE      0x1000
#define AGB_SAVE_SECTOR_SIGNATURE 0x08012025

#define AGB_BOX_COUNT     14
#define AGB_BOX_MON_COUNT 30

// One entry of sPmAgbRomCodeMappings
typedef struct PmAgbCartridgeSpec {
    u32 gameCode; // "AXVE" etc.
    u8 gameType;  // 0 Ruby/Sapphire (fixed save layout), 1 FireRed/LeafGreen, 2 Emerald (layout from the ROM header)
    u8 version;   // 0 Ruby, 1 Sapphire, 2 LeafGreen, 3 FireRed, 4 Emerald
    u16 language;
} PmAgbCartridgeSpec;

// sPmAgbCartridgeSpec
typedef struct PmAgbCartridge {
    PmAgbCartridgeSpec *spec; // the inserted game, NULL if none was identified
    int saveSlot;             // slot the next save goes to (-1: none)
} PmAgbCartridge;

// Footer of each 4 KB save sector
typedef struct AgbSaveSector {
    u8 data[0xFF4];
    u16 id; // 0 trainer info, 1-4 game state, 5-13 PC boxes
    u16 checksum;
    u32 signature; // AGB_SAVE_SECTOR_SIGNATURE
    u32 counter;   // save counter, the higher of the two slots is the latest save
} AgbSaveSector;

// A boxed Pokemon of the GBA games (0x50 bytes); the four 12-byte substructures are encrypted and their order
// depends on the personality value (AGB_GetBoxMonSubstruct)
typedef struct AgbBoxMon {
    u32 personality;
    u32 otId;
    u8 nickname[10];
    u8 language;
    u8 isBadEgg : 1;
    u8 hasSpecies : 1;
    u8 isEgg : 1;
    u8 unused : 5;
    u8 otName[7];
    u8 markings;
    u16 checksum;
    u16 unknown;
    u32 secure[12];
} AgbBoxMon;

// The PC boxes of the GBA save (PmAgbCartridge_GetPCStorage)
typedef struct AgbPokemonStorage {
    u32 currentBox;
    AgbBoxMon boxes[AGB_BOX_COUNT][AGB_BOX_MON_COUNT];
    u8 boxNames[AGB_BOX_COUNT][9];
    u8 wallpapers[AGB_BOX_COUNT];
} AgbPokemonStorage;

// Field ids of AGB_GetBoxMonData / AGB_SetBoxMonData (the ones MigrateBoxMon copies, named after their DP counterparts)
enum AgbMonData {
    AGB_MON_DATA_PERSONALITY = 0,
    AGB_MON_DATA_OT_ID,
    AGB_MON_DATA_NICKNAME,
    AGB_MON_DATA_LANGUAGE,
    AGB_MON_DATA_IS_BAD_EGG,
    AGB_MON_DATA_HAS_SPECIES,
    AGB_MON_DATA_SANITY_IS_EGG,
    AGB_MON_DATA_OT_NAME,
    AGB_MON_DATA_MARKINGS,
    AGB_MON_DATA_CHECKSUM,
    AGB_MON_DATA_UNKNOWN_10,
    AGB_MON_DATA_SPECIES, // 11, the first field of the encrypted part
    AGB_MON_DATA_HELD_ITEM,
    AGB_MON_DATA_MOVE1,
    AGB_MON_DATA_MOVE2,
    AGB_MON_DATA_MOVE3,
    AGB_MON_DATA_MOVE4,
    AGB_MON_DATA_MOVE1_PP,
    AGB_MON_DATA_MOVE2_PP,
    AGB_MON_DATA_MOVE3_PP,
    AGB_MON_DATA_MOVE4_PP,
    AGB_MON_DATA_PP_UPS, // 2 bits per move
    AGB_MON_DATA_COOL,
    AGB_MON_DATA_BEAUTY,
    AGB_MON_DATA_CUTE,
    AGB_MON_DATA_EXPERIENCE,
    AGB_MON_DATA_HP_EV,
    AGB_MON_DATA_ATK_EV,
    AGB_MON_DATA_DEF_EV,
    AGB_MON_DATA_SPEED_EV,
    AGB_MON_DATA_SPATK_EV,
    AGB_MON_DATA_SPDEF_EV,
    AGB_MON_DATA_FRIENDSHIP,
    AGB_MON_DATA_SMART,
    AGB_MON_DATA_POKERUS,
    AGB_MON_DATA_MET_LOCATION,
    AGB_MON_DATA_MET_LEVEL,
    AGB_MON_DATA_MET_GAME,
    AGB_MON_DATA_POKEBALL,
    AGB_MON_DATA_HP_IV,
    AGB_MON_DATA_ATK_IV,
    AGB_MON_DATA_DEF_IV,
    AGB_MON_DATA_SPEED_IV,
    AGB_MON_DATA_SPATK_IV,
    AGB_MON_DATA_SPDEF_IV,
    AGB_MON_DATA_IS_EGG,
    AGB_MON_DATA_ABILITY_NUM,
    AGB_MON_DATA_TOUGH,
    AGB_MON_DATA_SHEEN,
    AGB_MON_DATA_OT_GENDER,
    AGB_MON_DATA_COOL_RIBBONS, // 50, contest ribbon counts (0-4)
    AGB_MON_DATA_BEAUTY_RIBBONS,
    AGB_MON_DATA_CUTE_RIBBONS,
    AGB_MON_DATA_SMART_RIBBONS,
    AGB_MON_DATA_TOUGH_RIBBONS,
    AGB_MON_DATA_SPECIES_OR_EGG = 65, // the Egg species if it is an egg (or a bad egg)
    AGB_MON_DATA_COMBINED_IVS,
    AGB_MON_DATA_CHAMPION_RIBBON, // 67-78: the single ribbons, in the order of MON_DATA_CHAMPION_RIBBON...
    AGB_MON_DATA_WORLD_RIBBON = 78,
    AGB_MON_DATA_FATEFUL_ENCOUNTER = 80,
};

BOOL PmAgbCartridgeHasFlash(void);
int PmAgbCartridgeGetOffsets(void *buffer);
void PmAgbCartridgeUnlinkSpec(void);
u8 PmAgbCartridge_GetVersionInternal(void);
u16 PmAgbCartridge_GetLanguage(void);
int PmAgbCartridge_LoadSave(void);
BOOL PmAgbCartridge_StartSave(void);
int PmAgbCartridge_ContinueSave(void);
int PmAgbCartridge_GetSaveStatus(void);
AgbPokemonStorage *PmAgbCartridge_GetPCStorage(void);
void *PmAgbCartridge_GetTrainerInfo(void);
u32 PmAgbCartridge_GetTrainerId(void);
void *PmAgbCartridge_GetPlayerName(void *unused);
u32 AGB_GetBoxMonData(AgbBoxMon *mon, int field, u8 *dest);
void AGB_SetBoxMonData(AgbBoxMon *mon, int field, const void *value);
int TranslateAgbSpecies(int species);
u32 AGB_GetBoxMonLevel(AgbBoxMon *mon);
void MigrateBoxMon(AgbBoxMon *agbMon, BoxPokemon *boxMon);

// GBA (Ruby/Sapphire charset) string -> DP string, defined with the overlay helpers
BOOL ConvertRSStringToDPStringInternational(const u8 *src, u16 *dst, u32 length, u32 language);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_AGB_CARTRIDGE_H
