#include "global.h"

#include "constants/pokemon.h"

#include "gts_internal.h"
#include "heap.h"
#include "overlay_manager.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "evolution_scene.h"

extern const OverlayManagerTemplate sGtsTradeSequenceTemplate;

// Defined in save_block_22.c (copies the deposited Pokemon out of the GTS save data, GtsArgs.gtsSave, which
// Save_Gts_Get returns as an int); not declared in a header.
void GtsSave_GetStoredMon(void *gtsSave, void *dest);

int GtsTrade_Init(GtsApp *app, int state);
int GtsTrade_Main(GtsApp *app, int state);
int GtsTrade_Exit(GtsApp *app, int state);
PlayerProfile *GtsTrade_CreatePartnerProfile(GtsRecord *record);
Pokemon *GtsTrade_GetReceivedMon(GtsApp *app, int kind);
void GtsTrade_StoreReceivedMon(GtsApp *app);

#pragma force_active on

// Starts the trade sequence (OVY_71) for the kind of trade in nextScreenArg (7..10).
int GtsTrade_Init(GtsApp *app, int state) {
    app->depositedMon = AllocMonZeroed(HEAP_ID_61);
    switch (app->nextScreenArg) {
    case 7:
        app->tradeAnim.my_boxmon = Mon_GetBoxMon(&app->record.mon);
        app->tradeAnim.trade_boxmon = app->tradeAnim.my_boxmon;
        app->partnerProfile = GtsTrade_CreatePartnerProfile(&app->record);
        app->tradeAnim.trade_profile = app->partnerProfile;
        app->tradeAnim.time_of_day = 3;
        app->tradeAnim.is_ingame = 2;
        break;
    case 8:
        app->tradeAnim.trade_boxmon = Mon_GetBoxMon(&app->record.mon);
        app->tradeAnim.my_boxmon = app->tradeAnim.trade_boxmon;
        app->partnerProfile = GtsTrade_CreatePartnerProfile(&app->record);
        app->tradeAnim.trade_profile = app->partnerProfile;
        app->tradeAnim.time_of_day = 3;
        app->tradeAnim.is_ingame = 4;
        break;
    case 10:
        app->tradeAnim.trade_boxmon = Mon_GetBoxMon(&app->record.mon);
        GtsSave_GetStoredMon((void *)app->args->gtsSave, app->depositedMon);
        app->tradeAnim.my_boxmon = Mon_GetBoxMon(app->depositedMon);
        app->partnerProfile = GtsTrade_CreatePartnerProfile(&app->record);
        app->tradeAnim.trade_profile = app->partnerProfile;
        app->tradeAnim.time_of_day = 3;
        app->tradeAnim.is_ingame = 1;
        break;
    case 9:
        GtsSave_GetStoredMon((void *)app->args->gtsSave, app->depositedMon);
        app->tradeAnim.my_boxmon = Mon_GetBoxMon(app->depositedMon);
        app->tradeAnim.trade_boxmon = Mon_GetBoxMon(&app->searchResults[app->selectedResult].mon);
        app->partnerProfile = GtsTrade_CreatePartnerProfile(&app->searchResults[app->selectedResult]);
        app->tradeAnim.trade_profile = app->partnerProfile;
        app->tradeAnim.time_of_day = 3;
        app->tradeAnim.is_ingame = 1;
        break;
    }
    app->tradeAnim.options = app->args->options;
    app->tradeSequence = OverlayManager_New(&sGtsTradeSequenceTemplate, &app->tradeAnim, HEAP_ID_61);
    app->reloadGraphics = TRUE;
    return 2;
}

// After the trade sequence the received Pokemon may evolve by trade (kind 9; kinds 8 and 10 only when it is
// not the deposited Pokemon itself); then it is stored and the app goes to screen 7 (or back to screen 1).
int GtsTrade_Main(GtsApp *app, int state) {
    u32 kind;
    int method;
    int methodAfterCheck;
    Pokemon *mon;
    Pokemon *deposited;
    u16 species;
    int ret = 3;
    u32 receivedSpecies;
    u32 receivedPersonality;

    switch (app->state) {
    case 0:
        if (!OverlayManager_Run(app->tradeSequence)) {
            break;
        }
        OverlayManager_Delete(app->tradeSequence);
        kind = app->nextScreenArg;
        if (kind == ret + 6) { // kind 9, written against ret (matching)
            mon = GtsTrade_GetReceivedMon(app, kind);
            species = GetMonEvolution(NULL, mon, EVOCTX_TRADE, (u16)GetMonData(mon, MON_DATA_HELD_ITEM, NULL), &method);
            if (species != SPECIES_NONE) {
                GtsArgs *args = app->args;
                app->evolutionTask = EvolutionScene_New(NULL, mon, species, args->options, args->unk38, args->pokedex, args->bag, args->gameStats, method, 4, HEAP_ID_61);
                app->state = 1;
            } else {
                Gts_SetNextScreen(app, 1, 0);
                ret = 4;
            }
        } else if (kind == 8 || kind == 10) {
            mon = GtsTrade_GetReceivedMon(app, kind);
            deposited = AllocMonZeroed(HEAP_ID_61);
            GtsSave_GetStoredMon((void *)app->args->gtsSave, deposited);
            receivedSpecies = GetMonData(mon, MON_DATA_SPECIES, NULL);
            if (receivedSpecies != GetMonData(deposited, MON_DATA_SPECIES, NULL)) {
                goto checkEvolution;
            }
            receivedPersonality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
            if (receivedPersonality == GetMonData(deposited, MON_DATA_PERSONALITY, NULL)) {
                goto noEvolution;
            }
checkEvolution:
            species = GetMonEvolution(NULL, mon, EVOCTX_TRADE, (u16)GetMonData(mon, MON_DATA_HELD_ITEM, NULL), &methodAfterCheck);
            if (species != SPECIES_NONE) {
                GtsArgs *args = app->args;
                app->evolutionTask = EvolutionScene_New(NULL, mon, species, args->options, args->unk38, args->pokedex, args->bag, args->gameStats, methodAfterCheck, 4, HEAP_ID_61);
                app->state = 1;
            } else {
                Gts_SetNextScreen(app, 1, 0);
                ret = 4;
            }
            goto freeDeposited;
noEvolution:
            Gts_SetNextScreen(app, 1, 0);
            ret = 4;
freeDeposited:
            Heap_Free(deposited);
        } else {
            Gts_SetNextScreen(app, 1, 0);
            ret = 4;
        }
        break;
    case 1:
        if (EvolutionScene_IsDone(app->evolutionTask)) {
            EvolutionScene_Delete(app->evolutionTask);
            GtsTrade_StoreReceivedMon(app);
            reg_GX_DISPCNT &= 0xFFFF1FFF;
            Gts_SetNextScreen(app, 7, 12);
            ret = 4;
        }
        break;
    }
    return ret;
}

#pragma force_active reset

int GtsTrade_Exit(GtsApp *app, int state) {
    Heap_Free(app->depositedMon);
    Heap_Free(app->partnerProfile);
    Gts_SwitchScreen(app);
    return 1;
}

// The trade partner as a profile: the record's trainer name, game version and language.
PlayerProfile *GtsTrade_CreatePartnerProfile(GtsRecord *record) {
    PlayerProfile *profile = PlayerProfile_New(HEAP_ID_61);
    PlayerProfile_Init(profile);
    Save_Profile_PlayerName_Set(profile, record->trainerName);
    PlayerProfile_SetVersion(profile, record->version);
    PlayerProfile_SetLanguage(profile, record->language);
    return profile;
}

Pokemon *GtsTrade_GetReceivedMon(GtsApp *app, int kind) {
    if (kind == 9) {
        return &app->searchResults[app->selectedResult].mon;
    }
    if (kind == 10) {
        return &app->record.mon;
    }
    if (kind == 8) {
        return &app->record.mon;
    }
    GF_AssertFail();
    return NULL;
}

// Puts the received Pokemon where the offered one was: its party slot, or (after deleting the offered
// Pokemon from its box) the first empty box slot.
void GtsTrade_StoreReceivedMon(GtsApp *app) {
    int kind = app->nextScreenArg;
    Pokemon *mon = GtsTrade_GetReceivedMon(app, kind);
    if (app->offeredBox == GTS_BOX_PARTY) {
        CopyPokemonToPokemon(mon, Party_GetMonByIndex(app->args->party, app->offeredSlot));
    } else {
        int box = 0;
        int slot = 0;
        PCStorage_DeleteBoxMonByIndexPair(app->args->pcStorage, app->offeredBox, app->offeredSlot);
        PCStorage_FindFirstEmptySlot(app->args->pcStorage, &box, &slot);
        PCStorage_PlaceMonInBoxFirstEmptySlot(app->args->pcStorage, box, Mon_GetBoxMon(mon));
    }
}
