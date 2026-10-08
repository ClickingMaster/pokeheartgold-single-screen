#pragma force_active on
#include "global.h"

typedef struct Entity {
    u8 pad[0x240];
    u32 lo : 8;
    u32 current : 8;
    u32 pending : 8;
    u32 top : 8;
} Entity;

typedef struct Work {
    u8 unk00[0xB4];
    Entity *objects[128];
    Entity *list[128];
    u8 unk4B4[0x73C - 0x4B4];
    u8 active;
    u8 value;
    u8 flag;
} Work;

typedef struct Dispatch {
    void *update;
    void *tick;
    BOOL (*interact)(Work *, Entity *, Entity *);
} Dispatch;

extern const Dispatch ov89_0225CDB0[];
int ov89_0225AFC0(Entity *e, int state);
BOOL ov89_0225AF10(Work *w, int idx, int val);
void ov89_0225AC24(Work *w, Entity *e, int i);
void ov89_0225AF9C(Work *w);
void ov89_0225A5EC(Work *work);

// @fn ov89_0225A5EC
void ov89_0225A5EC(Work *work) {
    int i;
    int nA;
    int nB;
    int m;
    int j;
    Entity *obj;
    Entity *other;

    nA = 0;
    nB = 0;
    for (i = 0; i < 128; i++) {
        obj = work->objects[i];
        if (obj != NULL) {
            if (obj->current != 0) {
                work->list[nA] = obj;
                nA++;
            } else {
                work->list[127 - nB] = obj;
                nB++;
            }
        }
    }
    for (j = 0; j < nB; j++) {
        other = work->list[127 - j];
        for (m = 0; m < nA; m++) {
            obj = work->list[m];
            if (ov89_0225CDB0[obj->current].interact(work, obj, other) == TRUE && ov89_0225AFC0(other, obj->current) == TRUE && other->top == 0xFF) {
                other->top = obj->top;
                ov89_0225AF10(work, obj->top, obj->current);
                break;
            }
        }
    }
    if (work->value != 0 && work->flag == 1) {
        for (i = 0; i < 128; i++) {
            obj = work->objects[i];
            if (obj != NULL && obj->current != work->value && obj->pending != work->value && ov89_0225AFC0(obj, work->value) == FALSE) {
                ov89_0225AC24(work, obj, i);
            }
        }
        ov89_0225AF9C(work);
    }
}
