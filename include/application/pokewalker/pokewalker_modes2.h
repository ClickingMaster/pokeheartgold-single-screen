#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_MODES2_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_MODES2_H

#include "application/pokewalker/pokewalker_app.h"

#include "pokemon.h"
#include "pokemon_storage_system.h"

// A PC box or party Pokemon as the box selector (sub_020932E0..) reads it through its callbacks.
typedef struct PokewalkerBoxMon {
    u32 species;
    u32 personality;
    u16 isEgg;
    u16 form;
    u16 heldItem;
    u16 unk_0E;
    u16 isShiny;
    u16 gender;
    u16 level;
    u16 markings;
    u16 nickname[POKEMON_NAME_LENGTH + 2];
} PokewalkerBoxMon; // size=0x30

// What the box selector shows about the Pokemon under the cursor (ov112_021F04DC).
typedef struct PokewalkerSelectMonInfo {
    u32 personality;
    u16 species;
    u8 isShiny;
    u8 form;
    u8 gender;
    u8 state; // 0 = no Pokemon, 2 = Pokemon
    u16 nickname[POKEMON_NAME_LENGTH + 1];
    u8 markings[6];
    u16 heldItem;
    u8 level;
} PokewalkerSelectMonInfo; // size=0x2C

// PokewalkerApp seen from selectMonState (0x1D764): the PC box selection (mode 7) and the chosen Pokemon's
// species, nickname, form and gender (PokewalkerApp strollMonSpecies .. strollMonGender).
typedef struct PokewalkerSelectMonWork {
    int state;
    void *boxSelector;
    void *boxDisplay;
    u32 box; // 18 = party
    u32 slot;
    u32 cancelled;
    u16 species;
    u16 nickname[POKEMON_NAME_LENGTH + 1];
    u8 form;
    u8 isShiny;
    u8 gender;
} PokewalkerSelectMonWork;

// Arguments of the Pokewalker event log app (mode 8): PokewalkerApp eventLogSaveData / eventLogRecords.
typedef struct PokewalkerEventLogArgs {
    SaveData *saveData;
    PokewalkerIrRecord *records; // the 24 event log entries read from the Pokewalker
} PokewalkerEventLogArgs;

// Box selector (ARM9, no header yet).
void *sub_020932E0(enum HeapID heapId, int count, int flag);
void sub_02093354(void *boxSelector);
void sub_02093440(void *boxSelector, BgConfig *bgConfig, PCStorage *storage, Party *party, u32 a4, u32 a5, BOOL a6, u32 a7, void *getMon, void *getBoxName, void *selection, void *onCursorMove, void *arg);
void sub_02093594(void *boxSelector);
int sub_020935E0(void *boxSelector);
void sub_02093A40(void *boxSelector);

// Modes of the mode table in pokewalker_app.c: enter / update / exit.
int PokewalkerApp_LinkSave_Enter(PokewalkerApp *app);
int PokewalkerApp_LinkSave_Update(PokewalkerApp *app);
int PokewalkerApp_LinkSave_Exit(PokewalkerApp *app);
int PokewalkerApp_LinkSave_PrintSaving(PokewalkerApp *app);
int PokewalkerApp_LinkSave_WaitSaving(PokewalkerApp *app);
int PokewalkerApp_LinkSave_Save(PokewalkerApp *app);
int PokewalkerApp_LinkSave_EndSave(PokewalkerApp *app);
int PokewalkerApp_LinkSave_SetNextMode(PokewalkerApp *app);
int PokewalkerApp_LinkSave_PrintSaving2(PokewalkerApp *app);
int PokewalkerApp_LinkSave_WaitSaving2(PokewalkerApp *app);
int PokewalkerApp_LinkSave_Save2(PokewalkerApp *app);
int PokewalkerApp_LinkSave_EndSave2(PokewalkerApp *app);
int PokewalkerApp_LinkSave_SetNextMode2(PokewalkerApp *app);
int PokewalkerApp_LinkSave_End(PokewalkerApp *app);
int PokewalkerApp_SelectMon_Enter(PokewalkerApp *app);
int PokewalkerApp_SelectMon_Update(PokewalkerApp *app);
int PokewalkerApp_SelectMon_Exit(PokewalkerApp *app);
void PokewalkerApp_SelectMon_SetGxBanks(void);
void PokewalkerApp_SelectMon_SetGraphicsModes(BgConfig *bgConfig);
void PokewalkerApp_SelectMon_InitObjTransfer(void);
void PokewalkerApp_SelectMon_InitOam(PokewalkerSelectMonWork *work);
BOOL PokewalkerApp_SelectMon_GetBoxMon(PCStorage *storage, u32 box, u32 slot, PokewalkerBoxMon *mon);
BOOL PokewalkerApp_SelectMon_GetBoxMonIcon(PCStorage *storage, u32 box, u32 slot, PokewalkerBoxMon *mon);
void PokewalkerApp_SelectMon_GetBoxName(String *dest, PCStorage *storage, u32 box);
void PokewalkerApp_SelectMon_VBlankCB(PokewalkerSelectMonWork *work);
void PokewalkerApp_SelectMon_StoreSelection(PokewalkerSelectMonWork *work, PokewalkerApp *app);
void PokewalkerApp_SelectMon_ShowMonInfo(int box, int slot, PokewalkerApp *app);
int PokewalkerApp_EventLog_Enter(PokewalkerApp *app);
int PokewalkerApp_EventLog_Update(PokewalkerApp *app);
int PokewalkerApp_EventLog_Exit(PokewalkerApp *app);
int PokewalkerApp_WaitKey_Enter(PokewalkerApp *app);
int PokewalkerApp_WaitKey_Update(PokewalkerApp *app);
int PokewalkerApp_WaitKey_Exit(PokewalkerApp *app);
int PokewalkerApp_StrollReport_Enter(PokewalkerApp *app);
int PokewalkerApp_StrollReport_Update(PokewalkerApp *app);
int PokewalkerApp_StrollReport_Exit(PokewalkerApp *app);

// Box display of the PC box selection, Pokewalker event log app.
void *ov112_021F039C(enum HeapID heapId);
void ov112_021F03BC(void *boxDisplay, BgConfig *bgConfig);
void ov112_021F04DC(void *boxDisplay, PokewalkerSelectMonInfo *info);
void ov112_021F050C(void *boxDisplay);
void ov112_021F051C(void *boxDisplay);
BOOL PokewalkerApp_IsEventLogInvalid(PokewalkerIrRecord *records);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_MODES2_H
