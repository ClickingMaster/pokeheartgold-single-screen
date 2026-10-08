#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad0[0x1c];
    fx32 angle;
    u8 pad20[0xb0 - 0x20];
} Ov85Obj;
typedef struct {
    u8 pad0[0x140];
    Ov85Obj objs[5];
} Ov85ObjSys;
typedef struct {
    u8 pad0[0x2c];
    int idx;
    int count;
    u8 pad34[0x190 - 0x34];
    Ov85ObjSys sys;
} Ov85Work;
extern const u16 ov85_021EA7C4[][5];
Ov85Obj *ov85_021E7958(Ov85Work *w);
// @fn ov85_021E7958
Ov85Obj *ov85_021E7958(Ov85Work *w) {
    int dist[5];
    int order[5];
    int j;
    int keyDist;
    int key;
    int i;
    fx32 target;
    fx32 d;
    int n;
    Ov85ObjSys *sys;
    n = w->count;
    sys = &w->sys;
    i = 0;
    target = ov85_021EA7C4[n][w->idx] << 12;
    do {
        order[i] = i;
        dist[i] = 0xFFFF;
        i++;
    } while (i < n);
    for (i = 0; i < n; i++) {
        d = target - sys->objs[i].angle;
        if (d < 0) {
            d = target + FX32_CONST(360) - sys->objs[i].angle;
        }
        dist[i] = d / FX32_ONE;
    }
    for (i = 1; i < n; i++) {
        key = order[i];
        keyDist = dist[key];
        for (j = i - 1; j >= 0 && dist[order[j]] > keyDist; j--) {
            order[j + 1] = order[j];
        }
        order[j + 1] = key;
    }
    return &sys->objs[order[0]];
}
