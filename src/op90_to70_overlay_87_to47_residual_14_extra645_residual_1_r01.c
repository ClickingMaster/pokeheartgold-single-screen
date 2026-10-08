#pragma force_active on
#include "global.h"
#include "heap.h"

typedef struct Ov87Obj {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4[8];
    void *unkC;
} Ov87Obj;

void *ov87_021E7DF8(void *a0, void *a1, void *a2, int a3, int a4, u8 a5);
void ov87_021E7FEC(Ov87Obj *obj, int a1, u16 a2);
Ov87Obj *ov87_021E7F6C(void *a0, void *a1, void *a2, u16 a3, u16 a6, u8 a7, int a4, int a5);

// @fn ov87_021E7F6C
Ov87Obj *ov87_021E7F6C(void *a0, void *a1, void *a2, u16 a3, u16 a6, u8 a7, int a4, int a5) {
    Ov87Obj *obj = Heap_Alloc(HEAP_ID_122, sizeof(Ov87Obj));
    memset(obj, 0, sizeof(Ov87Obj));
    obj->unkC = ov87_021E7DF8(a0, a1, a2, a4, a5, a7);
    obj->unk2 = a7;
    ov87_021E7FEC(obj, a3, a6);
    return obj;
}
