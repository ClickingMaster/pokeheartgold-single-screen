#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma opt_strength_reduction off

typedef struct A {
    u8 used, pad;
    u16 v;
    u32 index;
    struct A *prev, *next;
    u8 data[192];
} A;

typedef struct {
    u32 n;
    A *first, *last;
} L;

typedef struct {
    u16 a, b;
    void *c, *d;
    int e;
} Msg;

typedef struct {
    u8 pad[0x2260];
    int phase;
    u32 option;
    u16 x268;
    u8 lock, reset;
    u32 x26c;
    L *list;
    u32 size;
    u32 mode;
    void (*callback)(Msg *);
    s16 notify;
    u16 aid;
} W;

extern W *WCMi_GetSystemWork(void);
A *WcmAllocApList(void);
A *w4(void);

A *WcmAllocApList(void) {
    W *work = WCMi_GetSystemWork();
    L *list = work->list;
    A *entry = NULL;

    struct {
        A *base;
        u32 count;
    } span;

    u32 capacity, index;
    if (!list) {
        return NULL;
    }
    if (work->size <= sizeof(L)) {
        return NULL;
    }
    capacity = (work->size - sizeof(L)) / sizeof(A);
    if (!capacity) {
        return NULL;
    }
    if (capacity <= list->n) {
        return NULL;
    }
    span.count = capacity;
    index = 0;
    if (index < span.count) {
        span.base = (A *)((u8 *)list + sizeof(L));
        do {
            entry = span.base + index;
            if (!entry->used) {
                break;
            }
        } while (++index < span.count);
    }
    if (index >= span.count) {
        return entry;
    }
    entry->used = 1;
    entry->index = list->n;
    entry->next = NULL;
    entry->prev = list->last;
    list->last = entry;
    if (entry->prev) {
        entry->prev->next = entry;
    } else {
        list->first = entry;
    }
    list->n++;
    return entry;
}
