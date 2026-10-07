#pragma force_active on
#include "global.h"
#include "assert.h"

typedef void (*Ov45HandlerFunc)(u32 id, void *data, u32 size, void *arg);

typedef struct Ov45Handler {
    Ov45HandlerFunc func;
    u32 size;
} Ov45Handler;

typedef struct Ov45HandlerTable {
    Ov45Handler *handlers;
    u32 count;
    void *arg;
} Ov45HandlerTable;

typedef struct Ov45Global {
    u8 unk0[0x12D];
    u8 unk12D;
    u8 unk12E[0x56];
    Ov45HandlerTable tables[2];
} Ov45Global;

extern Ov45Global *_022577C0;

int ov45_0222FB24(Ov45Global *global, u32 id, int a2);
u8 ov45_0222F7A8(u32 kind);
void ov45_0222FDD8(u32 id, u32 kind, u32 index, void *data, u32 size);

// @fn ov45_0222FDD8
void ov45_0222FDD8(u32 id, u32 kind, u32 index, void *data, u32 size) {
    int table;

    if (ov45_0222FB24(_022577C0, id, 0) == -1) {
        return;
    }
    switch (kind) {
    case 0:
        table = 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        if (_022577C0->unk12D != ov45_0222F7A8(kind)) {
            return;
        }
        table = 1;
        break;
    case 5:
        table = 0;
        break;
    default:
        return;
    }
    if (_022577C0->tables[table].count <= index) {
        return;
    }
    GF_ASSERT(_022577C0->tables[table].handlers[index].size <= size);
    _022577C0->tables[table].handlers[index].func(id, data, _022577C0->tables[table].handlers[index].size, _022577C0->tables[table].arg);
}
