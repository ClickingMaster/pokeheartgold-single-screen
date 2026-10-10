#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad[0x80];
    int step;
} Core;

typedef struct {
    u8 pad0[8];
    Core *core;
    u8 padC[0xf94 - 12];
    int drawing;
} State;

typedef struct {
    u16 seq;
    s16 x, y;
    u16 attr;
} Hier;

typedef struct {
    u16 count, animations;
    Hier *hier;
} Data;

typedef struct {
    Data *data;
    void *bank;
    int type;
    void *instances;
} Multi;

typedef struct {
    u8 data[0x5c];
} Anim;

typedef struct {
    u8 data[0x28];
} Node;

typedef struct {
    u8 pad[0x90];
    u16 index;
} Indices;

typedef struct {
    u32 pad;
    u32 values[1];
} Cache;

extern State _021D94D8;
extern Indices _021D9FD8;
extern Cache _021DA068;
extern int NNSi_G2dGetOamSoftEmuAutoZOffsetStep(void);
extern void NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(int);
extern void NNSi_G2dSetOamSoftEmuAutoZOffsetStep(int);
extern void NNSi_G2dResetOamSoftEmuAutoZOffset(void);
extern void NNS_G2dPushMtx(void);
extern void NNS_G2dPopMtx(int);

extern int NNS_G2dIsRndCoreFlipH(Core *);
extern int NNS_G2dIsRndCoreFlipV(Core *);
extern void DrawCellAnimationImpl_(Anim *);
extern void DrawNode_(Node *);

#pragma opt_lifetimes off

void NNS_G2dDrawMultiCell(const NNSG2dMultiCellInstance *instance) {
    struct {
        int y;
        const Multi *p;
    } locals;

    locals.y = 0;
    locals.p = (const Multi *)instance;
    Hier *h;
    Anim *animations;
    u16 index, i;
    int x;

    if (locals.p->type == 1) {
        {
            u16 j;
            for (j = 0; j < locals.p->data->animations; j++) {
                _021DA068.values[j] = locals.y;
            }
        }
        _021D94D8.drawing = 1;
        i = 0;
        if (_021D94D8.core->step) {
            int old = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
            NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(1);
            NNSi_G2dSetOamSoftEmuAutoZOffsetStep(_021D94D8.core->step);
            {
                for (; i < locals.p->data->count; i++) {
                    animations = locals.p->instances;
                    h = &locals.p->data->hier[i];
                    index = (h->attr & 0xff00) >> 8;
                    _021D9FD8.index = index;
                    NNS_G2dPushMtx();
                    locals.y = h->y;
                    x = h->x;
                    if (NNS_G2dIsRndCoreFlipH(_021D94D8.core)) {
                        x = -x;
                    }
                    if (NNS_G2dIsRndCoreFlipV(_021D94D8.core)) {
                        locals.y = -locals.y;
                    }
                    NNS_G2dTranslate(x << 12, locals.y << 12, 0);
                    DrawCellAnimationImpl_(&animations[index]);
                    NNS_G2dPopMtx(1);
                }
            }
            NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(0);
            NNSi_G2dSetOamSoftEmuAutoZOffsetStep(old);
            NNSi_G2dResetOamSoftEmuAutoZOffset();
        } else {
            for (; i < locals.p->data->count; i++) {
                animations = locals.p->instances;
                h = &locals.p->data->hier[i];
                index = (h->attr & 0xff00) >> 8;
                _021D9FD8.index = index;
                NNS_G2dPushMtx();
                locals.y = h->y;
                x = h->x;
                if (NNS_G2dIsRndCoreFlipH(_021D94D8.core)) {
                    x = -x;
                }
                if (NNS_G2dIsRndCoreFlipV(_021D94D8.core)) {
                    locals.y = -locals.y;
                }
                NNS_G2dTranslate(x << 12, locals.y << 12, 0);
                DrawCellAnimationImpl_(&animations[index]);
                NNS_G2dPopMtx(1);
            }
        }
        _021D94D8.drawing = 0;
    } else {
        Node *nodes = locals.p->instances;
        if (_021D94D8.core->step) {
            int old = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
            u16 i;
            NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(1);
            NNSi_G2dSetOamSoftEmuAutoZOffsetStep(_021D94D8.core->step);
            for (i = 0; i < locals.p->data->count; i++) {
                DrawNode_(&nodes[i]);
            }
            NNSi_G2dSetOamSoftEmuAutoZOffsetFlag(0);
            NNSi_G2dSetOamSoftEmuAutoZOffsetStep(old);
            NNSi_G2dResetOamSoftEmuAutoZOffset();
        } else {
            u16 i;
            for (i = 0; i < locals.p->data->count; i++) {
                DrawNode_(&nodes[i]);
            }
        }
    }
}
