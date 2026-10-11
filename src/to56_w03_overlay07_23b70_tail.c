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

void ov07_02223B70(void *task, void *data) {
    PicMoveWork *w = data;
    if (ov07_02222240(w->anim, w->x, w->y, w->pic) == 0) {
        Pokepic_SetAttr(w->pic, 0, w->x);
        Pokepic_SetAttr(w->pic, 1, w->y - 8);
        Pokepic_SetAttr(w->pic, 9, 0);
        ov07_0221C448(w->owner, task);
        Heap_Free(w);
    }
}

void ov07_02223BC0(void *owner) {
    int kind;
    int value;
    PicMoveWork *w;
    w = Heap_Alloc((enum HeapID)ov07_0221BFD0(owner), sizeof(PicMoveWork));
    w->owner = owner;
    ov07_02222A44(w->anim, (int)(ov07_0221C4A8(owner, 0)), (int)(ov07_0221C4A8(owner, 1)));
    kind = (int)(ov07_0221C4A8(owner, 2));
    switch (kind) {
    case 2:
        w->pic = ov07_0221FA48(w->owner, ov07_0221C468(owner));
        *(int *)&w->anim[0xc] *= -1;
        break;
    case 4:
        value = BattleAnimUtil_GetPartnerBattler(w->owner, ov07_0221C468(w->owner));
        w->pic = ov07_0221FA48(w->owner, value);
        *(int *)&w->anim[0xc] *= -1;
        break;
    case 8:
        w->pic = ov07_0221FA48(w->owner, ov07_0221C470(owner));
        break;
    case 16:
        value = BattleAnimUtil_GetPartnerBattler(w->owner, ov07_0221C470(w->owner));
        w->pic = ov07_0221FA48(w->owner, value);
        break;
    default:
        GF_AssertFail();
        break;
    }
    w->x = Pokepic_GetAttr(w->pic, 0);
    w->y = Pokepic_GetAttr(w->pic, 1);
    w->y += 8;
    ov07_0221C410(w->owner, ov07_02223B70, w);
}

void ov07_02223CAC(void *data, s16 x, s16 y) {
    W *w = data;
    ManagedSprite_SetDrawFlag(w->sprite, TRUE);
    ManagedSprite_SetPositionXY(w->sprite, x, y);
    w->direction = 0;
    ov07_02222508(w->anim, 10, 10, 14, 4);
}

int ov07_02223CE4(void *data) {
    W *w = data;
    if (ov07_02222748(w->anim, w->sprite) == 0) {
        if (w->direction != 0) {
            ManagedSprite_SetDrawFlag(w->sprite, FALSE);
            return TRUE;
        }
        w->direction = 1;
        ov07_02222508(w->anim, 14, 10, 12, 2);
    }
    return FALSE;
}

int ov07_02223D28(void *data) {
    W *w = data;
    int done = FALSE;
    switch (w->substate) {
    case 0:
        ov07_02223CAC(w, w->x + w->multiplier * 24, w->y - 16);
        w->substate++;
        break;
    case 1:
        if (ov07_02223CE4(w)) {
            w->substate++;
        }
        break;
    case 2:
        if (--w->delay < 0) {
            w->substate++;
            ov07_02223CAC(w, w->x - w->multiplier * 24, w->y - 24);
        }
        break;
    case 3:
        if (ov07_02223CE4(w)) {
            w->substate++;
        }
        break;
    case 4:
        done = TRUE;
        break;
    }
    return done;
}

void ov07_02223DC4(void *task, void *data) {
    W *w = data;
    switch (w->state) {
    case 0:
        if (ov07_02223D28(w)) {
            w->state++;
        }
        break;
    case 1:
        Sprite_DeleteAndFreeResources(w->sprite);
        ov07_0221C448(w->owner, task);
        Heap_Free(w);
        return;
    }
    SpriteSystem_DrawSprites(w->spriteSystem);
}
