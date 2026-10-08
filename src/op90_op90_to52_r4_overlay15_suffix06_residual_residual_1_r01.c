#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "text.h"
typedef struct {
    u16 id, quantity;
} Item;
typedef struct {
    Item *items;
    u16 reserved;
    s16 position;
    u8 state, size;
    u16 reserved2;
} Slot;
typedef struct {
    u32 reserved;
    Slot slots[8];
    u8 category;
} Bag;
typedef struct {
    u8 prefix[0xb4];
    Window windows[12];
    u8 gap174[0xc0];
    Bag *bag;
    u8 gap238[0x118];
    String *strings[200];
    u8 gap670[2];
    u8 selected;
    u8 gap673[0x17];
    u8 phase;
} State;
extern const u8 ov15_022008C8[];
void ov15_021FE17C(State *);
int ov15_021FF320(Slot *, int, int);
void ov15_021FF570(State *, Window *, String *, Slot *, int);
void ov15_021FF364(State *s, int start, void *unused, int dim);
// @fn ov15_021FF364
void ov15_021FF364(State *s, int start, void *unused, int dim) {
    int i, k, count;
    Slot *slot;
    int limit;
    u8 first, last;
    slot = &s->bag->slots[s->bag->category];
    limit = slot->size - slot->position;
    if (limit > 6) {
        limit = 6;
    }
    if (s->phase == 0) {
        first = 0;
        last = 6;
    } else {
        first = 6;
        last = 0;
    }
    s->phase ^= 1;
    ov15_021FE17C(s);
    for (i = 0; i < 6; i++) {
        FillWindowPixelBuffer(&s->windows[first + i], 0);
        ClearWindowTilemapAndScheduleTransfer(&s->windows[last + i]);
    }
    count = 0;
    k = ov15_021FF320(slot, s->bag->category, start);
    for (; k < ov15_022008C8[s->bag->category]; k++) {
        if (slot->items[k].id != 0 && slot->items[k].quantity != 0) {
            if (dim == 0) {
                ov15_021FF570(s, &s->windows[first + count], s->strings[k], slot, k);
            } else if (k == s->selected) {
                ov15_021FF570(s, &s->windows[first + count], s->strings[k], slot, k);
            } else {
                AddTextPrinterParameterizedWithColor(&s->windows[first + count], 0, s->strings[k], 0, 16, 255, 0x10200, NULL);
            }
            if (++count >= limit) {
                break;
            }
        }
    }
    for (i = 0; i < 6; i++) {
        ScheduleWindowCopyToVram(&s->windows[first + i]);
    }
}
