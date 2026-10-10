#include "stub_macro.h"
#include "integrity.h"
#include "static_init.h"

static asm void NitroStaticInit(void) {
    DECODE_FUNC_TABLE
    FUNC_TABLE_ENTRY(RunEncrypted_Integrity_MACOwner_IsBad, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_Integrity_MACOwner_IsGood, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_Integrity_ROMTest_IsBad, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_Integrity_ROMTest_IsGood, 0x68)
    FUNC_TABLE_END
}
