#include "to57_overlay07_23240_private.h"

typedef struct Ov07Work23240 {
    int count;
    u8 init[4];
    void *owner;
    u8 pad0C[4];
    void *spriteSystem;
    u8 pad14[4];
    void *bgConfig;
    u8 pad1C[4];
    s16 arg0;
    s16 arg1;
    s16 arg2;
    s16 arg3;
    u32 flags;
    struct {
        s16 unk00;
        s16 unk02;
        s16 x;
        s16 y;
        void *pic;
        void *sprite;
        u32 unk10;
    } slots[4];
    s16 offsetX;
    s16 offsetY;
    u8 tail[0x20];
} Ov07Work23240;

typedef struct Ov07FadeWork {
    u32 unk00;
    int done;
    u16 unk08;
    s16 x;
    s16 y;
    s16 dx;
    s16 dy;
    u16 limit;
    u16 counter;
    u16 eva;
    u16 evb;
    u8 pad1A[2];
    u8 state;
    u8 pad1D[3];
    void *owner;
    u8 pad24[0x0c];
    void *bgConfig;
    void *palette;
    u8 tail[0x80];
} Ov07FadeWork;

void ov07_02223240(void *task, Ov07Work23240 *work) {
    int i;
    switch (*(u8 *)work->init) {
    case 0:
        ov07_022227A8(&work->offsetX, work->arg0, work->arg1, work->arg2, work->arg3);
        (*(u8 *)work->init)++;
        return;
    case 1:
        if (ov07_022227D8(&work->offsetX) == 0) {
            (*(u8 *)work->init)++;
            return;
        }
        if ((work->flags & 0x100) == 0x100) {
            for (i = 0; i < work->count; i++) {
                if (work->slots[i].pic != NULL) {
                    Pokepic_SetAttr(work->slots[i].pic, 0, work->offsetX + work->slots[i].x);
                    Pokepic_SetAttr(work->slots[i].pic, 1, work->offsetY + work->slots[i].y);
                }
            }
        } else if ((work->flags & 0x200) == 0x200) {
            for (i = 0; i < work->count; i++) {
                if (work->slots[i].sprite != NULL) {
                    ManagedSprite_SetPositionXY(work->slots[i].sprite,
                        work->offsetX + work->slots[i].x,
                        work->offsetY + work->slots[i].y);
                    ManagedSprite_TickFrame(work->slots[i].sprite);
                }
            }
            SpriteSystem_DrawSprites(work->spriteSystem);
        }
        if ((work->flags & 0x400) == 0x400) {
            BgSetPosTextAndCommit(work->bgConfig, 3, 0, work->offsetX);
        }
        return;
    default:
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }
}

void ov07_02223350(void *owner) {
    Ov07Work23240 *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07Work23240));
    BattleAnimUtil_InitCommonWork(owner, work->init);
    work->arg0 = ((s32)BattleAnimSystem_GetArg(owner, 0));
    work->arg1 = ((s32)BattleAnimSystem_GetArg(owner, 1));
    work->arg2 = ((s32)BattleAnimSystem_GetArg(owner, 2));
    work->arg3 = ((s32)BattleAnimSystem_GetArg(owner, 3));
    work->flags = ((s32)BattleAnimSystem_GetArg(owner, 4));
    if ((work->flags & 0x100) == 0x100) {
        BattleAnimUtil_CollectPicsByFlags(owner, work->flags, work->slots, work);
    } else if ((work->flags & 0x200) == 0x200) {
        BattleAnimUtil_CollectSpritesByFlags(owner, work->flags, work->slots, work);
    }
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_02223240, work);
}

void ov07_022233D8(void *task, Ov07FadeWork *work) {
    int count;
    switch (work->state) {
    case 0:
        G2x_SetBlendAlpha_(0x04000050, 4, 0x39, work->eva, work->evb);
        ToggleBgLayer(2, 1);
        work->state++;
    case 1:
        count = 0;
        if (work->eva < work->unk08 - 2) {
            work->eva += 2;
        } else {
            count++;
        }
        if (work->evb > 9) {
            work->evb -= 2;
        } else {
            count++;
        }
        if (count == 2) {
            work->eva = work->unk08;
            work->evb = 7;
            work->state++;
        }
        *(vu16 *)0x04000052 = work->eva | (work->evb << 8);
        break;
    case 2:
        if (work->done != 0) {
            work->state++;
        }
        break;
    case 3:
        count = 0;
        if (work->eva > 2) {
            work->eva -= 2;
        } else {
            count++;
        }
        if (work->evb < 0x1d) {
            work->evb += 2;
        } else {
            count++;
        }
        if (count == 2) {
            work->eva = 0;
            work->evb = 0x1f;
            work->state++;
        }
        *(vu16 *)0x04000052 = work->eva | (work->evb << 8);
        break;
    default:
        ToggleBgLayer(2, 0);
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }

    if ((work->dy > 0 && work->y >= 0x200) || (work->dy < 0 && work->y <= -0x19c)) {
        work->done = 1;
    }
    work->x += work->dx;
    work->y += work->dy;
    if (work->limit < work->counter) {
        if (work->dy < 0) {
            work->dy++;
        } else {
            work->dy = 0;
        }
        if (work->dx < 0) {
            work->dx++;
        } else {
            work->dx = 0;
        }
        work->counter = 0;
    } else {
        work->counter++;
    }
    BgSetPosTextAndCommit(work->bgConfig, 2, 0, work->x);
    BgSetPosTextAndCommit(work->bgConfig, 2, 3, work->y);
}

void ov07_02223550(void *owner) {
    int narc;
    int member;
    Ov07FadeWork *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07FadeWork));
    BattleAnimUtil_InitCommonWork(owner, &work->state);
    work->x = ((s32)BattleAnimSystem_GetArg(owner, 1));
    work->y = ((s32)BattleAnimSystem_GetArg(owner, 2));
    work->dx = ((s32)BattleAnimSystem_GetArg(owner, 3));
    work->dy = ((s32)BattleAnimSystem_GetArg(owner, 4));
    work->unk00 = ((s32)BattleAnimSystem_GetArg(owner, 5));
    work->unk08 = ((s32)BattleAnimSystem_GetArg(owner, 6));
    work->limit = ((s32)BattleAnimSystem_GetArg(owner, 7));
    work->counter = 0;
    if (work->unk00 != 0 && BattleAnimUtil_GetBattlerSide(owner, BattleAnimSystem_GetAttacker(owner)) == 4) {
        work->x *= -1;
        work->y *= -1;
        work->dx *= -1;
        work->dy *= -1;
        work->y -= 0x54;
    } else {
        work->y += 0x54;
    }
    work->eva = 0;
    work->evb = 0x1f;
    work->done = 0;
    ToggleBgLayer(2, 0);
    narc = ((s32)BattleAnimSystem_GetArg(owner, 0));
    member = BattleAnimSystem_GetBackgroundFile(narc, 0);
    GfGfxLoader_LoadCharData(7, member, work->bgConfig, 2, 0, 0, 1, BattleAnimSystem_GetHeapID(owner));
    member = BattleAnimSystem_GetBackgroundFile(narc, 1);
    PaletteData_LoadNarc(work->palette, 7, member, BattleAnimSystem_GetHeapID(owner), 0, 0x20, 0x90);
    BgClearTilemapBufferAndCommit(work->bgConfig, 2);
    member = 2;
    if (ov07_0221BFC0(owner) == 1) {
        member = 4;
    } else if (BattleAnimUtil_GetBattlerSide(owner, BattleAnimSystem_GetAttacker(owner)) == 4) {
        member = 3;
    }
    member = BattleAnimSystem_GetBackgroundFile(narc, member);
    GfGfxLoader_LoadScrnData(7, member, work->bgConfig, 2, 0, 0, 1, BattleAnimSystem_GetHeapID(owner));
    BgSetPosTextAndCommit(work->bgConfig, 2, 0, work->x);
    BgSetPosTextAndCommit(work->bgConfig, 2, 3, work->y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_022233D8, work);
}

void ov07_022236F0(void *task, Ov07FadeWork *work) {
    int count;
    switch (work->state) {
    case 0:
        G2x_SetBlendAlpha_(0x04000050, 4, 0x39, work->eva, work->evb);
        ToggleBgLayer(2, 1);
        work->state++;
    case 1:
        count = 0;
        if (work->eva < 0x10) {
            work->eva += 2;
        } else {
            count++;
        }
        if (work->evb > 4) {
            work->evb -= 2;
        } else {
            count++;
        }
        if (count == 2) {
            work->eva = 0x10;
            work->evb = 4;
            work->state++;
        }
        *(vu16 *)0x04000052 = work->eva | (work->evb << 8);
        break;
    case 2:
        if (work->done != 0) {
            work->state++;
        }
        break;
    case 3:
        count = 0;
        if (work->eva > 2) {
            work->eva -= 2;
        } else {
            count++;
        }
        if (work->evb < 0x10) {
            work->evb += 2;
        } else {
            count++;
        }
        if (count == 2) {
            work->eva = 0;
            work->evb = 0x1f;
            work->state++;
        }
        *(vu16 *)0x04000052 = work->eva | (work->evb << 8);
        break;
    default:
        ToggleBgLayer(2, 0);
        BattleAnimSystem_DestroyTask(work->owner, task);
        Heap_Free(work);
        return;
    }

    if ((work->dy > 0 && work->y >= 0x200) || (work->dy < 0 && work->y <= -0x19c)) {
        work->done = 1;
    }
    work->x += work->dx;
    work->y += work->dy;
    if (work->limit < work->counter) {
        if (work->dy < 0) {
            work->dy++;
        } else {
            work->dy = 0;
        }
        if (work->dx < 0) {
            work->dx++;
        } else {
            work->dx = 0;
        }
        work->counter = 0;
    } else {
        work->counter++;
    }
    BgSetPosTextAndCommit(work->bgConfig, 2, 0, work->x);
    BgSetPosTextAndCommit(work->bgConfig, 2, 3, work->y);
}

void ov07_02223864(void *owner) {
    int narc;
    int member;
    Ov07FadeWork *work = BattleAnimUtil_AllocWork(owner, sizeof(Ov07FadeWork));
    BattleAnimUtil_InitCommonWork(owner, &work->state);
    work->x = ((s32)BattleAnimSystem_GetArg(owner, 1));
    work->y = ((s32)BattleAnimSystem_GetArg(owner, 2));
    work->dx = ((s32)BattleAnimSystem_GetArg(owner, 3));
    work->dy = ((s32)BattleAnimSystem_GetArg(owner, 4));
    work->unk00 = ((s32)BattleAnimSystem_GetArg(owner, 5));
    work->unk08 = ((s32)BattleAnimSystem_GetArg(owner, 6));
    work->limit = ((s32)BattleAnimSystem_GetArg(owner, 7));
    work->counter = 0;
    if (work->unk00 != 0 && BattleAnimUtil_GetBattlerSide(owner, BattleAnimSystem_GetAttacker(owner)) == 4) {
        work->x *= -1;
        work->y *= -1;
        work->dx *= -1;
        work->dy *= -1;
        work->y -= 0x54;
    } else {
        work->y += 0x54;
    }
    if (ov07_0221BFC0(owner) == 1) {
        work->dx *= -1;
    }
    work->eva = 4;
    work->evb = 0x10;
    work->eva = 0;
    work->evb = 0x10;
    work->done = 0;
    ToggleBgLayer(2, 0);
    narc = ((s32)BattleAnimSystem_GetArg(owner, 0));
    member = BattleAnimSystem_GetBackgroundFile(narc, 0);
    GfGfxLoader_LoadCharData(7, member, work->bgConfig, 2, 0, 0, 1, BattleAnimSystem_GetHeapID(owner));
    member = BattleAnimSystem_GetBackgroundFile(narc, 1);
    PaletteData_LoadNarc(work->palette, 7, member, BattleAnimSystem_GetHeapID(owner), 0, 0x20, 0x90);
    BgClearTilemapBufferAndCommit(work->bgConfig, 2);
    member = 2;
    if (ov07_0221BFC0(owner) == 1) {
        member = 4;
    } else if (BattleAnimUtil_GetBattlerSide(owner, BattleAnimSystem_GetAttacker(owner)) == 4) {
        member = 3;
    }
    member = BattleAnimSystem_GetBackgroundFile(narc, member);
    GfGfxLoader_LoadScrnData(7, member, work->bgConfig, 2, 0, 0, 1, BattleAnimSystem_GetHeapID(owner));
    BgSetPosTextAndCommit(work->bgConfig, 2, 0, work->x);
    BgSetPosTextAndCommit(work->bgConfig, 2, 3, work->y);
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_022236F0, work);
}
