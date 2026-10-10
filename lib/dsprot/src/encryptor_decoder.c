#include "stub_macro.h"
#include "static_init.h"

static asm void NitroStaticInit(void) {
    DECODE_FUNC_TABLE
    FUNC_TABLE_ENTRY(Encryptor_EncryptFunction, 0x8c)
    FUNC_TABLE_ENTRY(Encryptor_DecryptFunction, 0x84)
    FUNC_TABLE_END
}
