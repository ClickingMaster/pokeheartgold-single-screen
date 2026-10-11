#ifndef TO56_R2W11_PRIVATE_H
#define TO56_R2W11_PRIVATE_H
#include "global.h"

#include "constants/moves.h"

#include "bg_window.h"
typedef struct Ov08R2W11Work {
    void *root;
    u8 pad_0004[0x1FA4];
    void *msgData;
    u8 pad_1FAC[4];
    void *message;
    void *spriteManager;
    void *sprites[38];
    Window windows[2];
    void *windowArray;
    u8 windowCount;
    u8 pad_2075[6];
    u8 printer;
} Ov08R2W11Work;
extern void BattlePartyMenu_PrintPartyListPage(void *);
extern void BattlePartyMenu_PrintMonMenuPage(void *);
extern void BattlePartyMenu_PrintSummaryPage(void *);
extern void BattlePartyMenu_PrintCheckMovesPage(void *);
extern void BattlePartyMenu_PrintMoveDetailPage(void *);
extern void BattlePartyMenu_PrintRestorePPPage(void *);
extern void BattlePartyMenu_PrintLearnMovePage(void *);
extern void BattlePartyMenu_PrintForgetMovePage(void *);
extern void BattlePartyMenu_PrintContestForgetMovePage(void *);
extern void BattlePartyMenu_PrintMonLevel(void *, int, int, int, int);
extern void BattlePartyMenu_UpdateMoveListPP(void *, void *, int);
extern void DrawFrameAndWindow2(void *, BOOL, int, int);
extern void BattlePartyMenu_PrintMessage(void *);
extern void TextFlags_SetCanABSpeedUpPrint(BOOL);
extern int BattleSystem_GetTextFrameDelay(void *);
extern int AddTextPrinterParameterized(void *, int, void *, int, int, int, int);
extern void BattlePartyMenu_CreateSpriteManager(void *);
extern void BattlePartyMenu_LoadMonIconResources(void *);
extern void BattlePartyMenu_LoadStatusIconResources(void *);
extern void BattlePartyMenu_LoadTypeIconResources(void *);
extern void BattlePartyMenu_LoadHeldItemIconResources(void *);
extern void ov08_02220928(void *);
extern void BattlePartyMenu_CreateSprites(void *);
extern void BattlePartyMenu_CreateCursor(void *);
extern void *BattlePartyMenu_CreateSprite(void *, int);
extern void BattlePartyMenu_SetMonIconPalettes(void *);
extern void *BattleSystem_GetSpriteSystem(void *);
extern void Sprite_DeleteAndFreeResources(void *);
extern void BattlePartyMenu_FreeCursor(void *);
extern void SpriteSystem_FreeResourcesAndManager(void *, void *);
extern void *NewString_ReadMsgData(void *, int);
extern void String_Delete(void *);
extern int AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
extern u32 FontID_String_GetWidth(int, void *, int);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void DestroyMsgData(void *);
void BattlePartyMenu_BufferEmbargoMessage(void *);
void BattlePartyMenu_RemoveWindows(void *);
void BattlePartyMenu_PrintPageText(void *, int);
void BattlePartyMenu_PrintPartyMonLevel(void *, int);
void BattlePartyMenu_UpdateMovePP(void *, int, int);
void BattlePartyMenu_ShowMessage(void *);
void BattlePartyMenu_PrintMessage(void *);
void BattlePartyMenu_InitSprites(void *);
void BattlePartyMenu_CreateSprites(void *);
void BattlePartyMenu_FreeSprites(void *);
void BattlePartyMenu_PrintPPLabel(void *, int, int, int);
void BattlePartyMenu_PrintPrompt(void *, int);
void BattlePartyMenu_PrintButtonLabel(void *, int, int);
void BattlePartyMenu_PrintAccuracyLabel(void *, int);
void BattlePartyMenu_PrintPowerLabel(void *, int);
void BattlePartyMenu_PrintCategoryLabel(void *, int);
void BattlePartyMenu_PrintMoveCategory(void *, int, int);
void BattlePartyMenu_PrintForgetButtonLabel(void *, int);
extern void BattlePartyMenu_PrintMonHP(void *, int, int, int, int);
extern void BattlePartyMenu_DrawHPBar(void *, int, int, int, int);
void BattlePartyMenu_PrintMonHPAndBar(void *, int);
void BattlePartyMenu_PrintMoveDescription(void *, int, int);
extern void *BattleSystem_GetPartyMon(void *, int, int);
extern void *Mon_GetBoxMon(void *);
extern void BufferBoxMonNickname(void *, u32, void *);
extern void BufferMoveName(void *, u32, u32);
extern void StringExpandPlaceholders(void *, void *, void *);
#endif
