#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 kind, flags;
    u16 pad;
    u32 id;
} Item;

typedef struct {
    u32 count;
    Item items[1];
} Group;

extern Group *NNS_SndArcGetGroupInfo(int);
extern int NNSi_SndArcLoadSeq(u32, u32, void *, BOOL, void **);
extern int NNSi_SndArcLoadSeqArc(u32, u32, void *, BOOL, void **);
extern int NNSi_SndArcLoadBank(u32, u32, void *, BOOL, void **);
extern int NNSi_SndArcLoadWaveArc(u32, u32, void *, BOOL, void **);
int NNSi_SndArcLoadGroup(int, void *);

int NNSi_SndArcLoadGroup(int id, void *heap) {
    Group *g = NNS_SndArcGetGroupInfo(id);
    u32 i;
    Item *p;
    int result;
    if (!g) {
        return 1;
    }
    i = 0;
    if (i < g->count) {
        p = g->items;
        do {
            switch (p->kind) {
            case 0:
                result = NNSi_SndArcLoadSeq(p->id, p->flags, heap, 1, NULL);
                if (result) {
                    return result;
                }
                break;
            case 3:
                result = NNSi_SndArcLoadSeqArc(p->id, p->flags, heap, 1, NULL);
                if (result) {
                    return result;
                }
                break;
            case 1:
                result = NNSi_SndArcLoadBank(p->id, p->flags, heap, 1, NULL);
                if (result) {
                    return result;
                }
                break;
            case 2:
                result = NNSi_SndArcLoadWaveArc(p->id, p->flags, heap, 1, NULL);
                if (result) {
                    return result;
                }
                break;
            }
            p++;
            i++;
        } while (i < g->count);
    }
    return 0;
}
