#include "to58_overlay08_bank14_2_private.h"
extern void sub_0200CE7C(void *, int, int, int, int, void *, int, int);

void BattlePartyMenu_PrintMonLevel(void *arg, int window, int mon, int x, u8 y) {
    Ov08R2W11Work *work = arg;
    int offset = window << 4;
    struct LevelBits { u8 level : 7; u8 flag : 1; };
    u32 level = ((struct LevelBits *)((u8 *)work + mon * 0x50 + 0x1A))->level;
    sub_0200CE7C(*(void **)((u8 *)work + 0x1FA4), 1, level, 3, 0, (u8 *)work->windowArray + offset, x + 8, y);
    ScheduleWindowCopyToVram((Window *)((u8 *)work->windowArray + offset));
}
