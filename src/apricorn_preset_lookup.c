#include "apricorn_preset_lookup_private.h"

BOOL sub_02031CEC(SaveApricornBox *box, int index, UnkStruct_02031CEC *out) {
    const u16 *src;
    u16 *dst = (u16 *)out;

    if (index >= 9) {
        index = 0;
    }
    if (index <= 4) {
        *(ApricornPreset *)out = _020F6902[index];
    } else if (index <= 7) {
        src = (const u16 *)((u8 *)box + 0x38 + (index - 5) * 0x20);
        if (sub_02031D80((UnkStruct_02031CEC *)src) == 0) {
            sub_02031B5C(out);
            return FALSE;
        }
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    } else {
        if (*((u8 *)box + 9) == 0) {
            sub_02031B5C(out);
            return FALSE;
        }
        dst[0] = *(u16 *)((u8 *)box + 0xC);
        dst[1] = *(u16 *)((u8 *)box + 0xE);
        dst[2] = *(u16 *)((u8 *)box + 0x10);
        dst[3] = *(u16 *)((u8 *)box + 0x12);
    }
    return TRUE;
}
