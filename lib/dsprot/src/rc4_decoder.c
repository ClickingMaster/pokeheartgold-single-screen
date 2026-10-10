#include "stub_macro.h"
#include "rc4.h"
#include "static_init.h"

static asm void NitroStaticInit(void) {
    DECODE_FUNC_TABLE
    FUNC_TABLE_ENTRY(RC4_Init, 0x74)
    FUNC_TABLE_ENTRY(RC4_Byte, 0x4c)
    FUNC_TABLE_ENTRY(RC4_InitSBox, 0x24)
    FUNC_TABLE_ENTRY(RC4_EncryptInstructions, 0x108)
    FUNC_TABLE_ENTRY(RC4_DecryptInstructions, 0x108)
    FUNC_TABLE_ENTRY(RC4_InitAndEncryptInstructions, 0x58)
    FUNC_TABLE_ENTRY(RC4_InitAndDecryptInstructions, 0x58)
    FUNC_TABLE_END
}
