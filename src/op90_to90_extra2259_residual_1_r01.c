#pragma force_active on
#include "global.h"

typedef struct WalkerItemSlot {
    u16 item;
    u16 unk2;
} WalkerItemSlot2;
typedef struct W12Work {
    u8 raw[0x9DAC];
    WalkerItemSlot2 slotsA[3];
    WalkerItemSlot2 slotsB[10];
    u8 rest[0x1F378 - 0x9DAC - 13 * 4];
} W12Work;

#define U8(w, n) (*(u8 *)((w)->raw + (n)))
#define U16(w, n) (*(u16 *)((w)->raw + (n)))
#define U32(w, n) (*(u32 *)((w)->raw + (n)))
#define PTR(w, n) (*(void **)((w)->raw + (n)))


typedef struct WalkerFlags {
    u8 unk0 : 6;
    u8 hasItem : 1;
    u8 unk7 : 1;
} WalkerFlags;

extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void PokewalkerApp_LoadTilemap(W12Work *, int, int);
extern void PokewalkerApp_PrintStringDefaultColor(W12Work *, int, void *, int);
extern void PokewalkerApp_PrintString(W12Work *, int, void *, int, u32);
extern void *String_New(int, int);
extern void String_Delete(void *);
extern void GetItemNameIntoString(void *, u16, int);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern void PokewalkerApp_RemoveWindowBank(W12Work *, int);
extern void PokewalkerApp_AddWindowBank(void *, int);
extern BOOL ov112_021EFD4C(W12Work *);
extern void ov112_021EFD24(u16 *, u16);
int ov112_021EFDA8(W12Work *w);

// @fn ov112_021EFDA8
int ov112_021EFDA8(W12Work *w) {
    u16 items[14];
    int i;
    void *str;
    PokewalkerApp_RemoveWindowBank(w, 5);
    PokewalkerApp_AddWindowBank(w, 6);
    if (!ov112_021EFD4C(w)) {
        return 0x24;
    }
    FillBgTilemapRect(PTR(w, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    PokewalkerApp_LoadTilemap(w, 2, 0xF);
    PokewalkerApp_PrintStringDefaultColor(w, 1, PTR(w, 0x1E4A4), 0);
    MIi_CpuClearFast(0, (u32 *)items, sizeof(items));
    for (i = 0; i < 3; i++) {
        ov112_021EFD24(items, w->slotsA[i].item);
    }
    for (i = 0; i < 10; i++) {
        ov112_021EFD24(items, w->slotsB[i].item);
    }
    if (((WalkerFlags *)&U8(w, 0xAABC))->hasItem) {
        ov112_021EFD24(items, U16(w, 0xB002));
        MIi_CpuClearFast(0, (u32 *)&U8(w, 0xAABC), 0x6C8);
        MIi_CpuClearFast(0, (u32 *)&U8(w, 0x9D70), 0xD4C);
    }
    str = String_New(0x13, 0x9A);
    for (i = 0; i < 14; i++) {
        if (items[i] == 0) {
            break;
        }
        GetItemNameIntoString(str, items[i], 0x9A);
        PokewalkerApp_PrintString(w, i + 2, str, 0, 0x30400);
    }
    String_Delete(str);
    BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, 0x9A);
    return 0x12;
}
