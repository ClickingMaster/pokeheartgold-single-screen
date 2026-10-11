#include "to57_r2_03_overlay07_motion_group_private.h"

typedef struct R203Sin {
    s16 x, y;
    int duration;
    int angleX;
    int ampX;
    int angleY;
    int ampY;
    int stepX;
    int stepY;
    int unk20;
} R203Sin;

typedef struct R203Work {
    void *owner;
    void *spriteSystem;
    void *spriteManager;
    int state;
    int timer;
    int unk14;
    void *sprites[6];
    R203Sin motions[6];
    u8 pad108[0x28];
    s16 x;
    s16 y;
} R203Work;

void ov07_022312A8(void *, int);
void ov07_02231300(R203Work *);

void ov07_022312A8(void *p, int mul) {
    int i = 0;
    u8 *q = (u8 *)p;
    u32 angle;
    q += 0x30;
    angle = i;
    do {
        ov07_022220FC(q, 0, 0x7fff, 0, 0, 0x32000, 0, 0x30);
        *(u32 *)((u8 *)p + 0x38) += angle;
        i++;
        *(u32 *)((u8 *)p + 0x48) *= mul;
        q += 0x24;
        p = (u8 *)p + 0x24;
        angle += 0x2aaa;
    } while (i < 6);
}

void ov07_02231300(R203Work *work) {
    int i;
    for (i = 0; i < 6; i++) {
        ov07_02222180(&work->motions[i]);
        ManagedSprite_SetPositionXY(work->sprites[i], work->x + work->motions[i].x, work->y);
        if (work->motions[i].angleX >= 0x3fff && work->motions[i].angleX <= 0xbf49) {
            ManagedSprite_SetPriority(work->sprites[i], 1);
        } else {
            ManagedSprite_SetPriority(work->sprites[i], ov07_0221FAE8(work->owner) + 1);
        }
    }
}

void ov07_02231378(void *, R203Work *);
void ov07_02231468(void *, void *, void *, void *);

void ov07_02231378(void *taskOwner, R203Work *work) {
    int i;
    switch (work->state) {
    case 0:
        BattleAnimUtil_SetBlendWithBattleBg(work->owner, 1, 15);
        ov07_02222AC4((u8 *)work + 0x108, 1, 16, 15, 0, 10);
        ov07_022312A8(work, work->unk14);
        ov07_02231300(work);
        work->state++;
        break;
    case 1:
        ov07_02231300(work);
        if (ov07_02222AF4((u8 *)work + 0x108)) {
            work->state++;
            work->timer = 28;
        }
        break;
    case 2:
        ov07_02231300(work);
        work->timer--;
        if (work->timer < 0) {
            work->state++;
            ov07_02222AC4((u8 *)work + 0x108, 16, 1, 0, 15, 10);
        }
        break;
    case 3:
        ov07_02231300(work);
        if (ov07_02222AF4((u8 *)work + 0x108)) {
            work->state++;
        }
        break;
    case 4:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        Heap_Free(work);
        ov07_0221C448(work->owner, taskOwner);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}

void ov07_02231468(void *owner, void *spriteSystem, void *spriteManager, void *firstSprite) {
    u8 template[0x34];
    R203Work *work = BattleAnimUtil_AllocWork(owner, sizeof(R203Work));
    void *pic;
    int i;
    work->owner = owner;
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    pic = ov07_0221FA48(work->owner, ov07_0221C468(owner));
    work->x = Pokepic_GetAttr(pic, 0);
    work->y = Pokepic_GetAttr(pic, 1);
    ov07_0221F9E8(template, owner);
    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->sprites[i] = firstSprite;
        } else {
            work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, template);
        }
        ManagedSprite_SetDrawPriority(work->sprites[i], 100);
        ManagedSprite_SetOamMode(work->sprites[i], 1);
        ManagedSprite_SetAnimateFlag(work->sprites[i], 1);
    }
    work->unk14 = ov07_02222004(work->owner, ov07_0221C468(work->owner));
    ov07_0221C410(work->owner, ov07_02231378, work);
}
