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

extern u8 *ov45_022577DC;
void LobbyString_InitFromCStr(String *, const char *);
void LobbyString_Destroy(String *);
int ov45_02243808(void *, String *);
int chatGetChannelNumUsersA(int, const char *);
char *ov45_02242A2C(String *);
void ov45_0224367C(String *, void *, char *);
int ov45_02243280(void *, String *, String *);
Hooks *ov45_02242C3C(void *, String *);
void ov45_022428D8(String *, void *, String *);
int ov45_02242D30(void *, String *, String *);
int ov45_02243794(void *, String *);
void ov45_0223F32C(void *, const char *, const char *);

static inline char *getStringData(String *s) {
    if ((BOOL)s->large.tag == FALSE) {
        return s->small.chars;
    }
    return s->large.ptr;
}

void ov45_0223F32C(void *unused, const char *key, const char *value) {
    String result, a, b, c, d, e, f, g, h, i, j, k, l, m;
    int exists, index;
    Hooks *hooks;
    if (ov45_022577DC && *(int *)(ov45_022577DC + 4) == 5) {
        return;
    }
    LobbyString_InitFromCStr(&a, key);
    exists = ov45_02243808(ov45_022577DC, &a);
    LobbyString_Destroy(&a);
    if (exists && chatGetChannelNumUsersA(*(int *)ov45_022577DC, key) > exists) {
        return;
    }
    LobbyString_InitFromCStr(&b, value);
    ov45_0224367C(&result, ov45_022577DC, ov45_02242A2C(&b));
    LobbyString_Destroy(&b);
    LobbyString_InitFromCStr(&c, key);
    exists = ov45_02243280(ov45_022577DC, &result, &c);
    LobbyString_Destroy(&c);
    if (exists) {
        LobbyString_InitFromCStr(&d, key);
        hooks = ov45_02242C3C(ov45_022577DC, &d);
        LobbyString_Destroy(&d);
        LobbyString_InitFromCStr(&e, key);
        ov45_022428D8(&f, ov45_022577DC, &e);
        LobbyString_InitFromCStr(&g, value);
        {
            String *second = &g;
            char *first = getStringData(&f);
            hooks->second(first, ov45_02242A2C(second), hooks->arg);
        }
        LobbyString_Destroy(&g);
        LobbyString_Destroy(&f);
        LobbyString_Destroy(&e);
    }
    LobbyString_InitFromCStr(&h, value);
    LobbyString_InitFromCStr(&i, key);
    index = ov45_02242D30(ov45_022577DC, &h, &i);
    LobbyString_Destroy(&i);
    LobbyString_Destroy(&h);
    if (index == -1) {
        LobbyString_Destroy(&result);
        return;
    }
    LobbyString_InitFromCStr(&j, key);
    exists = ov45_02243794(ov45_022577DC, &j);
    LobbyString_Destroy(&j);
    if (exists) {
        LobbyString_InitFromCStr(&k, key);
        hooks = ov45_02242C3C(ov45_022577DC, &k);
        LobbyString_Destroy(&k);
        LobbyString_InitFromCStr(&l, key);
        ov45_022428D8(&m, ov45_022577DC, &l);
        hooks->first(getStringData(&m), index, hooks->arg);
        LobbyString_Destroy(&m);
        LobbyString_Destroy(&l);
    }
    LobbyString_Destroy(&result);
    (void)unused;
}
