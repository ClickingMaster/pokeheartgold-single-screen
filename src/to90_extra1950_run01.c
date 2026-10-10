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
void ov45_02237B58(String *, const char *);
void ov45_02237BC8(String *);
int ov45_02243808(void *, String *);
int ov45_02250F5C(int, const char *);
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
    ov45_02237B58(&a, key);
    exists = ov45_02243808(ov45_022577DC, &a);
    ov45_02237BC8(&a);
    if (exists && ov45_02250F5C(*(int *)ov45_022577DC, key) > exists) {
        return;
    }
    ov45_02237B58(&b, value);
    ov45_0224367C(&result, ov45_022577DC, ov45_02242A2C(&b));
    ov45_02237BC8(&b);
    ov45_02237B58(&c, key);
    exists = ov45_02243280(ov45_022577DC, &result, &c);
    ov45_02237BC8(&c);
    if (exists) {
        ov45_02237B58(&d, key);
        hooks = ov45_02242C3C(ov45_022577DC, &d);
        ov45_02237BC8(&d);
        ov45_02237B58(&e, key);
        ov45_022428D8(&f, ov45_022577DC, &e);
        ov45_02237B58(&g, value);
        {
            String *second = &g;
            char *first = getStringData(&f);
            hooks->second(first, ov45_02242A2C(second), hooks->arg);
        }
        ov45_02237BC8(&g);
        ov45_02237BC8(&f);
        ov45_02237BC8(&e);
    }
    ov45_02237B58(&h, value);
    ov45_02237B58(&i, key);
    index = ov45_02242D30(ov45_022577DC, &h, &i);
    ov45_02237BC8(&i);
    ov45_02237BC8(&h);
    if (index == -1) {
        ov45_02237BC8(&result);
        return;
    }
    ov45_02237B58(&j, key);
    exists = ov45_02243794(ov45_022577DC, &j);
    ov45_02237BC8(&j);
    if (exists) {
        ov45_02237B58(&k, key);
        hooks = ov45_02242C3C(ov45_022577DC, &k);
        ov45_02237BC8(&k);
        ov45_02237B58(&l, key);
        ov45_022428D8(&m, ov45_022577DC, &l);
        hooks->first(getStringData(&m), index, hooks->arg);
        ov45_02237BC8(&m);
        ov45_02237BC8(&l);
    }
    ov45_02237BC8(&result);
    (void)unused;
}
