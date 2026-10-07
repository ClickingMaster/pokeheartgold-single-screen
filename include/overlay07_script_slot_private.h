#ifndef OVERLAY07_SCRIPT_SLOT_PRIVATE_H
#define OVERLAY07_SCRIPT_SLOT_PRIVATE_H
#include "global.h"
typedef struct Ov07T10Slot {
    u32 unk00;
    s16 xy[2];
    void *pokepic;
    void *sprite;
    u32 index;
} Ov07T10Slot;
typedef char Ov07ScriptSlotSize[(sizeof(Ov07T10Slot)==20)?1:-1];
typedef struct Ov07T10Work {
    u8 state[4];
    void *owner;
    void *field08;
    void *field0C;
    void *field10;
    void *field14;
    void *field18;
} Ov07T10Work;
#endif
