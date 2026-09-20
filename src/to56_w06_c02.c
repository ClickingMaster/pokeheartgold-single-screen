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

void ov07_0221C8CC(Ov07Script *p) {
    u32 which;
    u8 temp[0x50];
    p->script++;
    which = *p->script;
    p->script++;
    ov07_0221C7B8(p, temp, which);
    if (which == 2) {
        ov07_02234A20(temp, p->field00);
    } else if (which == 3) {
        ov07_0223475C(temp, p->field00);
    } else {
        ov07_0223474C(temp, p->field00);
    }
}

void ov07_0221C910(Ov07Script *p) {
    u32 which;
    u8 temp[0x50];
    p->script++;
    which = *p->script;
    p->script++;
    ov07_0221C7B8(p, temp, which);
    ov07_0223494C(temp, p->field00);
}

void ov07_0221C934(Ov07Script *p) {
    int i;
    p->script++;
    for (i = 0; i < 3; i++) {
        if (p->slots[i].active != 1) {
            p->slots[i].active = 1;
            p->slots[i].counter = 0;
            p->slots[i].limit = *p->script;
            p->script++;
            p->slots[i].script = p->script;
            return;
        }
    }
}

void ov07_0221C974(Ov07Script *p) {
    int i;
    p->script++;
    for (i = 2; i >= 0; i--) {
        if (p->slots[i].active != 0) {
            p->slots[i].counter++;
            if (p->slots[i].counter == p->slots[i].limit) {
                p->slots[i].active = 0;
            } else {
                p->script = p->slots[i].script;
            }
            return;
        }
    }
}
