#include "to57_overlay112_118_private.h"
#define U8(w, o)  (*(u8 *)((w)->raw + (o)))
#define U16(w, o) (*(u16 *)((w)->raw + (o)))
#define U32(w, o) (*(u32 *)((w)->raw + (o)))
#define PTR(w, o) (*(void **)((w)->raw + (o)))
typedef struct Ov112Flags {
    u8 low : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
} Ov112Flags;

int ov112_021EDF4C(Ov112R118 *w) {
    void *s = String_New(12, 0x9A);
    int printer;
    FillBgTilemapRect(PTR(w, 0x18), 2, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(PTR(w, 0x18), 2);
    if (ov112_021ED610(w)) {
        CopyU16ArrayToStringN(s, w->raw + 0x9D54, 12);
        BufferString(PTR(w, 0x1E448), 1, s, (u32)(U8(w, 0x9D51) << 25) >> 30, 1, 1);
        printer = (int)PokewalkerApp_StartMessageBox(w, 2, 0x1B);
    } else {
        printer = (int)PokewalkerApp_StartMessageBox(w, 2, 0x20);
    }
    U32(w, 0x1E524) = printer;
    PokewalkerIr_Stop();
    String_Delete(s);
    return 0x26;
}

int ov112_021EDFE8(Ov112R118 *w) {
    int result = 1;
    void *mon;
    u32 species;
    u32 value;
    mon = AllocMonZeroed(0x9A);
    if (Pokewalker_TryGetBoxMon(PTR(w, 0x1E440), mon) == 0) {
        result = 0;
    }
    species = GetMonData(mon, 5, NULL);
    value = GetMonData(mon, 0x70, NULL);
    if (U16(w, 0x9D44) != species || ((u32)(U8(w, 0x9D51) << 27) >> 27) != value) {
        result = 0;
    }
    Heap_Free(mon);
    return result;
}

int ov112_021EE044(Ov112R118 *w) {
    ov112_021EAAE4(w, w->raw + 0x1F2C0);
    if (!TextPrinterCheckActive((u8)U32(w, 0x1E524))) {
        if (!IsFanfarePlaying()) {
            if (!ov112_021ED610(w)) {
                MIi_CpuClearFast(0, (u32 *)(w->raw + 0x9D44), 0x10);
                U8(w, 0x10E7) &= ~4;
            }
            if (!ov112_021ED640(w)) {
                MIi_CpuClearFast(0, (u32 *)(w->raw + 0xAD00), 0x2FC);
                U8(w, 0xAABC) &= ~0x20;
            }
            if (ov112_021EDFE8(w) || ((Ov112Flags *)(w->raw + 0xAABC))->bit5 || ((Ov112Flags *)(w->raw + 0xAABC))->bit6 || ((u32)(U8(w, 0x10E7) << 29) >> 31)) {
                U32(w, 4) = 6;
                return 0x42;
            }
            return 0x27;
        }
    }
    return 0x26;
}
