#include "overlay_80_command_helpers_internal.h"

extern u8 ov80_0223B9D8[];
extern void ov80_0222C03C(void *, void *);
int FrtCmd_011(void *c);

int FrtCmd_011(void *c) {
    u8 *script = PTR(c, 0x1c);
    PTR(c, 0x1c) = script + 1;
    u8 value = *script;
    u32 offset = FrontierScriptContext_ReadWord(c);
    u32 row = value * 3;
    u8 column = U8(c, 0x8c);
    if (ov80_0223B9D8[row + column] == 1) {
        ov80_0222C03C(c, (u8 *)PTR(c, 0x1c) + offset);
    }
    return 0;
}
