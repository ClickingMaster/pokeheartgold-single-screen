#ifndef POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_MODES2_H
#define POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_MODES2_H

#include "application/pokewalker/pokewalker_app.h"

#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"

// A PC box or party Pokemon as the box selector (BoxMonSelector_New..) reads it through its callbacks.
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

// Sprites of the box display: the six marking sprites and the picture of the Pokemon (sub screen).
typedef struct PokewalkerBoxDisplaySprites {
    enum HeapID heapId;
    SpriteList *spriteList;                          // 0x004
    G2dRenderer renderer;                            // 0x008
    GF_2DGfxResMan *resMan[GF_GFX_RES_TYPE_MAX];     // 0x130
    SpriteResource *resObjs[2][GF_GFX_RES_TYPE_MAX]; // 0x148: 0 = markings, 1 = Pokemon picture
    u8 unk_178[0x24];
    Sprite *markingSprites[6]; // 0x19C
    Sprite *monSprite;         // 0x1B4
    void *monCharData;         // 0x1B8, 80x80 4bpp
    u16 monPalette[16];        // 0x1BC
    u32 monCharLocation;       // 0x1DC, sub screen OBJ VRAM
    u32 monPlttLocation;       // 0x1E0
} PokewalkerBoxDisplaySprites; // size=0x1E4

// Sub screen of the PC box selection (mode 7): the Pokemon under the cursor with its picture, markings, nickname,
// species, gender, level and held item.
typedef struct PokewalkerBoxDisplay {
    enum HeapID heapId;
    BgConfig *bgConfig;
    Window windows[6];
    MsgData *msgData; // 0x68
    MessageFormat *msgFormat;
    String *string;
    PokewalkerBoxDisplaySprites sprites; // 0x74
} PokewalkerBoxDisplay;                  // size=0x258

// Box selector (ARM9, no header yet).
void *BoxMonSelector_New(enum HeapID heapId, int count, int flag);
void BoxMonSelector_Delete(void *boxSelector);
void BoxMonSelector_Init(void *boxSelector, BgConfig *bgConfig, PCStorage *storage, Party *party, u32 unused11, u32 unused12, BOOL canCancel, u32 initialPage, void *getMon, void *getBoxName, void *selection, void *onCursorMove, void *arg);
void BoxMonSelector_VBlank(void *boxSelector);
int BoxMonSelector_Run(void *boxSelector);
void BoxMonSelector_RenderSprites(void *boxSelector);

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

// States of mode 10 (sStrollReportStates).
int PokewalkerApp_StrollReport_Start(PokewalkerApp *app);
int PokewalkerApp_StrollReport_ShowResults(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitResultsFade(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintMonArrived(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintLevelUp(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintWatts(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintRouteFound(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintRouteAvailable(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintMilestone(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintStarfBerry(PokewalkerApp *app);
int PokewalkerApp_StrollReport_FadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitFadeOut(PokewalkerApp *app);
int PokewalkerApp_CountReceivedMons(PokewalkerIrStatus *status, PokewalkerIrBlocks *blocks);
int PokewalkerApp_StrollReport_ShowCaughtMons(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitCaughtMonsFade(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintMonsSent(PokewalkerApp *app);
int PokewalkerApp_StrollReport_CaughtMonsFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitCaughtMonsFadeOut(PokewalkerApp *app);
void PokewalkerApp_AddItemToList(u16 *items, u16 item);
int PokewalkerApp_CountReceivedItems(PokewalkerApp *app);
int PokewalkerApp_StrollReport_ShowItems(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitItemsFade(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintItemsPlaced(PokewalkerApp *app);
int PokewalkerApp_StrollReport_ItemsFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitItemsFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_GiftsFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitGiftsFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_ShowGifts(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitGiftsFade(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintGiftWatts(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintGiftRouteFound(PokewalkerApp *app);
int PokewalkerApp_StrollReport_GiftsDoneFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitGiftsDoneFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_RecordsFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitRecordsFadeOut(PokewalkerApp *app);
int PokewalkerApp_StrollReport_ShowRecords(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitRecordsFade(PokewalkerApp *app);
int PokewalkerApp_StrollReport_PrintRecords(PokewalkerApp *app);
int PokewalkerApp_StrollReport_WaitMessage(PokewalkerApp *app);
int PokewalkerApp_StrollReport_End(PokewalkerApp *app);

// Sub screen of the PC box selection (mode 7).
PokewalkerBoxDisplay *PokewalkerBoxDisplay_New(enum HeapID heapId);
void PokewalkerBoxDisplay_Init(PokewalkerBoxDisplay *display, BgConfig *bgConfig);
void PokewalkerBoxDisplay_ShowMon(PokewalkerBoxDisplay *display, PokewalkerSelectMonInfo *info);
void PokewalkerBoxDisplay_Update(PokewalkerBoxDisplay *display);
void PokewalkerBoxDisplay_Free(PokewalkerBoxDisplay *display);
void PokewalkerBoxDisplay_DeleteSprites(PokewalkerBoxDisplaySprites *sprites);
void PokewalkerBoxDisplay_LoadBgGraphics(BgConfig *bgConfig, enum HeapID heapId);
void PokewalkerBoxDisplay_InitBgs(BgConfig *bgConfig);
void PokewalkerBoxDisplay_PrintMonInfo(PokewalkerBoxDisplay *display, PokewalkerSelectMonInfo *info);
void PokewalkerBoxDisplay_ShowMonSprites(PokewalkerBoxDisplay *display, PokewalkerSelectMonInfo *info);
void PokewalkerBoxDisplay_HideSprites(PokewalkerBoxDisplay *display);
void PokewalkerBoxDisplay_InitSprites(PokewalkerBoxDisplaySprites *sprites);
void PokewalkerBoxDisplay_CreateSprites(PokewalkerBoxDisplaySprites *sprites);
Sprite *PokewalkerBoxDisplay_CreateSprite(PokewalkerBoxDisplaySprites *sprites, int resId, int drawPriority, int priority, int x, int y, int anim, BOOL visible);
void PokewalkerBoxDisplay_GetMonPictureLocation(PokewalkerBoxDisplaySprites *sprites);
void PokewalkerBoxDisplay_LoadMonPicture(PokewalkerBoxDisplaySprites *sprites, PokewalkerSelectMonInfo *info);
void PokewalkerBoxDisplay_TransferMonPicture(PokewalkerBoxDisplaySprites *sprites);

// Shared by the box display and the Pokewalker event log app.
void PokewalkerApp_InitSpriteTemplate(SpriteTemplate *template, SpriteList *spriteList, const SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen, enum HeapID heapId);

// Pokewalker event log app.
BOOL PokewalkerApp_IsEventLogInvalid(PokewalkerIrRecord *records);

#endif // POKEHEARTGOLD_APPLICATION_POKEWALKER_POKEWALKER_MODES2_H
