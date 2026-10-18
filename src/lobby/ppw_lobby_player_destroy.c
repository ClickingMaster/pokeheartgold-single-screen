#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

#pragma warn_uninitializedvar off
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))

extern u8 gPPWLobbyPlayersVtable[];
extern int PPWLobbyPlayerSet_Destroy();
extern int LobbyCharVector_Destroy();
extern int PPWLobbyPlayers_Clear();

u8 *PPWLobbyPlayer_Destroy(u8 *p) {
    LobbyCharVector_Destroy(p + 24);
    return p;
}

u8 *PPWLobbyPlayers_Destroy(u8 *p) {
    P(p, 0) = gPPWLobbyPlayersVtable;
    PPWLobbyPlayers_Clear(p);
    LobbyCharVector_Destroy(p + 40);
    PPWLobbyPlayerSet_Destroy(p + 4);
    return p;
}
