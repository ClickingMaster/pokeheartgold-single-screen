#ifndef TO56_W21_PRIVATE_H
#define TO56_W21_PRIVATE_H

#define FillBgTilemapRect Tracked_FillBgTilemapRect
#include "global.h"

#include "filesystem.h"
#include "unk_0208805C.h"
#undef FillBgTilemapRect

typedef struct Ov08Entry {
    u8 pad00[0x16];
    u8 percent : 7;
    u8 percentHigh : 1;
    u8 pad17[4];
    u8 padFlags : 7;
    u8 disabled : 1;
    u32 hp;
    u32 baseHp;
    u32 maxHp;
    u8 pad28[0x28];
} Ov08Entry;

extern BOOL BattlePartyMenu_IsDoubleBattle(void *work);
extern BOOL BattlePartyMenu_IsMultiBattle(void *work);
extern void BattlePartyMenu_ShowPageSprites(void *work, int state);
extern void BattlePartyMenu_RemovePageWindows(void *work);
extern void BattlePartyMenu_AddPageWindows(void *work, int state);
extern void BattlePartyMenu_PrintPageText(void *work, int state);
extern void BattlePartyMenu_SetPageCursor(void *work, int state);
extern void BattlePartyMenu_DrawPageButtons(void *work, int state);
extern void BattlePartyMenu_LoadPagePalette(void *work, int state);
extern BOOL NNS_G2dGetUnpackedScreenData(void *file, NNSG2dScreenData **screenData);
extern void FillBgTilemapRect(BgConfig *, int, int, int, int, int, int, int);
extern const u8 sBattlePartyMultiBattleOrder[6];
extern const u32 sBattlePartyPageScreenFiles[];

int BattlePartyMenu_GetSlotType(u8 *work, int index);
u8 BattlePartyMenu_FindNextMon(u8 *work, int index, int delta);
void BattlePartyMenu_DrawExpBar(u8 *work, int state);
void BattlePartyMenu_SetExpBarTile(u8 *work, int fill, u16 x, int y);
void ov08_0221D77C(u8 *work, int fill, u8 row);
void ov08_0221D81C(u8 *work);
void BattlePartyMenu_ChangePage(u8 *work, int state);
void BattlePartyMenu_LoadPageTilemaps(u8 *work, int state);
u8 BattlePartyMenu_IsSelectedMonEgg(u8 *work);

#endif
