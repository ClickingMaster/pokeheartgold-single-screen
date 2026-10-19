#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_internal.h"

extern u8 gPPWLobbyPlayersVtable[];
void *PPWLobbyPlayerSet_Destroy(u32 *playerSet);

PPWLobbyPlayer *PPWLobbyPlayer_Destroy(PPWLobbyPlayer *player) {
    LobbyCharVector_Destroy(&player->userProfile);
    return player;
}

PPWLobbyPlayers *PPWLobbyPlayers_Destroy(PPWLobbyPlayers *players) {
    players->vtable = gPPWLobbyPlayersVtable;
    PPWLobbyPlayers_Clear(players);
    LobbyCharVector_Destroy(&players->self.userProfile);
    PPWLobbyPlayerSet_Destroy(players->playerSet);
    return players;
}
