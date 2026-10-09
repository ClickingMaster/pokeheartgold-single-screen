#pragma force_active on
#include "global.h"
typedef struct PmAgbCartridgeSpec {
    u32 gameCode;
    u8 kind;
} PmAgbCartridgeSpec;
typedef struct PmAgbRomHeader {
    u8 unk00[0x50];
    u32 offset50;
    u8 unk54[0x34];
    u32 offset88;
    u32 offset8C;
    u8 unk90[8];
    u32 offset98;
    u8 unk9C[0x60];
} PmAgbRomHeader;
typedef struct Ov74AgbState {
    u32 unk00[2];
    int value;
    u8 unk0C[0xC];
    PmAgbRomHeader header;
} Ov74AgbState;
extern Ov74AgbState ov74_0223D33C;
extern PmAgbCartridgeSpec *sPmAgbCartridgeSpec;
extern PmAgbCartridgeSpec sPmAgbRomCodeMappings[];
int IdentifyPmAgbCartridge(PmAgbCartridgeSpec *specs, int count);
BOOL PmAgbCartridgeHasFlash(void);
void CTRDG_CpuCopy32(const void *src, void *dest, u32 size);
int PmAgbCartridgeGetOffsets(int value);

// @fn PmAgbCartridgeGetOffsets
int PmAgbCartridgeGetOffsets(int value) {
    int result;
    ov74_0223D33C.value = value;
    if (sPmAgbCartridgeSpec != NULL) {
        return 12;
    }
    result = IdentifyPmAgbCartridge(sPmAgbRomCodeMappings, 30);
    if (result != 0) {
        return result;
    }
    if (sPmAgbCartridgeSpec->kind == 0) {
        ov74_0223D33C.header.offset88 = 0x890;
        ov74_0223D33C.header.offset8C = 0x3AC0;
        ov74_0223D33C.header.offset50 = 0x1220;
        ov74_0223D33C.header.offset98 = 9;
    } else {
        CTRDG_CpuCopy32((const void *)0x08000100, &ov74_0223D33C.header, sizeof(PmAgbRomHeader));
    }
    if (!PmAgbCartridgeHasFlash()) {
        return 3;
    }
    return 0;
}
