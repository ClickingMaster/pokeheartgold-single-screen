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
extern void ov45_02237B58(String *, const char *);
extern char ov45_022553A8[];
extern void ov45_02237A24(String *, const void *);
extern void ov45_022431AC(void *, int *);
extern void ov45_02243780(Pair *, int, const String *);
extern void ov45_02243F3C(void *, void *, const Pair *);
extern void ov45_02237BC8(String *);
int ov45_022435F8(Work *, const String *);
void ov45_0224367C(String *, Work *, int, ...);
int ov45_022436E0(Work *, const String *, const String *);

int ov45_022436E0(Work *w, const String *key, const String *value) {
    String temp;
    Pair pair;
    Pair entry;
    int id;

    struct {
        void *node;
        u8 success;
    } result;

    int ok;
    id = ov45_022435F8(w, key);
    ov45_022431AC(&w->tree, &id);
    ov45_02237A24(&temp, value);
    ov45_02243780(&pair, id, &temp);
    {
        volatile u8 second;

        struct {
            volatile u8 x;
        } first = { 0 };

        second = 0;
        entry.id = pair.id;
        ov45_02237A24(&entry.value, &pair.value);
        ov45_02243F3C(&result, &w->tree, &entry);
        ok = result.success;
        ov45_02237BC8(&entry.value);
        ov45_02237BC8(&pair.value);
        ov45_02237BC8(&temp);
        return ok ? id : -1;
    }
}
