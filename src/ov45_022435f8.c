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

extern int ov45_02242A2C(const String *);
extern void ov45_022431FC(void **, void *, const void *);
extern int ov45_022340F8(const void *, const void *);
extern void LobbyString_InitFromCStr(String *, const char *);
extern char ov45_022553A8[];
extern void LobbyString_InitCopy(String *, const void *);
extern void ov45_022431AC(void *, int *);
extern void ov45_02243780(Pair *, int, const String *);
extern void ov45_02243F3C(void *, void *, const Pair *);
extern void LobbyString_Destroy(String *);
int ov45_022435F8(Work *, const String *);
void ov45_0224367C(String *, Work *, int, ...);
int ov45_022436E0(Work *, const String *, const String *);

int ov45_022435F8(Work *w, const String *key) {
    int id;
    void *node;
    id = ov45_02242A2C(key);
    if (id == -1) {
        return -1;
    }
    ov45_022431FC(&node, &w->tree, &id);
    if (node == w->sentinel) {
        return -1;
    }
    if ((BOOL)(ov45_022340F8((u8 *)node + 0x10, key) == 0) != FALSE) {
        return -1;
    }
    return id;
}

void ov45_0224367C(String *out, Work *w, int id, ...) {
    void *node;
    ov45_022431FC(&node, &w->tree, &id);
    if (node == w->sentinel) {
        LobbyString_InitFromCStr(out, ov45_022553A8);
    } else {
        LobbyString_InitCopy(out, (u8 *)node + 0x10);
    }
}
