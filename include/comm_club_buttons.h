#ifndef POKEHEARTGOLD_COMM_CLUB_BUTTONS_H
#define POKEHEARTGOLD_COMM_CLUB_BUTTONS_H

#include "bg_window.h"
#include "sys_task.h"

// Shared with the Communication Club (overlay_03 communication_club.c, CommClubManager+0x9C).
typedef struct CommClubButtonsArgs {
    u32 *msgIds;     // msg_0191 labels, one per button
    u8 count;        // 1 or 2 buttons
    u8 closeRequest; // set by the club: 1 = clear the buttons, 2 = clear and hold touch input
    u16 selection;   // touched button, 0xFFFF = none
} CommClubButtonsArgs;

SysTask *CommClubButtons_Init(BgConfig *bgConfig, void *a1, void *a2, CommClubButtonsArgs *args);
void CommClubButtons_Exit(void *a0, SysTask *task);
BOOL CommClubButtons_IsExitDone(void);

#endif // POKEHEARTGOLD_COMM_CLUB_BUTTONS_H
