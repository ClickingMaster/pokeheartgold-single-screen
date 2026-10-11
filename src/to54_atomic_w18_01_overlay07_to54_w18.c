#include "to54_atomic_w18_overlay07_to54_w18_private.h"

typedef struct {
    s16 value;
    u8 pad02[6];
    int speed;
    int velocity;
    u8 pad10[0x14];
} Movement;

typedef struct {
    void *owner;
    u8 pad04[8];
    void *pic;
    u8 movement[0x24];
    int count;
    int unused38;
    int attr;
    void *other;
} WorkA;

typedef struct {
    void *owner;
    void *unk4;
    void *spriteSystem;
    u32 state;
    int timer;
    void *sprite;
    u8 move0[0x24];
    u8 move1[0x28];
    u32 animState;
    int animCount;
    int repeats;
    int duration;
    int x;
    int y;
    int substate;
} WorkB;

typedef struct {
    int state;
    void *owner;
    void *pic;
    s16 x;
    s16 y;
    u8 movement[0x24];
    int count;
} WorkC;

typedef struct {
    int state;
    void *owner;
    int multiplier;
    void *pic1;
    s16 x1;
    s16 y1;
    Movement movement1;
    void *pic2;
    s16 x2;
    s16 y2;
    Movement movement2;
} WorkD;

void ov07_0222E6A8(void *, void *);
void ov07_0222E6F4(void *);
void ov07_0222E754(WorkB *);
BOOL ov07_0222E7BC(WorkB *);
BOOL ov07_0222E894(WorkB *);
void ov07_0222EA48(void *, void *, void *, void *);
void ov07_0222EAB4(WorkC *);
void ov07_0222EB6C(void *);
void ov07_0222ED7C(void *);
static void ov07_0222E90C(void *, void *);
static void ov07_0222EB00(void *, void *);
static void ov07_0222EBB0(void *, void *);

void ov07_0222E6F4(void *owner) {
    WorkA *work = BattleAnimUtil_AllocWork(owner, sizeof(WorkA));
    work->owner = owner;
    work->pic = ov07_0221FA48(work->owner, ov07_0221C468(owner));
    ov07_02222508(work->movement, 10, 10, 15, 8);
    work->count = 4;
    work->other = ov07_0221FAA0(work->owner, ov07_0221C468(work->owner));
    work->attr = Pokepic_GetAttr(work->pic, 1);
    ov07_0221C410(work->owner, ov07_0222E6A8, work);
}

void ov07_0222E754(WorkB *work) {
    ov07_02222508(work->move0, work->x / 100, 10, work->y / 100, work->duration / 100);
    BattleAnimUtil_SetBlendWithBattleBg(work->owner, 16, 0);
    ov07_02222AC4(work->move1, 16, 10, 0, 6, (work->duration / 100) * 2);
    work->animState = 0;
    work->animCount = 0;
}

BOOL ov07_0222E7BC(WorkB *work) {
    BOOL done = FALSE;
    switch (work->animState) {
    case 0:
        if (!ov07_02222748(work->move0, work->sprite)) {
            ov07_02222508(work->move0, work->y / 100, 10, work->x / 100, work->duration / 100);
            work->animState++;
        }
        break;
    case 1:
        if (!ov07_02222748(work->move0, work->sprite)) {
            work->animCount++;
            if (work->animCount < 2) {
                work->animState = 0;
                ov07_02222508(work->move0, work->x / 100, 10, work->y / 100, work->duration / 100);
            } else {
                work->animState++;
            }
        }
        break;
    case 2:
        if (ov07_02222AF4(work->move1)) {
            done = TRUE;
            work->animState++;
        }
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}

BOOL ov07_0222E894(WorkB *work) {
    BOOL done = FALSE;
    switch (work->substate) {
    case 0:
        ov07_0222E754(work);
        ov07_0222E7BC(work);
        work->substate++;
        break;
    case 1:
        if (ov07_0222E7BC(work)) {
            work->repeats--;
            if (work->repeats >= 0) {
                work->duration -= work->duration / 3;
                work->x -= work->x / 3;
                work->y -= work->y / 3;
                work->substate = 0;
            } else {
                done = TRUE;
                work->substate++;
            }
        }
        break;
    case 2:
        done = TRUE;
        break;
    }
    return done;
}

static void ov07_0222E90C(void *task, void *data) {
    WorkB *work = data;
    switch (work->state) {
    case 0:
        if (ov07_0222E894(work)) {
            work->state++;
            work->timer = 30;
            ManagedSprite_SetAnim(work->sprite, 0);
            ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
            ManagedSprite_SetAffineScale(work->sprite, 1.0f, 1.0f);
            ov07_02222AC4(work->move1, 0, 16, 16, 0, 4);
        }
        break;
    case 1:
        if (work->timer == 0) {
            StartBrightnessTransition(8, 16, 0, 13, 1);
            work->timer = -1;
        } else if (work->timer > 0) {
            work->timer--;
        }
        if (work->timer == -1 && IsBrightnessTransitionActive(1)) {
            StartBrightnessTransition(8, 0, 16, 13, 1);
            work->timer = -2;
        }
        if (!ManagedSprite_IsAnimated(work->sprite)) {
            work->state++;
        }
        break;
    case 2:
        if (IsBrightnessTransitionActive(1)) {
            work->state++;
            BattleAnimUtil_SetBlendWithBattleBg(work->owner, 16, 0);
            ov07_02222AC4(work->move1, 16, 0, 0, 16, 8);
        }
        break;
    case 3:
        if (ov07_02222AF4(work->move1)) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            work->state++;
        }
        break;
    case 4:
        Sprite_DeleteAndFreeResources(work->sprite);
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteSystem);
}

void ov07_0222EA48(void *owner, void *spriteSystem, void *arg, void *sprite) {
    WorkB *work = BattleAnimUtil_AllocWork(owner, sizeof(WorkB));
    work->owner = owner;
    work->unk4 = spriteSystem;
    work->spriteSystem = arg;
    work->sprite = sprite;
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(work->sprite, 2);
    ManagedSprite_SetOamMode(work->sprite, 1);
    ManagedSprite_SetAnimationFrame(work->sprite, 4);
    work->repeats = 3;
    work->duration = 400;
    work->x = 1500;
    work->y = 1000;
    ov07_0221C410(work->owner, ov07_0222E90C, work);
}

void ov07_0222EAB4(WorkC *work) {
    switch (work->count) {
    case 0:
    case 2:
        ov07_02222268(work->movement, 0, 0, 0, 32, 6);
        break;
    case 1:
    case 3:
        ov07_02222268(work->movement, 0, 0, 32, 0, 6);
        break;
    }
}

static void ov07_0222EB00(void *task, void *data) {
    WorkC *work = data;
    switch (work->state) {
    case 0:
        ov07_0222EAB4(work);
        work->state++;
        break;
    case 1:
        if (ov07_022222B4(work->movement)) {
            ov07_022220B8(work->movement, work->pic, work->x, work->y);
            break;
        }
        work->count++;
        if (work->count < 4) {
            work->state = 0;
        } else {
            work->state++;
        }
        break;
    case 2:
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        break;
    }
}

void ov07_0222EB6C(void *owner) {
    WorkC *work = BattleAnimUtil_AllocWork(owner, sizeof(WorkC));
    work->owner = owner;
    work->pic = ov07_0221FA48(work->owner, ov07_0221C468(owner));
    work->x = Pokepic_GetAttr(work->pic, 0);
    work->y = Pokepic_GetAttr(work->pic, 1);
    work->count = 0;
    ov07_0221C410(work->owner, ov07_0222EB00, work);
}

static void ov07_0222EBB0(void *task, void *data) {
    WorkD *work = data;
    switch (work->state) {
    case 0:
        ov07_02222A44(&work->movement1, 1, 64);
        work->movement1.velocity *= work->multiplier;
        work->state++;
        break;
    case 1:
        if (ov07_02222240(&work->movement1, work->x1, work->y1 + 8, work->pic1)) {
            break;
        }
        Pokepic_SetAttr(work->pic1, 0, work->x1);
        Pokepic_SetAttr(work->pic1, 1, work->y1);
        ov07_02222268(&work->movement1, work->x1, work->x1 + 32, 0, 0, 2);
        work->movement1.speed *= work->multiplier;
        work->state++;
        ov07_022222B4(&work->movement1);
        Pokepic_SetAttr(work->pic1, 0, work->movement1.value);
        break;
    case 2:
        if (ov07_022222B4(&work->movement1)) {
            Pokepic_SetAttr(work->pic1, 0, work->movement1.value);
            break;
        }
        ov07_02222268(&work->movement2, work->x2, work->x2 + (work->multiplier << 5), 0, 0, 2);
        work->state++;
        break;
    case 3:
        if (ov07_022222B4(&work->movement2)) {
            Pokepic_SetAttr(work->pic2, 0, work->movement2.value);
            break;
        }
        ov07_02222268(&work->movement1, work->movement1.value, work->x1, 0, 0, 8);
        work->state++;
        break;
    case 4:
        if (ov07_022222B4(&work->movement1)) {
            Pokepic_SetAttr(work->pic1, 0, work->movement1.value);
            break;
        }
        Pokepic_SetAttr(work->pic1, 0, work->x1);
        ov07_02222268(&work->movement2, work->movement2.value, work->x2, 0, 0, 8);
        work->state++;
        break;
    case 5:
        if (ov07_022222B4(&work->movement2)) {
            Pokepic_SetAttr(work->pic2, 0, work->movement2.value);
            break;
        }
        Pokepic_SetAttr(work->pic2, 0, work->x2);
        work->state++;
        break;
    case 6:
        ov07_0221C448(work->owner, task);
        Heap_Free(work);
        break;
    }
}

void ov07_0222ED7C(void *owner) {
    WorkD *work = BattleAnimUtil_AllocWork(owner, sizeof(WorkD));
    work->owner = owner;
    work->multiplier = (int)ov07_02222004(work->owner, ov07_0221C468(owner));
    work->pic1 = ov07_0221FA48(work->owner, ov07_0221C468(owner));
    work->x1 = Pokepic_GetAttr(work->pic1, 0);
    work->y1 = Pokepic_GetAttr(work->pic1, 1);
    work->pic2 = ov07_0221FA48(work->owner, ov07_0221C470(owner));
    work->x2 = Pokepic_GetAttr(work->pic2, 0);
    work->y2 = Pokepic_GetAttr(work->pic2, 1);
    ov07_0221C410(work->owner, ov07_0222EBB0, work);
}
