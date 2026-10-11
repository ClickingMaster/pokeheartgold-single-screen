#include "global.h"
extern void *BattleDpadCursor_New(u32);
extern void ov08_02223000(void *);
extern void ov08_022230F4(void *);
extern void ov08_022231E8(void *);
extern void FontID_Alloc(int, u32);
extern void *BattleSystem_GetBagCursor(void *);
extern int BagCursor_Battle_GetPocket(void *);
extern void ov08_02223BF4(void *);
extern void ov08_02224A50(void *, int);
extern void ov08_022233B8(void *);
extern void ov08_02223480(void *, int);
extern void ov08_02223D08(void *);
extern void ov08_02223F94(void *, int);
extern void BattleDpadCursor_SetActive(void *, int);
extern void ov08_02224134(void *, int);
extern void ov08_0222421C(void *, int);
extern u8 PaletteData_BeginPaletteFade(void *, u16, u16, s8, u8, u8, u16);
int ov08_0222276C(u8 *work);
int ov08_0222276C(u8 *work) {
    *(volatile u16 *)0x04001050 = 0;
    *(void **)(work + 0x34) = BattleDpadCursor_New(*(u32 *)(*(u8 **)work + 0xC));
    ov08_02223000(work);
    ov08_022230F4(work);
    ov08_022231E8(work);
    FontID_Alloc(4, *(u32 *)(*(u8 **)work + 0xC));
    work[0x114D] = BagCursor_Battle_GetPocket(BattleSystem_GetBagCursor(*(void **)(*(u8 **)work)));
    ov08_02223BF4(work);
    ov08_02224A50(work, work[0x114C]);
    ov08_022233B8(work);
    ov08_02223480(work, work[0x114C]);
    ov08_02223D08(work);
    ov08_02223F94(work, work[0x114C]);
    if ((*(u8 **)work)[0x25]) BattleDpadCursor_SetActive(*(void **)(work + 0x34), 1);
    ov08_02224134(work, work[0x114C]);
    ov08_0222421C(work, work[0x114C]);
    PaletteData_BeginPaletteFade(*(void **)(work + 8), 10, 0xFFFF, -8, 16, 0, 0);
    if (*(u32 *)(*(u8 **)work + 0x14) == 1) return 12;
    return 1;
}

extern void *ov08_02225B4C;
extern int PaletteData_GetSelectedBuffersBitmask(void *);
extern int ov08_02223368(void *, void *);
extern int BattleDpadCursor_HandleInput(void *);
extern void ov08_0222417C(void *);
extern void PlaySE(int);
extern void ov08_02224938(void *, int, int);
extern void ov08_02223BA8(void *);
int ov08_02222840(u8 *work);
int ov08_02222840(u8 *work) {
    int choice;
    u8 slot;
    if (PaletteData_GetSelectedBuffersBitmask(*(void **)(work + 8))) return 1;
    choice = ov08_02223368(work, &ov08_02225B4C);
    if (choice == -1) {
        choice = BattleDpadCursor_HandleInput(*(void **)(work + 0x34));
        if (choice == -2) choice = 5;
    } else {
        ov08_0222417C(work);
    }
    switch (choice) {
    case 0: case 1: case 2: case 3:
        PlaySE(0x5DD);
        slot = choice;
        work[0x114D] = slot;
        work[0x114B] = 5;
        ov08_02224938(work, slot, 0);
        return 11;
    case 4:
        if (!*(u16 *)(*(u8 **)work + 0x20)) break;
        PlaySE(0x5DD);
        work[0x114D] = (*(u8 **)work)[0x1F];
        work[0x114B] = 6;
        ov08_02223BA8(work);
        ov08_02224938(work, 4, 0);
        return 11;
    case 5:
        PlaySE(0x5DD);
        *(u16 *)(*(u8 **)work + 0x1C) = 0;
        (*(u8 **)work)[0x1E] = 4;
        ov08_02224938(work, 5, 0);
        return 13;
    }
    return 1;
}
