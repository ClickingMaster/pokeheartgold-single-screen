#include "global.h"

typedef struct NARC NARC;
typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;
typedef struct ManagedSprite ManagedSprite;

typedef struct MoveArgs {
    u8 pad00[0x10];
    u16 *unk_10;
    u16 unk_14;
    u16 unk_16;
    u8 mode;
} MoveArgs;

typedef struct MoveWork {
    MoveArgs *args;
    u8 pad004[0x11c - 4];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[12];
    u8 pad154[0x1b8 - 0x154];
    u8 count;
} MoveWork;

extern void SpriteSystem_LoadCharResObjFromOpenNarc(SpriteSystem *, SpriteManager *, NARC *, int, int, int, int);
extern void SpriteSystem_LoadPlttResObjFromOpenNarc(SpriteSystem *, SpriteManager *, NARC *, int, int, int, int, int);
extern void SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *, SpriteManager *, NARC *, int, int, int);
extern void SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *, SpriteManager *, NARC *, int, int, int);
extern void SpriteSystem_ReplaceCharResObj(SpriteSystem *, SpriteManager *, int, int, int, int);
extern void sub_020776B8(SpriteSystem *, SpriteManager *, int, int, int);
extern void sub_020776EC(SpriteSystem *, SpriteManager *, int, int);
extern void sub_0207775C(SpriteSystem *, SpriteManager *, int, int);
extern void sub_02077834(SpriteSystem *, SpriteManager *, int, int, int);
extern int GetMoveAttr(u16, int);
extern int sub_020776B4(void);
extern int sub_02077678(int);
extern int sub_0207769C(int);
extern void ManagedSprite_SetPaletteOverride(ManagedSprite *, int);
extern void ManagedSprite_SetPositionXY(ManagedSprite *, s16, s16);
extern void ManagedSprite_SetDrawFlag(ManagedSprite *, int);

void ov68_021E6EB8(MoveWork *work, NARC *narc);
void ov68_021E7028(MoveWork *work, u16 move, u16 index);
void ov68_021E70BC(MoveWork *work);

void ov68_021E6EB8(MoveWork *work, NARC *narc) {
    u32 i;

    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 6, FALSE, 2, 0xB8A8);
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 11, FALSE, 2, 0xB8A9);
    for (i = 0xB8AB; i <= 0xB8B2; i++) {
        if (i < 0xB8AF) {
            sub_020776B8(work->spriteSystem, work->spriteManager, 2, 0, i);
        } else {
            sub_020776B8(work->spriteSystem, work->spriteManager, 1, 0, i);
        }
    }
    sub_02077834(work->spriteSystem, work->spriteManager, 2, 0, 0xB8AA);
    SpriteSystem_LoadPlttResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 5, FALSE, 4, 2, 0xB8A8);
    sub_020776EC(work->spriteSystem, work->spriteManager, 2, 0xB8A9);
    sub_020776EC(work->spriteSystem, work->spriteManager, 1, 0xB8AA);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 7, FALSE, 0xB8A8);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 10, FALSE, 0xB8A9);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 8, FALSE, 0xB8A8);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 9, FALSE, 0xB8A9);
    sub_0207775C(work->spriteSystem, work->spriteManager, 0xB8AA, 0xB8AA);
}

void ov68_021E7028(MoveWork *work, u16 move, u16 index) {
    int type;
    int narc;
    int file;

    if (work->args->mode == 0) {
        type = GetMoveAttr(move, 3);
    } else {
        type = GetMoveAttr(move, 11) + 18;
    }
    narc = sub_020776B4();
    file = sub_02077678(type);
    SpriteSystem_ReplaceCharResObj(work->spriteSystem, work->spriteManager, narc, file, TRUE, 0xB8AB + index);
    if (index < 4) {
        ManagedSprite_SetPaletteOverride(work->sprites[index + 4], sub_0207769C(type) + 4);
    } else {
        ManagedSprite_SetPaletteOverride(work->sprites[index + 4], sub_0207769C(type));
    }
}

void ov68_021E70BC(MoveWork *work) {
    u32 i = 0;
    s32 y = 40;
    u8 *walker = (u8 *)work;

    for (; i < 4;) {
        ManagedSprite_SetPositionXY(*(ManagedSprite **)(walker + 0x134), 22, y);
        if (work->args->unk_16 + i >= work->count) {
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)(walker + 0x134), FALSE);
        } else {
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)(walker + 0x134), TRUE);
            ov68_021E7028(work, work->args->unk_10[work->args->unk_16 + i], i);
        }
        i++;
        y += 32;
        walker += 4;
    }
}
