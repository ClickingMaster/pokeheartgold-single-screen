#ifndef POKEHEARTGOLD_WIFI_CLUB_COMM_TABLE_H
#define POKEHEARTGOLD_WIFI_CLUB_COMM_TABLE_H

#include "global.h"

#include "overlay_44.h"

typedef s32 (*WifiClubCommSizeFunc)(void);
typedef void (*WifiClubCommRecvFunc)(void *, void *, String *, WifiClubGameEntry *);

typedef struct WifiClubCommCommand {
    WifiClubCommRecvFunc func1;
    WifiClubCommSizeFunc func2;
    WifiClubCommSizeFunc func3;
} WifiClubCommCommand;

const WifiClubCommCommand *WifiClubComm_GetCommandTable(void);
int WifiClubComm_GetCommandCount(void);

#endif // POKEHEARTGOLD_WIFI_CLUB_COMM_TABLE_H
