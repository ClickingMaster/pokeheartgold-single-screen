#include "global.h"

extern const u8 ov99_021EA35A[];
void ov99_021E8828(void *work);
void ov99_021E89A4(void *);
void ov99_021E9038(void *);
void ov99_021E87D0(void *);
void ov99_021E8FEC(void *, int);
void ov99_021E9020(void *);
void ov98_0221EE84(void *, int);
void ov98_0221EEDC(void *, int);
void ov98_0221EEEC(void *, int, u8);
void ov98_0221EC08(void *, int, int, int, int);
void BgClearTilemapBufferAndCommit(void *, int);
void ManagedSprite_SetDrawFlag(void *, int);

void ov99_021E8828(void *work) {
    int i;
    const u8 *table;

    ov99_021E89A4(work);
    ov99_021E9038(work);
    ov99_021E87D0(work);
    table = ov99_021EA35A;
    for (i = 0; i < 5; i++, table += 2) {
        ov98_0221EE84(*(void **)((u8 *)work + 0x10), i + 7);
        ov98_0221EEDC(*(void **)((u8 *)work + 0x10), i + 7);
        ov98_0221EEEC(*(void **)((u8 *)work + 0x10), i + 7, table[*(u32 *)((u8 *)work + 0xB0)]);
    }
    ov99_021E8FEC(work, 0);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)work + 4), 6);
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)work + 0x1C), 0);
    ov98_0221EE84(*(void **)((u8 *)work + 0x10), 3);
    ov98_0221EE84(*(void **)((u8 *)work + 0x10), 6);
    ov98_0221EE84(*(void **)((u8 *)work + 0x10), 0x11);
    ov98_0221EE84(*(void **)((u8 *)work + 0x10), 0x12);
    ov99_021E9020(work);
    ov98_0221EC08(*(void **)((u8 *)work + 0x10), 2, 0x3F, 1, 1);
    ov98_0221EC08(*(void **)((u8 *)work + 0x10), 5, 0x40, 0, 1);
    *(u32 *)((u8 *)work + 0xB4) = 0;
}
