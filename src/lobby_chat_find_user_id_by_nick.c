#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <stdarg.h>

typedef struct {
    u8 b[12];
} String;

typedef struct {
    int id;
    String value;
} Pair;

typedef struct {
    u8 p[0x70];
    u32 tree;
    u8 sentinel[1];
} Work;

extern int LobbyChat_GetUserIdFromNick(const String *);
extern void LobbyUserMap_Find(void **, void *, const void *);
extern int LobbyString_Equals(const void *, const void *);
extern void LobbyString_InitFromCStr(String *, const char *);
extern char sStr_Empty_18[];
extern void LobbyString_InitCopy(String *, const void *);
extern void LobbyUserMap_EraseKey(void *, int *);
extern void LobbyUserMap_MakeEntry(Pair *, int, const String *);
extern void LobbyUserMap_Insert(void *, void *, const Pair *);
extern void LobbyString_Destroy(String *);
int LobbyChat_FindUserIdByNick(Work *, const String *);
void LobbyChat_GetUserNick(String *, Work *, int, ...);
int LobbyChat_RenameUser(Work *, const String *, const String *);

int LobbyChat_FindUserIdByNick(Work *w, const String *key) {
    int id;
    void *node;
    id = LobbyChat_GetUserIdFromNick(key);
    if (id == -1) {
        return -1;
    }
    LobbyUserMap_Find(&node, &w->tree, &id);
    if (node == w->sentinel) {
        return -1;
    }
    if ((BOOL)(LobbyString_Equals((u8 *)node + 0x10, key) == 0) != FALSE) {
        return -1;
    }
    return id;
}

void LobbyChat_GetUserNick(String *out, Work *w, int id, ...) {
    void *node;
    LobbyUserMap_Find(&node, &w->tree, &id);
    if (node == w->sentinel) {
        LobbyString_InitFromCStr(out, sStr_Empty_18);
    } else {
        LobbyString_InitCopy(out, (u8 *)node + 0x10);
    }
}
