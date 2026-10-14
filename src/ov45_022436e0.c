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
extern char ov45_022553A8[];
extern void LobbyString_InitCopy(String *, const void *);
extern void LobbyUserMap_EraseKey(void *, int *);
extern void LobbyUserMap_MakeEntry(Pair *, int, const String *);
extern void LobbyUserMap_Insert(void *, void *, const Pair *);
extern void LobbyString_Destroy(String *);
int LobbyChat_FindUserIdByNick(Work *, const String *);
void LobbyChat_GetUserNick(String *, Work *, int, ...);
int LobbyChat_RenameUser(Work *, const String *, const String *);

int LobbyChat_RenameUser(Work *w, const String *key, const String *value) {
    String temp;
    Pair pair;
    Pair entry;
    int id;

    struct {
        void *node;
        u8 success;
    } result;

    int ok;
    id = LobbyChat_FindUserIdByNick(w, key);
    LobbyUserMap_EraseKey(&w->tree, &id);
    LobbyString_InitCopy(&temp, value);
    LobbyUserMap_MakeEntry(&pair, id, &temp);
    {
        volatile u8 second;

        struct {
            volatile u8 x;
        } first = { 0 };

        second = 0;
        entry.id = pair.id;
        LobbyString_InitCopy(&entry.value, &pair.value);
        LobbyUserMap_Insert(&result, &w->tree, &entry);
        ok = result.success;
        LobbyString_Destroy(&entry.value);
        LobbyString_Destroy(&pair.value);
        LobbyString_Destroy(&temp);
        return ok ? id : -1;
    }
}
