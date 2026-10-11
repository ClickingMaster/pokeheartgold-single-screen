#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x20];
    u32 *args;
    s32 count;
} Command;

typedef struct {
    u32 kind, value;
    void *ptr;
} Entry;

extern char ov45_022567F4[];
extern char ov45_022565D0[];
extern char ov45_02255F44[];
extern char ov45_02256690[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern int ov45_02248238(void *, u32);
extern int ov45_02248288(void *, u32, void *);
extern Entry *ov45_0224AD24(u32, u32 *, int);
extern void ov45_02248454(void *, u32, void *);
extern void ov45_02249194(void *, void *, u32, int, u32);
extern void ov45_0224AFD8(void *, Entry *);
extern void ov45_02248374(void *, u32, void *);
extern void ov00_021EC3D8(void *);
void ov45_0224C388(void *, Command *);

void ov45_0224C388(void *sys, Command *c) {
    Entry *first, *e;
    u32 id, kind;
    u32 state[8];
    if (c->count < 2) {
        __msl_assertion_failed(ov45_022567F4, ov45_022565D0, ov45_02255F44, 0x893);
    }
    if (c->count < 2) {
        return;
    }
    id = c->args[0];
    kind = c->args[1];
    if (ov45_02248238(sys, id) == 0) {
        return;
    }
    first = ov45_0224AD24(kind, c->args + 2, c->count - 2);
    if (first == NULL) {
        return;
    }
    e = first;
    while (e->kind != 0) {
        switch (e->kind) {
        case 6:
            if (e->value != 0) {
                ov45_02248454(sys, id, e->ptr);
            } else {
                ov45_02248454(sys, id, NULL);
            }
            break;
        case 10:
            if (e->ptr != NULL) {
                ov45_02249194(sys, e->ptr, id, 2, e->value);
            }
            break;
        case 11:
            if (e->ptr != NULL) {
                ov45_02249194(sys, e->ptr, id, 1, e->value);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 12:
        case 13:
        case 14:
            break;
        default:
            __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02255F44, 0x8dc);
            break;
        }
        e++;
    }
    if (ov45_02248288(sys, id, state) == 0) {
        ov00_021EC3D8(first);
        return;
    }
    ov45_0224AFD8(state, first);
    ov45_02248374(sys, id, state);
    ov00_021EC3D8(first);
}
