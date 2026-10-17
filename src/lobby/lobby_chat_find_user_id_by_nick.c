#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

typedef struct {
    u8 b[12];
} String;

typedef struct {
    int id;
    String value;
} Pair;

typedef struct {
    u8 unk_00[0x70];
    u32 userMap;
    u8 userMapHead[1];
} Work;

extern int LobbyChat_GetUserIdFromNick(const String *);
extern void LobbyUserMap_Find(void **, void *, const void *);
extern int LobbyString_Equals(const void *, const void *);
extern void LobbyString_InitFromCStr(String *, const char *);
extern char sStr_Empty_18[];
extern void LobbyString_InitCopy(String *, const void *);
int LobbyChat_FindUserIdByNick(Work *, const String *);
void LobbyChat_GetUserNick(String *, Work *, int, ...);

// The ID in a nick only counts while the user map still has that nick for it.
int LobbyChat_FindUserIdByNick(Work *manager, const String *nick) {
    int id;
    void *node;
    id = LobbyChat_GetUserIdFromNick(nick);
    if (id == -1) {
        return -1;
    }
    LobbyUserMap_Find(&node, &manager->userMap, &id);
    if (node == manager->userMapHead) {
        return -1;
    }
    if ((BOOL)(LobbyString_Equals((u8 *)node + 0x10, nick) == 0) != FALSE) {
        return -1;
    }
    return id;
}

void LobbyChat_GetUserNick(String *out, Work *manager, int userId, ...) {
    void *node;
    LobbyUserMap_Find(&node, &manager->userMap, &userId);
    if (node == manager->userMapHead) {
        LobbyString_InitFromCStr(out, sStr_Empty_18);
    } else {
        LobbyString_InitCopy(out, (u8 *)node + 0x10);
    }
}
