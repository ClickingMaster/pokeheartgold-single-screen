#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct String {
    union {
        struct {
            u32 flag : 1;
            u32 spare : 31;
        } bits;

        char bytes[4];
    } head;

    u32 cap;
    char *text;
};

struct State {
    int a, state;
};

struct Callback {
    void (*first)(char *, int, void *);
    void (*second)(char *, int, void *);
    u8 pad[12];
    void *arg;
};
extern struct State *ov45_022577DC;
extern void LobbyString_InitFromCStr(struct String *, char *);
extern void LobbyString_Destroy(struct String *);
extern void LobbyChat_StripChannelPrefix(struct String *, struct State *, struct String *);
extern int LobbyChat_FindUserIdByNick(struct State *, struct String *);
extern int LobbyChat_RemoveChannelUser(struct State *, struct String *, struct String *);
extern struct Callback *LobbyChat_GetChannelCallbacks(struct State *, struct String *);
void LobbyChat_OnUserParted(int, char *, char *);

static inline char *getText(struct String *s) {
    if (s->head.bits.flag == 0) {
        return s->head.bytes + 1;
    }
    return s->text;
}

void LobbyChat_OnUserParted(int unused, char *name, char *value) {
    struct String a, b, c, d, e, f;
    int id;
    struct Callback *cb;
    if (ov45_022577DC && ov45_022577DC->state == 5) {
        return;
    }
    LobbyString_InitFromCStr(&a, value);
    id = LobbyChat_FindUserIdByNick(ov45_022577DC, &a);
    LobbyString_Destroy(&a);
    if (id == -1) {
        return;
    }
    LobbyString_InitFromCStr(&b, name);
    cb = LobbyChat_GetChannelCallbacks(ov45_022577DC, &b);
    LobbyString_Destroy(&b);
    LobbyString_InitFromCStr(&c, name);
    LobbyChat_StripChannelPrefix(&d, ov45_022577DC, &c);
    cb->second(getText(&d), id, cb->arg);
    LobbyString_Destroy(&d);
    LobbyString_Destroy(&c);
    LobbyString_InitFromCStr(&e, value);
    LobbyString_InitFromCStr(&f, name);
    LobbyChat_RemoveChannelUser(ov45_022577DC, &e, &f);
    LobbyString_Destroy(&f);
    LobbyString_Destroy(&e);
}
