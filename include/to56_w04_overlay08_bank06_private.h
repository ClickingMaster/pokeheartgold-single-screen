#ifndef TO56_W04_OVERLAY08_BANK06_PRIVATE_H
#define TO56_W04_OVERLAY08_BANK06_PRIVATE_H

#include "global.h"

#include "bg_window.h"
#include "palette.h"

typedef struct Ov08W04State {
    u8 pad0000[0x1E4];
    void *bgConfig;
    void *paletteData;
    u8 pad01EC[0x1F60 - 0x1EC];
    u8 paletteCopy[0x40];
    u8 pad1FA0[4];
    void *printer;
    void *msgData;
    void *msgFormat;
    void *string;
    u8 pad1FB4[0x207C - 0x1FB4];
    u8 state;
    u8 pad207D[0x2088 - 0x207D];
    void *smallState;
} Ov08W04State;

typedef struct Ov08W04Root {
    u8 pad[0xC];
    u32 heapId;
} Ov08W04Root;

extern const struct GraphicsModes sBattlePartyGraphicsModes;
extern const struct BgTemplate sBattlePartyBgTemplateSub3;
extern const struct BgTemplate sBattlePartyBgTemplateSub2;
extern const struct BgTemplate sBattlePartyBgTemplateSub1;
extern const struct BgTemplate sBattlePartyBgTemplateSub0;
extern const u8 sBattlePartyPartyListHitboxes[];
extern const u8 sBattlePartyMonMenuHitboxes[];
extern const u8 sBattlePartySummaryHitboxes[];
extern const u8 sBattlePartyCheckMovesHitboxes[];
extern const u8 sBattlePartyMoveDetailHitboxes[];

void DestroyMsgData(void *);
void MessagePrinter_Delete(void *);
void MessageFormat_Delete(void *);
void String_Delete(void *);
void *NewMsgDataFromNarc(int, int, int, u32);
void *MessagePrinter_New(int, int, int, u32);
void *MessageFormat_New(u32);
void *String_New(int, u32);
void FontID_Release(int);
void DestroySysTaskAndEnvironment(void *);
void BattlePartyMenu_FreeSprites(void *);
void BattlePartyMenu_RemoveWindows(void *);
void BattlePartyMenu_FreeText(Ov08W04State *);
u8 BattleDpadCursor_IsActive(void *);
void BattleDpadCursor_Free(void *);
int BattlePartyMenu_CheckTouchHitbox(void *, const void *);
int BattleDpadCursor_HandleInput(void *);
BOOL BattlePartyMenu_GetSlotType(void *, int);
void BattlePartyMenu_DeactivateCursor(void *);

int BattlePartyMenu_StartFadeOut(Ov08W04State *work);
int BattlePartyMenu_Exit(void *task, Ov08W04State *work);
void BattlePartyMenu_InitBgs(Ov08W04State *work);
void BattlePartyMenu_FreeBgs(void *bgConfig);
void BattlePartyMenu_InitText(Ov08W04State *work);
u8 BattlePartyMenu_GetMonMenuInput(void *work);
u8 BattlePartyMenu_GetSummaryInput(void *work);
u8 BattlePartyMenu_GetCheckMovesInput(void *work);
u8 BattlePartyMenu_GetMoveDetailInput(void *work);
BOOL BattlePartyMenu_HandlePartyListSelection(void *work);

#endif
