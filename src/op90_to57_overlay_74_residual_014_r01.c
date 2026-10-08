#pragma force_active on
#include "global.h"
typedef struct PmAgbCartridgeSpec {
    u32 gameCode;
    u32 kind;
} PmAgbCartridgeSpec;
typedef struct AgbRomHeader {
    u32 entry;
    u8 logo[156];
    u8 title[12];
    u8 gameCode[4];
    u8 makerCode[2];
    u8 fixed;
    u8 unitCode;
    u8 deviceType;
    u8 reserved[7];
    u8 version;
    u8 checksum;
    u8 reserved2[2];
} AgbRomHeader;
extern PmAgbCartridgeSpec *sPmAgbCartridgeSpec;
extern const u8 sAgbCartNintendoLogo[];
BOOL CTRDG_IsAgbCartridge(void);
u32 CTRDG_GetAgbGameCode(void);
void CTRDG_Enable(BOOL enable);
void CTRDG_CpuCopy32(const void *src, void *dest, u32 size);
int IdentifyPmAgbCartridge(PmAgbCartridgeSpec *specs, int count);

// @fn IdentifyPmAgbCartridge
int IdentifyPmAgbCartridge(PmAgbCartridgeSpec *specs, int count) {
    AgbRomHeader header;
    u8 *bytes;
    u32 gameCode;
    int i;
    u32 j;
    int k;
    u8 sum;

    sPmAgbCartridgeSpec = NULL;
    if (!CTRDG_IsAgbCartridge()) {
        return 1;
    }
    gameCode = CTRDG_GetAgbGameCode();
    for (i = 0; i < count; i++) {
        if (gameCode == specs[i].gameCode) {
            sPmAgbCartridgeSpec = &specs[i];
            break;
        }
    }
    if (sPmAgbCartridgeSpec == NULL) {
        return 2;
    }
    CTRDG_Enable(TRUE);
    bytes = (u8 *)&header;
    CTRDG_CpuCopy32((const void *)0x08000000, bytes, sizeof(AgbRomHeader));
    for (j = 0; j < 156; j++) {
        if (sAgbCartNintendoLogo[j] != bytes[j + 4]) {
            return 4;
        }
    }
    sum = 0;
    for (k = 0xA0; k <= 0xBC; k++) {
        sum += bytes[k];
    }
    sum = -(sum + 0x19);
    if (bytes[0xBD] != sum) {
        return 4;
    }
    return 0;
}
