#include "global.h"
#include <string.h>
extern void *CreateSysTaskAndEnvironment(void (*)(void *, void *), u32, u32, u32);
extern void *SysTask_GetData(void *);
extern void *BattleSystem_GetBgConfig(void *);
extern void *BattleSystem_GetPaletteData(void *);
extern void *BattleSystem_GetBagCursor(void *);
extern void BagCursor_Battle_PocketGetPosition(void *, u32, u8 *, u8 *);
extern u16 BagCursor_Battle_GetLastUsedItem(void *);
extern u8 BagCursor_Battle_GetLastUsedPocket(void *);
extern void ov08_02223B78(void *);
extern u32 BattleSystem_GetBattleType(void *);
extern void ov08_02222670(void *, u8 *);
void ov08_022225D4(u8 *state);
void ov08_022225D4(u8 *state) {
    u8 *work;
    void *cursor;
    u8 i;
    work = SysTask_GetData(CreateSysTaskAndEnvironment((void (*)(void *, void *))ov08_02222670, 0x115C, 100, *(u32 *)(state + 0xC)));
    memset(work, 0, 0x115C);
    *(u8 **)work = state;
    *(void **)(work + 4) = BattleSystem_GetBgConfig(*(void **)state);
    *(void **)(work + 8) = BattleSystem_GetPaletteData(*(void **)state);
    work[0x114A] = 0;
    cursor = BattleSystem_GetBagCursor(*(void **)state);
    for (i = 0; i < 5; i++) {
        BagCursor_Battle_PocketGetPosition(cursor, i, *(u8 **)work + 0x27 + i, *(u8 **)work + 0x2C + i);
    }
    *(u16 *)(*(u8 **)work + 0x20) = BagCursor_Battle_GetLastUsedItem(cursor);
    (*(u8 **)work)[0x1F] = BagCursor_Battle_GetLastUsedPocket(cursor);
    ov08_02223B78(work);
    if (BattleSystem_GetBattleType(*(void **)(*(u8 **)work)) & 0x400) {
        *(u32 *)(*(u8 **)work + 0x14) = 1;
    }
}
#include "global.h"
extern int ov08_0222276C(void *);
extern int ov08_02222840(void *);
extern int ov08_02222918(void *);
extern int ov08_02222AF0(void *);
extern int ov08_02222D78(void *);
extern int ov08_02222D84(void *);
extern int ov08_02222D90(void *);
extern int ov08_02222A78(void *);
extern int ov08_02222D9C(void *);
extern int ov08_02222DAC(void *);
extern int ov08_02222DC4(void *);
extern int ov08_02222DEC(void *);
extern int ov08_02222EC4(void *);
extern int ov08_02222E04(void *);
extern int ov08_02222E2C(void *, void *);
extern void ov08_02224974(void *);
extern void SpriteSystem_DrawSprites(void *);
void ov08_02222670(void *task, u8 *work);
void ov08_02222670(void *task, u8 *work) {
    int state = work[0x114A];
    switch (state) {
    case 0: work[0x114A] = ov08_0222276C(work); break;
    case 1: work[0x114A] = ov08_02222840(work); break;
    case 2: work[0x114A] = ov08_02222918(work); break;
    case 3: work[0x114A] = ov08_02222AF0(work); break;
    case 4: work[0x114A] = ov08_02222D78(work); break;
    case 5: work[0x114A] = ov08_02222D84(work); break;
    case 6: work[0x114A] = ov08_02222D90(work); break;
    case 7: work[0x114A] = ov08_02222A78(work); break;
    case 8: work[0x114A] = ov08_02222D9C(work); break;
    case 9: work[0x114A] = ov08_02222DAC(work); break;
    case 10: work[0x114A] = ov08_02222DC4(work); break;
    case 11: work[0x114A] = ov08_02222DEC(work); break;
    case 12: work[0x114A] = ov08_02222EC4(work); break;
    case 13: work[0x114A] = ov08_02222E04(work); break;
    case 14: if (ov08_02222E2C(task, work) == 1) return; break;
    }
    ov08_02224974(work);
    SpriteSystem_DrawSprites(*(void **)(work + 0x30C));
}
