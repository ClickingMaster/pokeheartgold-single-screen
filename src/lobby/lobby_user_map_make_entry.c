#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

#define W(p, o) (*(int *)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))

void LobbyUserMap_MakeEntry(IntStrPair *p, int a, const Str *b) {
    W(p, 0) = a;
    LobbyString_InitCopy(A(p, 4), b);
}

int LobbyChat_AreChannelUsersEnumerated(LobbyChatManager *p, Str *key) {
    Iter n;
    LobbyChannelMap_Find(&n, A(p, 0x80), key);
    if (n.current == A(p, 0x84)) {
        return 0;
    }
    return W(n.current, 24);
}

int LobbyChat_SetChannelUsersEnumerated(LobbyChatManager *p, Str *key) {
    Iter n;
    LobbyChannelMap_Find(&n, A(p, 0x80), key);
    if (n.current == A(p, 0x84)) {
        return 0;
    }
    W(n.current, 24) = 1;
    return 1;
}

int LobbyChat_GetChannelUserLimit(LobbyChatManager *p, Str *key) {
    Iter n;
    LobbyChannelMap_Find(&n, A(p, 0x80), key);
    if (n.current == A(p, 0x84)) {
        return -1;
    }
    return W(n.current, 40);
}
