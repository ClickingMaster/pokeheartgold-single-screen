#include "to56_w06_private.h"

typedef struct Ov07Owner {
    u8 pad00[4];
    u32 field04;
    u16 field08;
    u16 field0A;
    u32 field0C;
    u16 field10;
    u16 field12;
    u8 pad14[8];
    void *objects[16];
} Ov07Owner;

typedef struct ScriptSlot {
    u32 *script;
    u8 counter;
    u8 limit;
    u8 pad06[2];
    u32 active;
} ScriptSlot;

typedef struct Ov07Script {
    void *field00;
    u8 pad04[0x14];
    u32 *script;
    u32 *stack[3];
    ScriptSlot slots[3];
    u8 pad4C[0x41];
    u8 result;
    u8 pad8E[0x2E];
    void (*callback)(void);
    Ov07Owner *owner;
} Ov07Script;

u32 ov07_0221C768(Ov07Script *p, u32 which) {
    switch (which) {
    case 0:
        return p->owner->field04;
    case 1:
        return p->owner->field08;
    case 2:
        return p->owner->field0A;
    case 3:
        return p->owner->field0C;
    case 4:
        return p->owner->field10;
    case 5:
        return p->owner->field12;
    default:
        return 0;
    }
}
