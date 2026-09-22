#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_0222057C(void *arg) {
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

void ov08_022205E0(void *work) {
    ov08_0222061C(work);
    ov08_02220668(work);
    ov08_02220750(work);
    ov08_02220800(work);
    ov08_02220878(work);
    ov08_02220928(work);
    ov08_02220A28(work);
    ov08_0222162C(work);
    GfGfx_EngineBTogglePlanes(0x10, TRUE);
}
