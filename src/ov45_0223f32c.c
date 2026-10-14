#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef union String {
    struct {
        u32 tag : 1;
        u32 rest : 31;
        char *other;
        char *ptr;
    } large;

    struct {
        char tag;
        char chars[11];
    } small;
} String;

typedef struct Hooks {
    void (*first)(char *, int, void *);
    void (*second)(char *, char *, void *);
    int pad[3];
    void *arg;
} Hooks;

extern u8 *gLobbyChatManager;
void LobbyString_InitFromCStr(String *, const char *);
void LobbyString_Destroy(String *);
int LobbyChat_GetChannelUserLimit(void *, String *);
int chatGetChannelNumUsersA(int, const char *);
char *LobbyChat_GetUserIdFromNick(String *);
void LobbyChat_GetUserNick(String *, void *, char *);
int LobbyChat_RemoveChannelUser(void *, String *, String *);
Hooks *LobbyChat_GetChannelCallbacks(void *, String *);
void LobbyChat_StripChannelPrefix(String *, void *, String *);
int LobbyChat_AddChannelUser(void *, String *, String *);
int LobbyChat_AreChannelUsersEnumerated(void *, String *);
void LobbyChat_OnUserJoined(void *, const char *, const char *);

static inline char *getStringData(String *s) {
    if ((BOOL)s->large.tag == FALSE) {
        return s->small.chars;
    }
    return s->large.ptr;
}

void LobbyChat_OnUserJoined(void *unused, const char *key, const char *value) {
    String result, a, b, c, d, e, f, g, h, i, j, k, l, m;
    int exists, index;
    Hooks *hooks;
    if (gLobbyChatManager && *(int *)(gLobbyChatManager + 4) == 5) {
        return;
    }
    LobbyString_InitFromCStr(&a, key);
    exists = LobbyChat_GetChannelUserLimit(gLobbyChatManager, &a);
    LobbyString_Destroy(&a);
    if (exists && chatGetChannelNumUsersA(*(int *)gLobbyChatManager, key) > exists) {
        return;
    }
    LobbyString_InitFromCStr(&b, value);
    LobbyChat_GetUserNick(&result, gLobbyChatManager, LobbyChat_GetUserIdFromNick(&b));
    LobbyString_Destroy(&b);
    LobbyString_InitFromCStr(&c, key);
    exists = LobbyChat_RemoveChannelUser(gLobbyChatManager, &result, &c);
    LobbyString_Destroy(&c);
    if (exists) {
        LobbyString_InitFromCStr(&d, key);
        hooks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &d);
        LobbyString_Destroy(&d);
        LobbyString_InitFromCStr(&e, key);
        LobbyChat_StripChannelPrefix(&f, gLobbyChatManager, &e);
        LobbyString_InitFromCStr(&g, value);
        {
            String *second = &g;
            char *first = getStringData(&f);
            hooks->second(first, LobbyChat_GetUserIdFromNick(second), hooks->arg);
        }
        LobbyString_Destroy(&g);
        LobbyString_Destroy(&f);
        LobbyString_Destroy(&e);
    }
    LobbyString_InitFromCStr(&h, value);
    LobbyString_InitFromCStr(&i, key);
    index = LobbyChat_AddChannelUser(gLobbyChatManager, &h, &i);
    LobbyString_Destroy(&i);
    LobbyString_Destroy(&h);
    if (index == -1) {
        LobbyString_Destroy(&result);
        return;
    }
    LobbyString_InitFromCStr(&j, key);
    exists = LobbyChat_AreChannelUsersEnumerated(gLobbyChatManager, &j);
    LobbyString_Destroy(&j);
    if (exists) {
        LobbyString_InitFromCStr(&k, key);
        hooks = LobbyChat_GetChannelCallbacks(gLobbyChatManager, &k);
        LobbyString_Destroy(&k);
        LobbyString_InitFromCStr(&l, key);
        LobbyChat_StripChannelPrefix(&m, gLobbyChatManager, &l);
        hooks->first(getStringData(&m), index, hooks->arg);
        LobbyString_Destroy(&m);
        LobbyString_Destroy(&l);
    }
    LobbyString_Destroy(&result);
    (void)unused;
}
