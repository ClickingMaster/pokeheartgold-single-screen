#ifndef TO56_R3_W18_PRIVATE_H
#define TO56_R3_W18_PRIVATE_H
#include "global.h"
typedef struct Ov08WorkR3W18 {
    u8 pad[0x2077];
    u8 mode : 4;
    u8 upper : 4;
    u8 pad2078[2];
    u8 selection;
} Ov08WorkR3W18;
typedef struct Ov08OwnerR3W18 {
    void *unk0;
    u8 pad4[8];
    int heapId;
    u8 pad10[0x12];
    u16 item;
} Ov08OwnerR3W18;
extern void BattlePartyMenu_ChangePage(void *work, int state);
extern void BattlePartyMenu_ShowMoveIcons(void *work);
extern s32 GetItemAttr(u16 item, u16 attr, int heapID);
extern void BattlePartyMenu_ShowPageSprites(void *work, u8 selection);
extern void BattlePartyMenu_PrintPageText(void *work, u8 selection);
extern void BattlePartyMenu_DrawPageButtons(void *work, u8 selection);
extern void BattlePartyMenu_DrawExpBar(void *work, u8 selection);
int BattlePartyMenu_ShowLearnMovePage(Ov08WorkR3W18 *work);
int BattlePartyMenu_ShowForgetMovePage(Ov08WorkR3W18 *work);
int BattlePartyMenu_ShowRestorePPPage(Ov08OwnerR3W18 **work);
int BattlePartyMenu_RefreshMonPage(Ov08WorkR3W18 *work);
#endif
