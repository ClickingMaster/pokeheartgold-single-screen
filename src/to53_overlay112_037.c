#include "to53_overlay112_w09_private.h"


void ov112_021E966C(void *p, int value) {
    PokewalkerApp_LoadTilemap(p, 2, 13);
    ov112_021EA51C(p, 0);
    ov112_021E9FA4(p, 3, *(String **)((u8 *)p + 0x1E514), 4, 0xF0100);
    ov112_021E9FA4(p, 4, *(String **)((u8 *)p + 0x1E518), 4, 0xF0100);
    ov112_021EA51C(p, value);
    *(int *)((u8 *)p + 0x1EC50) = value;
    *(u16 *)((u8 *)p + 0x1F2E2) = 0;
}

void ov112_021E96D0(void *p, int value) {
    String *a = NewString_ReadMsgData(*(void **)((u8 *)p + 0x1E44C), 0x96);
    String *b = NewString_ReadMsgData(*(void **)((u8 *)p + 0x1E44C), 0x97);
    PokewalkerApp_LoadTilemap(p, 2, 13);
    ov112_021EA51C(p, 0);
    ov112_021E9FA4(p, 3, a, 4, 0xF0100);
    ov112_021E9FA4(p, 4, b, 4, 0xF0100);
    ov112_021EA51C(p, value);
    *(int *)((u8 *)p + 0x1EC50) = value;
    *(u16 *)((u8 *)p + 0x1F2E2) = 0;
    String_Delete(b);
    String_Delete(a);
}
