#include "global.h"

#include "heap.h"

typedef void BgConfig;
typedef void PaletteData;
u8 PaletteData_ForceBeginPaletteFade(PaletteData *, u16, u16, s8, u8, u8, u16);
u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *);
void PaletteData_BlendPalettes(PaletteData *, int, u16, u8, u16);
void PaletteData_LoadNarc(PaletteData *, int, int, int, int, u32, u16);
void SetBgControlParam(BgConfig *, u8, int, u8);
void BgSetPosTextAndCommit(BgConfig *, u8, int, int);
void SetBgPriority(u8, u8);
void ToggleBgLayer(u8, u8);
u32 GfGfxLoader_LoadCharData(int, int, BgConfig *, int, int, int, int, int);
void GfGfxLoader_LoadScrnData(int, int, BgConfig *, int, int, int, int, int);

#define OWNER(w) ((Ov07Owner *)(w)->owner)

typedef struct Ov07FadeWork {
    u32 state;
    u8 pad04;
    u8 step;
    u8 pad06[3];
    u8 eva;
    u8 evb;
    u8 evaTarget;
    u8 evbTarget;
    u8 reverse;
    s8 firstDelay;
    s8 secondDelay;
    u32 data10;
    u8 pad14[8];
    void *items[10];
    u8 pad44[4];
    void *owner;
} Ov07FadeWork;

typedef struct Ov07Owner {
    u32 heapId;
    u8 pad004[0x18 - 4];
    u32 flags;
    u8 pad01C[0x94 - 0x1C];
    void *items[10];
    u8 pad0BC[0xC4 - 0xBC];
    BgConfig *bgConfig;
    PaletteData *palette;
    u8 pad0CC[0x190 - 0xCC];
    int charNarc;
    int charMember;
    int screenNarc;
    int screenMember;
    int paletteMember;
    int paletteNarc;
    u32 paletteMask;
} Ov07Owner;

typedef void (*Ov07Callback)(void *);
typedef struct Ov07MaskTable {
    u32 masks[6];
} Ov07MaskTable;

extern const Ov07MaskTable ov07_02234BA8;
extern const Ov07Callback ov07_02234C40[];

BOOL ov07_0221DEB0(u32, u32);
void GF_AssertFail(void);

extern u32 ov07_0221C4A8(void *p, int i);
void ov07_0221EB98(void *, int);
u8 ov07_0221FB04(void *, int);
int ov07_0221BFC0(void *);
void ov07_0221DE04(void *, void *, int, void *);
void ov07_0221DEC0(void *);
void ov07_0221E664(void *);
void ov07_0221FB30(void *, int);
void ov07_0221FB58(void *);
void ov07_0221EC7C(void *, int);
void BattleSystem_SetDefaultBlend(void);
void G2x_SetBlendAlpha_(u32, int, int, int, int);
Ov07FadeWork *ov07_0221DF1C(Ov07Owner *);
int ov07_0221DFB4(void *, void *);
int ov07_0221E280(void *, void *);
int ov07_0221E0B4(void *, void *);
int ov07_0221E3B8(void *, void *);

void ov07_0221DEC0(void *arg) {
    Ov07Owner *owner = arg;
    Ov07MaskTable table = ov07_02234BA8;
    u32 i;

    for (i = 0; i < 6; i++) {
        if (ov07_0221DEB0(owner->flags, table.masks[i])) {
            int value = table.masks[i] >> 16;
            int index = 0;
            while (value >= 2) {
                value /= 2;
                index++;
            }
            ov07_02234C40[index](owner);
        }
    }
}

Ov07FadeWork *ov07_0221DF1C(Ov07Owner *owner) {
    Ov07FadeWork *work = Heap_Alloc((enum HeapID)owner->heapId, sizeof(Ov07FadeWork));
    int i;

    if (work == NULL) {
        GF_AssertFail();
        return NULL;
    }
    memset(work, 0, sizeof(Ov07FadeWork));
    work->state = 0;
    work->step = 0;
    work->secondDelay = 0;
    work->firstDelay = 0;
    work->owner = owner;
    work->eva = 0;
    work->evb = 31;
    work->evaTarget = 29;
    work->evbTarget = 2;
    if (((s32)ov07_0221C4A8(owner, 5)) == 1) {
        work->eva = 0;
        work->evb = 31;
        work->evaTarget = 15;
        work->evbTarget = 7;
    }
    if (((s32)ov07_0221C4A8(owner, 5)) == 2) {
        work->eva = 7;
        work->evb = 15;
        work->evaTarget = 29;
        work->evbTarget = 2;
    }
    for (i = 0; i < 10; i++) {
        work->items[i] = owner->items[i];
    }
    ((u8 *)owner)[0x17C] = 1;
    return work;
}

int ov07_0221DFB4(void *unused, void *arg) {
    int done;
    int priority;
    Ov07FadeWork *work = arg;

    switch (work->step) {
    case 0:
        ov07_0221EB98(work->owner, 2);
        priority = ov07_0221FB04(work->owner, 2);
        ov07_0221FB04(work->owner, 1);
        SetBgPriority(3, (u8)priority);
        SetBgPriority(2, (u8)priority);
        ToggleBgLayer(2, 1);
        work->step++;
        goto return_one;
    case 1:
        SetBgControlParam(OWNER(work)->bgConfig, 3, 2, 3);
        if (ov07_0221BFC0(work->owner) != 1) {
            SetBgControlParam(OWNER(work)->bgConfig, 3, 0, 0);
        }
        ov07_0221DE04(work, work->owner, 3, (void *)work->data10);
        G2x_SetBlendAlpha_(0x04000050, 4, 8, work->evb, work->eva);
        ov07_0221DEC0(work);
        work->step++;
        /* fallthrough */
    case 2:
        done = 0;
        if (work->eva < work->evaTarget) {
            work->eva += 2;
        } else {
            done++;
        }
        if (work->evb > work->evbTarget) {
            work->evb -= 2;
        } else {
            done++;
        }
        if (done == 2) {
            work->eva = work->evaTarget;
            work->evb = work->evbTarget;
            work->step++;
        }
        *(vu16 *)0x04000052 = work->evb | (work->eva << 8);
        if (work->step == 2) {
            goto return_one;
        }
        return 0;
    default:
        return 0;
    }
return_one:
    return 1;
}

int ov07_0221E0B4(void *unused, void *arg) {
    Ov07FadeWork *work = arg;
    int done;
    int pri2;
    int pri1;

    switch (work->step) {
    case 0:
        ToggleBgLayer(2, 1);
        work->step++;
        /* fallthrough */
    case 1:
        pri2 = ov07_0221FB04(work->owner, 2);
        pri1 = ov07_0221FB04(work->owner, 1);
        SetBgPriority(3, (u8)pri2);
        SetBgPriority(2, (u8)pri1);
        G2x_SetBlendAlpha_(0x04000050, 4, 8, work->eva, work->evb);
        ov07_0221DEC0(work);
        work->step++;
        /* fallthrough */
    case 2:
        done = 0;
        if (work->eva < work->evaTarget) {
            work->eva += 2;
        } else {
            done++;
        }
        if (work->evb > work->evbTarget) {
            work->evb -= 2;
        } else {
            done++;
        }
        if (done == 2) {
            work->eva = work->evaTarget + 2;
            work->evb = work->evbTarget - 2;
            work->step++;
        }
        *(vu16 *)0x04000052 = work->eva | (work->evb << 8);
        goto active;
    case 3:
        ov07_0221E664(work);
        BgSetPosTextAndCommit(OWNER(work)->bgConfig, 3, 0, 0);
        BgSetPosTextAndCommit(OWNER(work)->bgConfig, 3, 3, 0);
        SetBgControlParam(OWNER(work)->bgConfig, 3, 2, 4);
        if (ov07_0221BFC0(work->owner) == 0) {
            SetBgControlParam(OWNER(work)->bgConfig, 3, 0, 1);
            ov07_0221FB30(work->owner, 3);
            ov07_0221FB58(work->owner);
        } else {
            GfGfxLoader_LoadCharData(OWNER(work)->charNarc, OWNER(work)->charMember, OWNER(work)->bgConfig, 3, 0, 0, 1, OWNER(work)->heapId);
            PaletteData_LoadNarc(OWNER(work)->palette, OWNER(work)->charNarc, OWNER(work)->screenNarc, OWNER(work)->heapId, 0, OWNER(work)->paletteNarc << 5, (u16)OWNER(work)->paletteMember);
        }
        GfGfxLoader_LoadScrnData(OWNER(work)->charNarc, OWNER(work)->screenMember, OWNER(work)->bgConfig, 3, 0, 0, 1, OWNER(work)->heapId);
        work->step++;
        goto active;
    case 4:
        BattleSystem_SetDefaultBlend();
        ToggleBgLayer(2, 0);
        ov07_0221EC7C(work->owner, 2);
        work->step++;
        goto active;
    default:
        return 0;
    }
active:
    return 1;
}

int ov07_0221E280(void *unused, void *arg) {
    Ov07FadeWork *work = arg;

    switch (work->step) {
    case 0:
        if (work->reverse == 0) {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, (u16)OWNER(work)->paletteMask, work->firstDelay, 0, 16, 0);
            PaletteData_BlendPalettes(OWNER(work)->palette, 0, 0x200, 16, 0);
        } else {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, (u16)OWNER(work)->paletteMask, work->firstDelay, 0, 16, 0xFFFF);
            PaletteData_BlendPalettes(OWNER(work)->palette, 0, 0x200, 16, 0xFFFF);
        }
        work->step++;
        /* fallthrough */
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(OWNER(work)->palette) != 0) {
            goto fade_active;
        }
        SetBgControlParam(OWNER(work)->bgConfig, 3, 0, 0);
        ov07_0221DE04(work, work->owner, 3, (void *)work->data10);
        if (work->reverse == 0) {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, 0x200, work->firstDelay, 16, 0, 0);
        } else {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, 0x200, work->firstDelay, 16, 0, 0xFFFF);
        }
        ov07_0221DEC0(work);
        ((u8 *)work->owner)[0x17C] = 2;
        work->step++;
        goto fade_active;
    default:
        if (PaletteData_GetSelectedBuffersBitmask(OWNER(work)->palette) == 0) {
            return 0;
        }
        goto fade_active;
    }
fade_active:
    return 1;
}

int ov07_0221E3B8(void *unused, void *arg) {
    Ov07FadeWork *work = arg;

    switch (work->step) {
    case 0:
        ov07_0221DEC0(work);
        work->step++;
        /* fallthrough */
    case 1:
        if (work->reverse == 0) {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, 0x200, work->secondDelay, 0, 16, 0);
            PaletteData_BlendPalettes(OWNER(work)->palette, 0, (u16)OWNER(work)->paletteMask, 16, 0);
        } else {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, 0x200, work->secondDelay, 0, 16, 0xFFFF);
            PaletteData_BlendPalettes(OWNER(work)->palette, 0, (u16)OWNER(work)->paletteMask, 16, 0xFFFF);
        }
        work->step++;
        /* fallthrough */
    case 2:
        if (PaletteData_GetSelectedBuffersBitmask(OWNER(work)->palette) != 0) {
            goto fade_active;
        }
        ov07_0221E664(work);
        ToggleBgLayer(3, 0);
        if (ov07_0221BFC0(work->owner) == 0) {
            SetBgControlParam(OWNER(work)->bgConfig, 3, 0, 1);
            ov07_0221FB30(work->owner, 3);
            ov07_0221FB58(work->owner);
        } else {
            GfGfxLoader_LoadCharData(OWNER(work)->charNarc, OWNER(work)->charMember, OWNER(work)->bgConfig, 3, 0, 0, 1, OWNER(work)->heapId);
            PaletteData_LoadNarc(OWNER(work)->palette, OWNER(work)->charNarc, OWNER(work)->screenNarc, OWNER(work)->heapId, 0, OWNER(work)->paletteNarc << 5, (u16)OWNER(work)->paletteMember);
        }
        GfGfxLoader_LoadScrnData(OWNER(work)->charNarc, OWNER(work)->screenMember, OWNER(work)->bgConfig, 3, 0, 0, 1, OWNER(work)->heapId);
        ToggleBgLayer(3, 1);
        work->step++;
        /* fallthrough */
    case 3:
        BgSetPosTextAndCommit(OWNER(work)->bgConfig, 3, 0, 0);
        BgSetPosTextAndCommit(OWNER(work)->bgConfig, 3, 3, 0);
        if (work->reverse == 0) {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, (u16)OWNER(work)->paletteMask, work->secondDelay, 16, 0, 0);
        } else {
            PaletteData_ForceBeginPaletteFade(OWNER(work)->palette, 1, (u16)OWNER(work)->paletteMask, work->secondDelay, 16, 0, 0xFFFF);
        }
        work->step++;
        /* fallthrough */
    default:
        break;
    }
    if (PaletteData_GetSelectedBuffersBitmask(OWNER(work)->palette) != 0) {
        goto fade_active;
    }
    ((u8 *)work->owner)[0x17C] = 2;
    work->step++;
    return 0;
fade_active:
    return 1;
}
