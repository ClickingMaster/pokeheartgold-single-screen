#ifndef POKEHEARTGOLD_UNK_0203AC88_H
#define POKEHEARTGOLD_UNK_0203AC88_H

#include "global.h"

#include "overlay_44.h"

typedef s32 (*UnkFuncPtr_0203AC88)(void);
typedef void (*UnkFuncPtr_0203AC88_void)(void *, void *, String *, WifiClubGameEntry *);

typedef struct WifiClubCommCommand {
    UnkFuncPtr_0203AC88_void func1;
    UnkFuncPtr_0203AC88 func2;
    UnkFuncPtr_0203AC88 func3;
} WifiClubCommCommand;

const WifiClubCommCommand *WifiClubComm_GetCommandTable(void);
int WifiClubComm_GetCommandCount(void);

#endif // POKEHEARTGOLD_UNK_0203AC88_H
