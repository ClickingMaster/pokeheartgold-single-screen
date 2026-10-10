#include "stub_macro.h"
#include "rom_test.h"
#include "mac_owner.h"
#include "rom_util.h"
#include "static_init.h"

static asm void NitroStaticInit(void) {
    DECODE_FUNC_TABLE
    FUNC_TABLE_ENTRY(RunEncrypted_ROMTest_IsBad, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_ROMTest_IsGood, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_MACOwner_IsBad, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_MACOwner_IsGood, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_ROMUtil_Read, 0x68)
    FUNC_TABLE_ENTRY(RunEncrypted_ROMUtil_CRC32, 0x68)
    FUNC_TABLE_END
}
