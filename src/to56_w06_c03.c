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

void ov07_0221CF14(Ov07Script *p) {
    int sum = 0;
    int i;
    for (i = 0; i < 16; i++) {
        if (p->owner->objects[i] != NULL) {
            sum += ParticleSystem_GetActiveEmitterCount(p->owner->objects[i]);
        }
    }
    if (sum == 0) {
        p->script++;
        p->result = 0;
    } else {
        p->result = 1;
    }
}

void ov07_0221CF54(Ov07Script *p) {
    u32 i;
    void *object;
    u32 value;
    p->script++;
    i = *p->script;
    p->script++;
    if (p->owner->objects[i] != NULL) {
        GF_AssertFail();
    }
    value = *p->script;
    p->script++;
    object = ov07_0221FE88(p->field00, value, 0);
    p->owner->objects[i] = object;
    p->result = 2;
    p->callback = ov07_0221BE20;
}

void ov07_0221CFA8(Ov07Script *p) {
    u32 i;
    void *object;
    u32 value;
    p->script++;
    i = *p->script;
    p->script++;
    p->script++;
    if (p->owner->objects[i] != NULL) {
        GF_AssertFail();
    }
    value = *p->script;
    p->script++;
    object = ov07_0221FEB0(p->field00, 0x60, value, 0);
    p->owner->objects[i] = object;
    p->result = 2;
    p->callback = ov07_0221BE20;
}

void ov07_0221D000(Ov07Script *p) {
    u32 i;
    p->script++;
    i = *p->script;
    p->script++;
    ov07_0221FF2C(p->owner->objects[i]);
    p->owner->objects[i] = NULL;
}

void ov07_0221D02C(Ov07Script *p) {
    int i;
    p->script++;
    for (i = 0; i < 3; i++) {
        if (p->stack[i] == NULL) {
            p->stack[i] = p->script + 1;
            p->script += *p->script;
            return;
        }
    }
}

void ov07_0221D05C(Ov07Script *p) {
    int i;
    p->script++;
    for (i = 2; i >= 0; i--) {
        if (p->stack[i] != NULL) {
            p->script = p->stack[i];
            p->stack[i] = NULL;
            return;
        }
    }
}
