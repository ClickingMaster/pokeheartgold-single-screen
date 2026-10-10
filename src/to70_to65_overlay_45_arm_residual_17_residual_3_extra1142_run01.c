#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    char text[0xdc];
    int value;
} Item;

typedef struct {
    void *w;
    int count;
    Item **items;
    int *values;
} List;

typedef struct {
    u8 p[0x134];
    void *list;
} Result;

extern char ov45_02255938[];
extern char ov45_02255918[];
extern char ov45_0225585C[];
extern char ov45_02255A30[];
extern char ov45_02255A44[];
extern char ov45_02255A58[];
extern char ov45_02255A70[];
extern char ov45_02255A90[];
extern char ov45_02255AA4[];
extern char ov45_022559B8[];
extern char ov45_02255778[];
extern char ov45_022559D4[];
extern char ov45_022559E4[];
extern char ov45_022559F8[];
extern char ov45_02255AA8[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void *ov00_021EC3BC(void *, u32);
extern void ov00_021EC3D8(void *), ov00_021FF160(void *, void (*)(Item *, List *), List *);
extern Result *ov45_022475B8(void *, const char *);
void ov45_02247F60(Item *, List *);
void ov45_022480EC(void *, const char *, void (*)(void *, const char *, int, Item **, int *, int), int);

void ov45_02247F60(Item *item, List *l) {
    void *p;
    if (!item) {
        __msl_assertion_failed(ov45_02255938, ov45_02255918, ov45_0225585C, 444);
    }
    if (!l) {
        __msl_assertion_failed(ov45_02255A30, ov45_02255918, ov45_0225585C, 445);
    }
    if (!item) {
        __msl_assertion_failed(ov45_02255A44, ov45_02255918, ov45_0225585C, 450);
    }
    if (!item->text[0]) {
        __msl_assertion_failed(ov45_02255A58, ov45_02255918, ov45_0225585C, 450);
    }
    if (strlen(item->text) >= 128) {
        __msl_assertion_failed(ov45_02255A70, ov45_02255918, ov45_0225585C, 450);
    }
    if (l->count < 0) {
        __msl_assertion_failed(ov45_02255A90, ov45_02255918, ov45_0225585C, 455);
    }
    p = ov00_021EC3BC(l->items, (l->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(ov45_02255AA4, ov45_02255918, ov45_0225585C, 473);
        return;
    }
    l->items = p;
    p = ov00_021EC3BC(l->values, (l->count + 1) * 4);
    if (!p) {
        __msl_assertion_failed(ov45_02255AA4, ov45_02255918, ov45_0225585C, 480);
        return;
    }
    l->values = p;
    l->items[l->count] = item;
    l->values[l->count] = item->value;
    l->count++;
}

void ov45_022480EC(void *w, const char *name, void (*cb)(void *, const char *, int, Item **, int *, int), int arg) {
    List l;
    Result *r;
    if (!w) {
        __msl_assertion_failed(ov45_022559B8, ov45_02255918, ov45_02255778, 496);
    }
    if (!name) {
        __msl_assertion_failed(ov45_022559D4, ov45_02255918, ov45_02255778, 498);
    }
    if (!name[0]) {
        __msl_assertion_failed(ov45_022559E4, ov45_02255918, ov45_02255778, 498);
    }
    if (strlen(name) >= 257) {
        __msl_assertion_failed(ov45_022559F8, ov45_02255918, ov45_02255778, 498);
    }
    if (!cb) {
        __msl_assertion_failed(ov45_02255AA8, ov45_02255918, ov45_02255778, 499);
    }
    r = ov45_022475B8(w, name);
    if (!r) {
        return;
    }
    l.w = w;
    l.count = 0;
    l.items = 0;
    l.values = 0;
    ov00_021FF160(r->list, ov45_02247F60, &l);
    cb(w, name, l.count, l.items, l.values, arg);
    ov00_021EC3D8(l.items);
    ov00_021EC3D8(l.values);
}
