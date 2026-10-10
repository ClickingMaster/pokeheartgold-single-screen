#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    void *object;
    u32 unused;
    void (*destroy)(void *, int);
} Catch;

void __end__catch(Catch *);

void __end__catch(Catch *c) {
    if (c->object && c->destroy) {
        c->destroy(c->object, -1);
    }
}
