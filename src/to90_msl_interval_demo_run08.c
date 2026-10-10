#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 action;
} Action;

typedef struct {
    u8 pad[8];
    Action *current;
} ActionIterator;

u8 _Z13CurrentActionPK14ActionIterator(const ActionIterator *);

u8 _Z13CurrentActionPK14ActionIterator(const ActionIterator *it) {
    return it->current ? (it->current->action & 0x1f) : 0;
}
