#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_BufferEmbargoMessage(void *arg) {
    Ov08R2W11Work *work = arg;
    u8 *root = work->root;
    u8 *battler = root + root[0x11];
    void *mon = BattleSystem_GetPartyMon(*(void **)(root + 8), *(int *)(root + 0x28), battler[0x2C]);
    void *string = NewString_ReadMsgData(work->msgData, 0x5F);
    BufferBoxMonNickname(*(void **)((u8 *)work + 0x1FAC), 0, Mon_GetBoxMon(mon));
    BufferMoveName(*(void **)((u8 *)work + 0x1FAC), 1, MOVE_EMBARGO);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0x1FAC), *(void **)((u8 *)work + 0x1FB0), string);
    String_Delete(string);
}

void BattlePartyMenu_InitSprites(void *work) {
    BattlePartyMenu_CreateSpriteManager(work);
    BattlePartyMenu_LoadMonIconResources(work);
    BattlePartyMenu_LoadStatusIconResources(work);
    BattlePartyMenu_LoadTypeIconResources(work);
    BattlePartyMenu_LoadHeldItemIconResources(work);
    ov08_02220928(work);
    BattlePartyMenu_CreateSprites(work);
    BattlePartyMenu_CreateCursor(work);
    GfGfx_EngineBTogglePlanes(0x10, TRUE);
}
