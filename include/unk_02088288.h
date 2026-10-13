#ifndef POKEHEARTGOLD_UNK_02088288_H
#define POKEHEARTGOLD_UNK_02088288_H

#include "menu_input_state.h"
#include "options.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "save_special_ribbons.h"

extern struct OverlayManagerTemplate gOverlayTemplate_PokemonSummary;

typedef struct PokemonSummaryArgs {
    Party *party;
    Options *options;
    const u16 *playerName; // with playerId and playerGender: tells the player's own Pokemon
    u32 playerId; // with playerGender and the player's name: tells the player's own Pokemon
    u8 playerGender;
    u8 monDataType;
    u8 mode;
    u8 partyCount;
    u8 partySlot;
    u8 pageFlags;
    u8 selectedMoveSlot;
    u8 exitedWithB; // set to 1 when the screen is left with B
    u16 moveToLearn;
    u8 unk1A[0x2];
    BOOL natDexEnabled;
    SaveSpecialRibbons *ribbons;
    u8 unk24[0x4];
    int chatot;
    int unk2C;
    MenuInputStateMgr *menuInputStatePtr;
    BOOL isFlag982Set;
    u8 monModified[0x4];
} PokemonSummaryArgs;

u32 sub_02088288(SaveData *);
BOOL PokemonSummary_IsPokeathlonUnlocked(SaveData *);
void PokemonSummary_SetPages(PokemonSummaryArgs *, const u8 *);
void PokemonSummaryArgs_SetPlayer(PokemonSummaryArgs *, PlayerProfile *);
int StatusIcon_GetCharNaix(void);
int StatusIcon_GetPlttNaix(void);
int StatusIcon_GetCellNaix(void);
int StatusIcon_GetAnimNaix(void);
u32 Pokemon_GetStatusIconId(Pokemon *pokemon);

#endif // POKEHEARTGOLD_UNK_02088288_H
