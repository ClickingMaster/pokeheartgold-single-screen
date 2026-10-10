#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[20];
    u8 flags;
    u8 pad2[7];
    int ticks;
} Item;
typedef struct Work Work;

struct Work {
    u8 pad[44];
    u8 index[20];
    int count;
    void (*callback)(Work *, int, Item *, void *);
    void *context;
};

extern void *ov00_0221A0B8[];
extern int ov00_02213FD4(s8 *, int);
extern int ov00_021FF368(void);
extern void ov00_02212CD0(Item *, void *, s8 *);
extern void ov00_02213044(Item *, s8 *, int);
extern void ov00_02212384(void *, Item *);
void ov00_02212724(Work *, Item *, s8 *, int);

void ov00_02212724(Work *w, Item *item, s8 *data, int length) {
    int i, size;
    if (*data) {
        return;
    }
    data += 5;
    length -= 5;
    if (item->flags & 4) {
        for (i = 0; i < w->count; i++) {
            size = ov00_02213FD4(data, length);
            if (size < 0) {
                break;
            }
            ov00_02212CD0(item, ov00_0221A0B8[w->index[i]], data);
            data += size;
            length -= size;
        }
        item->flags |= 0x41;
    } else {
        ov00_02213044(item, data, length);
        item->flags |= 0x43;
    }
    item->flags &= 0xf3;
    item->ticks = ov00_021FF368() - item->ticks;
    ov00_02212384((u8 *)w + 8, item);
    w->callback(w, 0, item, w->context);
}
