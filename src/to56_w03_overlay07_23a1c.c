#include "to56_w03_overlay07_private.h"

typedef struct W {
    void *owner; // 00
    u8 unk04[4];
    void *spriteSystem; // 08
    int state;          // 0c
    u8 unk10[4];
    void *sprite;   // 14
    u8 anim[0x24];  // 18
    int multiplier; // 3c
    int direction;  // 40
    int delay;      // 44
    int substate;   // 48
    s16 x;          // 4c
    s16 y;          // 4e
} W;

void ov07_02223CAC(void *, s16, s16);
int ov07_02223CE4(void *);
int ov07_02223D28(void *);
void ov07_02223DC4(void *, void *);
void ov07_02223E08(void *, void *, void *, void *);

typedef struct PicAnimWork {
    u8 state;
    u8 pad01[3];
    void *owner;
    void *pic;
    u8 anim[0x14];
    int unk20;
    int unk24;
} PicAnimWork;

typedef struct PicMoveWork {
    void *owner;
    u8 anim[0x24];
    void *pic;
    s16 x;
    s16 y;
} PicMoveWork;

void ov07_02223A1C(void *task, void *data) {
    PicAnimWork *w = data;
    switch (w->state) {
    case 0:
    case 1:
        if (ov07_02222440(w->anim) == 1) {
            Pokepic_SetAttr(w->pic, 9, (u16) * (u32 *)w->anim);
            return;
        }
        ov07_022223F0(w->anim, *(u32 *)w->anim, w->unk20, w->unk24);
        w->state++;
        return;
    case 2:
        Pokepic_SetAttr(w->pic, 10, 0);
        Pokepic_SetAttr(w->pic, 11, 0);
        Pokepic_SetAttr(w->pic, 9, 0);
        BattleAnimSystem_DestroyTask(w->owner, task);
        Heap_Free(w);
        return;
    }
}
