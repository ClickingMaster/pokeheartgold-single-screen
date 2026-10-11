#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 active;
    u8 pad[0x368];
    u8 container[1];
} Context;

typedef struct {
    u32 index;
    u8 pad[0x28];
    u32 count;
} Entry;

typedef struct {
    const char *key;
    void *container;
    const char *value;
    u32 type;
} Record;

extern const char ov45_02256BA0[];
extern const char ov45_02256B94[];
extern const char ov45_02256B6C[];
extern const char ov45_02256E9C[];
extern const char ov45_02256EAC[];
extern const char ov45_02256ED8[];
extern const char ov45_02256F50[];
extern const char ov45_02256F60[];
extern const char ov45_02256F78[];
extern const char ov45_02256F88[];
extern const char ov45_02256F94[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02252F74(void *, const char *, ...);
extern Entry *ov45_02247E98(Context *, const char *);
extern void ov45_02244D30(Context *, int, u32, const Record *, u32, int, const char *, int);
void ov45_0225089C(Context *, const char *, const char *, u32);

void ov45_0225089C(Context *c, const char *key, const char *value, u32 type) {
    Entry *e;
    Record rec;
    if (!c) {
        __msl_assertion_failed(ov45_02256BA0, ov45_02256B94, ov45_02256B6C, 0x5fc);
    }
    if (!c || !c->active) {
        return;
    }
    if (!key) {
        __msl_assertion_failed(ov45_02256E9C, ov45_02256B94, ov45_02256B6C, 0x5ff);
    }
    if (!*key) {
        __msl_assertion_failed(ov45_02256EAC, ov45_02256B94, ov45_02256B6C, 0x5ff);
    }
    if (type > 4) {
        __msl_assertion_failed(ov45_02256ED8, ov45_02256B94, ov45_02256B6C, 0x600);
    }
    if (!value || !*value) {
        return;
    }
    if (type == 0) {
        ov45_02252F74((u8 *)c + 0x1c, ov45_02256F50, key, value);
    } else if (type == 1) {
        ov45_02252F74((u8 *)c + 0x1c, ov45_02256F60, key, value);
    } else if (type == 2) {
        ov45_02252F74((u8 *)c + 0x1c, ov45_02256F78, key, value);
    } else if (type == 3) {
        ov45_02252F74((u8 *)c + 0x1c, ov45_02256F88, key, value);
    } else if (type == 4) {
        ov45_02252F74((u8 *)c + 0x1c, ov45_02256F94, key, value);
    } else {
        return;
    }
    e = ov45_02247E98(c, key);
    if (!e) {
        return;
    }
    rec.container = (u8 *)c + 0x36c;
    rec.key = key;
    rec.value = value;
    rec.type = type;
    ov45_02244D30(c, 4, e->index, &rec, e->count, 0, key, 16);
}
