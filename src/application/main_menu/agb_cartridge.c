#include "application/main_menu/agb_cartridge.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/species.h"

#include "config.h"
#include "item.h"
#include "pm_version.h"
#include "pokemon.h"
#include "system.h"

// GBA cartridge access (NitroSDK CTRDG)
BOOL CTRDG_IsAgbCartridge(void);
u32 CTRDG_GetAgbGameCode(void);
void CTRDG_Enable(BOOL enable);
void CTRDG_CpuCopy32(const void *src, void *dest, u32 size);
int CTRDG_IdentifyAgbBackup(int type);
void CTRDG_ReadAgbFlash(u16 sector, u32 offset, void *dest, u32 size);
void CTRDG_WriteAndVerifyAgbFlashAsync(u8 sector, void *src, u32 verifySize, void *callback);

// A CTRDG task as passed to its callback
typedef struct PmAgbFlashTask {
    u8 unk_00[8];
    s32 result;
} PmAgbFlashTask;

// Save sizes of the game (read from the ROM by FireRed/LeafGreen/Emerald, fixed for Ruby/Sapphire)
typedef struct PmAgbSaveSizes {
    u8 unk_00[0x20];
    u32 trainerInfoSize; // data size of save sector 0
    u32 gameStateSize;   // total data size of save sectors 1-4
    u8 unk_28[8];
    u32 unk_30;
    u8 unk_34[0x60];
} PmAgbSaveSizes;

// The part of the GBA ROM header copied from 0x08000100
typedef struct PmAgbRomHeader {
    u8 unk_00[0x50];
    u32 unk_50;
    u8 unk_54[0x14];
    PmAgbSaveSizes saveSizes;
} PmAgbRomHeader;

// The GBA ROM header at 0x08000000
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

// State of the cartridge save (bss sPmAgbSave); the save counter and the write status are also reached through
// their own labels (sPmAgbSaveCounter), the ROM header copy through sPmAgbRomHeader (header)
typedef struct PmAgbSave {
    u32 slot;           // the save slot read (0/1)
    u32 sectorsWritten; // sectors written by the current save
    u8 *buffer;         // sector buffer (0x1000) followed by the save data copies (see PmAgbCartridge_GetSectorData)
    u32 firstSector;    // flash sector of the trainer info sector (the sectors of a slot are rotated)
    u32 counter;        // save counter
    u32 writeStatus;    // 0 idle, 1 writing, 2 last sector, 3 next sector, 4 failed
    PmAgbRomHeader header;
} PmAgbSave;

// The four encrypted substructures of an AgbBoxMon (12 bytes each)
typedef union AgbBoxMonSubstruct {
    u32 words[3];

    struct {
        u32 first;
        u32 hpIV : 5, atkIV : 5, defIV : 5, speedIV : 5, spAtkIV : 5, spDefIV : 5, isEgg : 1, abilityNum : 1;
        u32 coolRibbons : 3, beautyRibbons : 3, cuteRibbons : 3, smartRibbons : 3, toughRibbons : 3, others : 17;
    } misc;

    struct {
        u8 unk_00[8];
        u8 pp[4];
    } attacks;
} AgbBoxMonSubstruct;

extern PmAgbCartridge sPmAgbCartridgeSpec;
extern PmAgbCartridgeSpec sPmAgbRomCodeMappings[];
extern const u8 sAgbCartNintendoLogo[];
extern u32 sMigrateFromAgbIcons;
extern u32 sPmAgbSaveCounter;
extern PmAgbRomHeader sPmAgbRomHeader;
extern const u16 sAgbKeepFirstAbilitySpecies[];
extern u16 ov74_0223CC5C[];
extern u16 ov74_0223CC5E[];

int IdentifyPmAgbCartridge(PmAgbCartridgeSpec *specs, int count);
u16 PmAgbCartridge_CalcSectorChecksum(u32 *data, u16 size);
void PmAgbCartridge_ReadSector(int sector, void *dest);
u16 PmAgbCartridge_GetSectorDataSize(int id);
void *PmAgbCartridge_GetSectorData(int id);
int PmAgbCartridge_CheckSaveSlot(int slot, AgbSaveSector *sector, u32 *counter);
int PmAgbCartridge_FindLatestSaveSlot(AgbSaveSector *sector, u32 *slot, u32 *counter);
void PmAgbCartridge_WriteSectorAsync(int id);
u32 PmAgbCartridge_WriteSectorCallback(PmAgbFlashTask *task);
void *AGB_GetBoxMonSubstruct(AgbBoxMon *mon, u32 personality, u32 which);
void AGB_DecryptBoxMon(AgbBoxMon *mon);
void AGB_EncryptBoxMon(AgbBoxMon *mon);
u16 AGB_CalcBoxMonChecksum(AgbBoxMon *mon);
int AGB_GetBoxMonAbility(AgbBoxMon *agbMon, BoxPokemon *boxMon);

extern PmAgbSave sPmAgbSave;

BOOL PmAgbCartridgeHasFlash(void) {
    if (CTRDG_IdentifyAgbBackup(1) != 0) {
        return FALSE;
    }
    return TRUE;
}

#pragma force_active on

int PmAgbCartridgeGetOffsets(void *buffer) {
    int result;
    sPmAgbSave.buffer = buffer;
    if (sPmAgbCartridgeSpec.spec != NULL) {
        return PM_AGB_RESULT_ALREADY_IDENTIFIED;
    }
    result = IdentifyPmAgbCartridge(sPmAgbRomCodeMappings, 30);
    if (result != PM_AGB_RESULT_OK) {
        return result;
    }
    if (sPmAgbCartridgeSpec.spec->gameType == 0) {
        sPmAgbSave.header.saveSizes.trainerInfoSize = 0x890;
        sPmAgbSave.header.saveSizes.gameStateSize = 0x3AC0;
        sPmAgbSave.header.unk_50 = 0x1220;
        sPmAgbSave.header.saveSizes.unk_30 = 9;
    } else {
        CTRDG_CpuCopy32((const void *)0x08000100, &sPmAgbSave.header, sizeof(PmAgbRomHeader));
    }
    if (!PmAgbCartridgeHasFlash()) {
        return PM_AGB_RESULT_NO_FLASH;
    }
    return PM_AGB_RESULT_OK;
}

#pragma force_active reset

void PmAgbCartridgeUnlinkSpec(void) {
    sPmAgbCartridgeSpec.spec = NULL;
}

u8 PmAgbCartridge_GetVersionInternal(void) {
    return sPmAgbCartridgeSpec.spec->version;
}

u16 PmAgbCartridge_GetLanguage(void) {
    return sPmAgbCartridgeSpec.spec->language;
}

#pragma force_active on

int IdentifyPmAgbCartridge(PmAgbCartridgeSpec *specs, int count) {
    AgbRomHeader header;
    u8 *bytes;
    u32 gameCode;
    int i;
    u32 j;
    int k;
    u8 sum;

    sPmAgbCartridgeSpec.spec = NULL;
    if (!CTRDG_IsAgbCartridge()) {
        return PM_AGB_RESULT_NO_CARTRIDGE;
    }
    gameCode = CTRDG_GetAgbGameCode();
    for (i = 0; i < count; i++) {
        if (gameCode == specs[i].gameCode) {
            sPmAgbCartridgeSpec.spec = &specs[i];
            break;
        }
    }
    if (sPmAgbCartridgeSpec.spec == NULL) {
        return PM_AGB_RESULT_UNKNOWN_GAME;
    }
    CTRDG_Enable(TRUE);
    bytes = (u8 *)&header;
    CTRDG_CpuCopy32((const void *)0x08000000, bytes, sizeof(AgbRomHeader));
    for (j = 0; j < 156; j++) {
        if (sAgbCartNintendoLogo[j] != bytes[j + 4]) {
            return PM_AGB_RESULT_BAD_HEADER;
        }
    }
    // header checksum over 0xA0-0xBC
    sum = 0;
    for (k = 0xA0; k <= 0xBC; k++) {
        sum += bytes[k];
    }
    sum = -(sum + 0x19);
    if (bytes[0xBD] != sum) {
        return PM_AGB_RESULT_BAD_HEADER;
    }
    return PM_AGB_RESULT_OK;
}

#pragma force_active reset

u16 PmAgbCartridge_CalcSectorChecksum(u32 *data, u16 size) {
    u16 i;
    u32 sum = 0;
    for (i = 0; i < size / 4; i++) {
        sum += *data++;
    }
    return (u16)(((sum >> 16) & 0xFFFF) + (sum & 0xFFFF));
}

void PmAgbCartridge_ReadSector(int sector, void *dest) {
    CTRDG_ReadAgbFlash((u16)sector, 0, dest, AGB_SAVE_SECTOR_SIZE);
}

#pragma force_active on
#pragma push
#pragma opt_propagation off

// Size of the data in the save sector with this id
u16 PmAgbCartridge_GetSectorDataSize(int id) {
    if (id == 0) {
        PmAgbSaveSizes *sizes = &sPmAgbRomHeader.saveSizes;
        return sizes->trainerInfoSize;
    }
    if (id == 4) {
        PmAgbSaveSizes *sizes = &sPmAgbRomHeader.saveSizes;
        return sizes->gameStateSize - 3 * 0xF80;
    }
    if (id == 13) {
        return 2000;
    }
    return 0xF80;
}

#pragma pop
#pragma force_active reset

// Where the data of the save sector with this id is kept: trainer info at +0x1000, game state at +0x2000, PC boxes
// at +0x6000
void *PmAgbCartridge_GetSectorData(int id) {
    if (id == 0) {
        return sPmAgbSave.buffer + 0x1000;
    }
    if (id >= 1 && id <= 4) {
        return sPmAgbSave.buffer + 0x2000 + (id - 1) * 0xF80;
    }
    if (id >= 5 && id < 14) {
        return sPmAgbSave.buffer + 0x6000 + (id - 5) * 0xF80;
    }
    return NULL;
}

#pragma force_active on

// 1: all 14 sectors of the slot are valid, 0xFF: some are, 0: the slot is empty
int PmAgbCartridge_CheckSaveSlot(int slot, AgbSaveSector *sector, u32 *counter) {
    u32 validSectors = 0;
    u8 found = FALSE;
    int i;
    *counter = 0;
    sPmAgbSave.sectorsWritten = 0;
    for (i = 0; i < AGB_SAVE_SECTORS_PER_SLOT; i++) {
        PmAgbCartridge_ReadSector(i % AGB_SAVE_SECTORS_PER_SLOT + slot * AGB_SAVE_SECTORS_PER_SLOT, sector);
        DC_FlushRange(sector, AGB_SAVE_SECTOR_SIZE);
        if (sector->signature == AGB_SAVE_SECTOR_SIGNATURE) {
            found = TRUE;
            if (PmAgbCartridge_CalcSectorChecksum((u32 *)sector, PmAgbCartridge_GetSectorDataSize(sector->id)) == sector->checksum) {
                *counter = sector->counter;
                validSectors |= 1 << sector->id;
            }
        }
    }
    if (found) {
        if (validSectors == 0x3FFF) {
            return 1;
        }
        return 0xFF;
    }
    return 0;
}

#pragma force_active reset

// Picks the newer of the two save slots
int PmAgbCartridge_FindLatestSaveSlot(AgbSaveSector *sector, u32 *slot, u32 *counter) {
    u32 counter0, counter1;
    int status0 = PmAgbCartridge_CheckSaveSlot(0, sector, &counter0);
    int status1 = PmAgbCartridge_CheckSaveSlot(1, sector, &counter1);
    if (status0 == 1 && status1 == 1) {
        if ((counter0 == 0xFFFFFFFF && counter1 == 0) || (counter0 == 0 && counter1 == 0xFFFFFFFF)) {
            // the counter wrapped around
            if (counter0 + 1 < counter1 + 1) {
                *counter = counter1;
                *slot = 1;
            } else {
                *counter = counter0;
                *slot = 0;
            }
        } else if (counter0 < counter1) {
            *counter = counter1;
            *slot = 1;
        } else {
            *counter = counter0;
            *slot = 0;
        }
        return 1;
    }
    if (status0 == 1 && status1 != 1) {
        *counter = counter0;
        if (status1 == 0xFF) {
            *slot = 0;
            return 0xFF;
        }
        *slot = 0;
        return 1;
    }
    if (status1 == 1 && status0 != 1) {
        *counter = counter1;
        if (status0 == 0xFF) {
            *slot = 1;
            return 0xFF;
        }
        *slot = 1;
        return 1;
    }
    if (status0 == 0 && status1 == 0) {
        *counter = 0;
        *slot = 0;
        return 0;
    }
    *counter = 0;
    *slot = 0;
    return 2;
}

#pragma force_active on

// Reads the latest save into the buffer
int PmAgbCartridge_LoadSave(void) {
    // sMigrateFromAgbIcons + 1 is sPmAgbSave: the literal pool holds the state's address twice
    int result = PmAgbCartridge_FindLatestSaveSlot((AgbSaveSector *)sPmAgbSave.buffer, (u32 *)(&sMigrateFromAgbIcons + 1), &sPmAgbSaveCounter);
    PmAgbSave *save;

    if (result != 1) {
        switch (result) {
        case 0xFF:
            return PM_AGB_RESULT_SAVE_DAMAGED;
        case 0:
            return PM_AGB_RESULT_NO_SAVE;
        case 2:
            return PM_AGB_RESULT_SAVE_CORRUPT;
        }
    }
    save = &sPmAgbSave;
    if (save->slot <= 1) {
        int i;
        AgbSaveSector *sector = (AgbSaveSector *)save->buffer;
        u32 validSectors = 0;
        for (i = 0; i < AGB_SAVE_SECTORS_PER_SLOT; i++) {
            PmAgbCartridge_ReadSector(i + save->slot * AGB_SAVE_SECTORS_PER_SLOT, sector);
            if (sector->signature == AGB_SAVE_SECTOR_SIGNATURE && sector->checksum == PmAgbCartridge_CalcSectorChecksum((u32 *)sector, PmAgbCartridge_GetSectorDataSize(sector->id))) {
                u32 size;
                save->counter = sector->counter;
                validSectors |= 1 << sector->id;
                size = PmAgbCartridge_GetSectorDataSize(sector->id);
                MIi_CpuCopy32((const u32 *)sector, (u32 *)PmAgbCartridge_GetSectorData(sector->id), size);
                if (sector->id == 0) {
                    save->firstSector = i;
                }
            }
        }
        if (validSectors == 0x3FFF) {
            return PM_AGB_RESULT_OK;
        }
        return PM_AGB_RESULT_BAD_HEADER;
    }
    return PM_AGB_RESULT_SAVE_CORRUPT;
}

#pragma force_active reset

// Writes the save sector with this id into the other slot (asynchronously; PmAgbCartridge_WriteSectorCallback continues)
void PmAgbCartridge_WriteSectorAsync(int id) {
    AgbSaveSector *sector = (AgbSaveSector *)sPmAgbSave.buffer;
    int size;
    int flashSector;
    sPmAgbSave.writeStatus = 1;
    MIi_CpuClear32(0, (u32 *)sector, AGB_SAVE_SECTOR_SIZE);
    size = PmAgbCartridge_GetSectorDataSize(id);
    MIi_CpuCopy32((u32 *)PmAgbCartridge_GetSectorData(id), (u32 *)sector, size);
    sector->counter = sPmAgbSave.counter;
    sector->id = id;
    sector->signature = AGB_SAVE_SECTOR_SIGNATURE;
    sector->checksum = PmAgbCartridge_CalcSectorChecksum((u32 *)sector, PmAgbCartridge_GetSectorDataSize(id));
    flashSector = (id + (int)sPmAgbSave.firstSector + 1) % AGB_SAVE_SECTORS_PER_SLOT + AGB_SAVE_SECTORS_PER_SLOT * sPmAgbCartridgeSpec.saveSlot;
    CTRDG_WriteAndVerifyAgbFlashAsync((u8)flashSector, sector, 4, (void *)PmAgbCartridge_WriteSectorCallback);
}

u32 PmAgbCartridge_WriteSectorCallback(PmAgbFlashTask *task) {
    if (task->result == 0) {
        sPmAgbSave.sectorsWritten++;
        if ((s32)sPmAgbSave.sectorsWritten >= AGB_SAVE_SECTORS_PER_SLOT) {
            sPmAgbSave.sectorsWritten = 0;
            sPmAgbSave.writeStatus = 0;
            return 0;
        }
        if (sPmAgbSave.sectorsWritten == AGB_SAVE_SECTORS_PER_SLOT - 1) {
            sPmAgbSave.writeStatus = 2;
            return 0;
        }
        sPmAgbSave.writeStatus = 3;
    } else {
        sPmAgbSave.writeStatus = 4;
    }
    return 0;
}

// Starts saving into the slot the game did not load from
BOOL PmAgbCartridge_StartSave(void) {
    int sectorsWritten = sPmAgbSave.sectorsWritten;
    if (sectorsWritten == 0) {
        if (sPmAgbSave.slot == 1) {
            sPmAgbCartridgeSpec.saveSlot = 0;
        } else {
            sPmAgbCartridgeSpec.saveSlot = 1;
        }
        sPmAgbSave.counter++;
    } else {
        return FALSE;
    }
    if (sPmAgbCartridgeSpec.saveSlot < 0) {
        return FALSE;
    }
    PmAgbCartridge_WriteSectorAsync(sectorsWritten);
    return TRUE;
}

// Writes the next sector when the previous one is done (the last one only on the second call after it)
int PmAgbCartridge_ContinueSave(void) {
    switch (sPmAgbSave.writeStatus) {
    case 2:
        PmAgbCartridge_WriteSectorAsync(sPmAgbSave.sectorsWritten);
        return PM_AGB_RESULT_OK;
    case 3:
        PmAgbCartridge_WriteSectorAsync(sPmAgbSave.sectorsWritten);
    case 1:
        return PM_AGB_RESULT_WRITE_BUSY;
    case 4:
        return PM_AGB_RESULT_WRITE_FAILED;
    case 0:
    default:
        return PM_AGB_RESULT_OK;
    }
}

int PmAgbCartridge_GetSaveStatus(void) {
    switch (sPmAgbSave.writeStatus) {
    case 2:
        return PM_AGB_RESULT_WRITE_LAST;
    case 3:
        return PM_AGB_RESULT_WRITE_BUSY;
    case 1:
        return PM_AGB_RESULT_WRITE_STARTING;
    case 4:
        return PM_AGB_RESULT_WRITE_FAILED;
    case 0:
    default:
        return PM_AGB_RESULT_OK;
    }
}

AgbPokemonStorage *PmAgbCartridge_GetPCStorage(void) {
    return (AgbPokemonStorage *)(sPmAgbSave.buffer + 0x6000);
}

// The trainer info sector
void *PmAgbCartridge_GetTrainerInfo(void) {
    return PmAgbCartridge_GetSectorData(0);
}

// The player's trainer id
u32 PmAgbCartridge_GetTrainerId(void) {
    u8 *trainerInfo = PmAgbCartridge_GetTrainerInfo();
    return (trainerInfo[0xD] << 24) | (trainerInfo[0xC] << 16) | (trainerInfo[0xB] << 8) | trainerInfo[0xA];
}

void *PmAgbCartridge_GetPlayerName(void *unused) {
    return PmAgbCartridge_GetTrainerInfo();
}

#pragma force_active on

// Substructure `which` (0 growth, 1 attacks, 2 EVs and condition, 3 misc) of the box Pokemon; the order of the four
// depends on personality % 24
void *AGB_GetBoxMonSubstruct(AgbBoxMon *mon, u32 personality, u32 which) {
    u8 *result = NULL;
    switch (personality % 24) {
    case 0: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data;
            break;
        case 1:
            result = data + 12;
            break;
        case 2:
            result = data + 24;
            break;
        case 3:
            result = data + 36;
            break;
        }
        break;
    }
    case 1: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data;
            break;
        case 1:
            result = data + 12;
            break;
        case 2:
            result = data + 36;
            break;
        case 3:
            result = data + 24;
            break;
        }
        break;
    }
    case 2: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data;
            break;
        case 1:
            result = data + 24;
            break;
        case 2:
            result = data + 12;
            break;
        case 3:
            result = data + 36;
            break;
        }
        break;
    }
    case 3: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data;
            break;
        case 1:
            result = data + 36;
            break;
        case 2:
            result = data + 12;
            break;
        case 3:
            result = data + 24;
            break;
        }
        break;
    }
    case 4: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data;
            break;
        case 1:
            result = data + 24;
            break;
        case 2:
            result = data + 36;
            break;
        case 3:
            result = data + 12;
            break;
        }
        break;
    }
    case 5: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data;
            break;
        case 1:
            result = data + 36;
            break;
        case 2:
            result = data + 24;
            break;
        case 3:
            result = data + 12;
            break;
        }
        break;
    }
    case 6: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 12;
            break;
        case 1:
            result = data;
            break;
        case 2:
            result = data + 24;
            break;
        case 3:
            result = data + 36;
            break;
        }
        break;
    }
    case 7: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 12;
            break;
        case 1:
            result = data;
            break;
        case 2:
            result = data + 36;
            break;
        case 3:
            result = data + 24;
            break;
        }
        break;
    }
    case 8: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 24;
            break;
        case 1:
            result = data;
            break;
        case 2:
            result = data + 12;
            break;
        case 3:
            result = data + 36;
            break;
        }
        break;
    }
    case 9: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 36;
            break;
        case 1:
            result = data;
            break;
        case 2:
            result = data + 12;
            break;
        case 3:
            result = data + 24;
            break;
        }
        break;
    }
    case 10: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 24;
            break;
        case 1:
            result = data;
            break;
        case 2:
            result = data + 36;
            break;
        case 3:
            result = data + 12;
            break;
        }
        break;
    }
    case 11: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 36;
            break;
        case 1:
            result = data;
            break;
        case 2:
            result = data + 24;
            break;
        case 3:
            result = data + 12;
            break;
        }
        break;
    }
    case 12: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 12;
            break;
        case 1:
            result = data + 24;
            break;
        case 2:
            result = data;
            break;
        case 3:
            result = data + 36;
            break;
        }
        break;
    }
    case 13: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 12;
            break;
        case 1:
            result = data + 36;
            break;
        case 2:
            result = data;
            break;
        case 3:
            result = data + 24;
            break;
        }
        break;
    }
    case 14: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 24;
            break;
        case 1:
            result = data + 12;
            break;
        case 2:
            result = data;
            break;
        case 3:
            result = data + 36;
            break;
        }
        break;
    }
    case 15: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 36;
            break;
        case 1:
            result = data + 12;
            break;
        case 2:
            result = data;
            break;
        case 3:
            result = data + 24;
            break;
        }
        break;
    }
    case 16: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 24;
            break;
        case 1:
            result = data + 36;
            break;
        case 2:
            result = data;
            break;
        case 3:
            result = data + 12;
            break;
        }
        break;
    }
    case 17: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 36;
            break;
        case 1:
            result = data + 24;
            break;
        case 2:
            result = data;
            break;
        case 3:
            result = data + 12;
            break;
        }
        break;
    }
    case 18: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 12;
            break;
        case 1:
            result = data + 24;
            break;
        case 2:
            result = data + 36;
            break;
        case 3:
            result = data;
            break;
        }
        break;
    }
    case 19: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 12;
            break;
        case 1:
            result = data + 36;
            break;
        case 2:
            result = data + 24;
            break;
        case 3:
            result = data;
            break;
        }
        break;
    }
    case 20: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 24;
            break;
        case 1:
            result = data + 12;
            break;
        case 2:
            result = data + 36;
            break;
        case 3:
            result = data;
            break;
        }
        break;
    }
    case 21: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 36;
            break;
        case 1:
            result = data + 12;
            break;
        case 2:
            result = data + 24;
            break;
        case 3:
            result = data;
            break;
        }
        break;
    }
    case 22: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 24;
            break;
        case 1:
            result = data + 36;
            break;
        case 2:
            result = data + 12;
            break;
        case 3:
            result = data;
            break;
        }
        break;
    }
    case 23: {
        u8 *data = (u8 *)mon + 32;
        switch (which) {
        case 0:
            result = data + 36;
            break;
        case 1:
            result = data + 24;
            break;
        case 2:
            result = data + 12;
            break;
        case 3:
            result = data;
            break;
        }
        break;
    }
    }
    return result;
}

#pragma force_active reset

// The substructures are XORed with the OT id and the personality
void AGB_DecryptBoxMon(AgbBoxMon *mon) {
    u32 *key = (u32 *)mon;
    u32 *ptr;
    u32 i = 0;
    ptr = mon->secure;
    for (; i < 12; i++) {
        u32 x = *ptr ^ key[1];
        *ptr = x;
        *ptr++ = x ^ key[0];
    }
}

void AGB_EncryptBoxMon(AgbBoxMon *mon) {
    u32 *key = (u32 *)mon;
    u32 *ptr;
    u32 i = 0;
    ptr = mon->secure;
    for (; i < 12; i++) {
        u32 x = *ptr ^ key[0];
        *ptr = x;
        *ptr++ = x ^ key[1];
    }
}

u16 AGB_CalcBoxMonChecksum(AgbBoxMon *mon) {
    u16 *substruct2;
    u16 *substruct1;
    u16 *substruct0;
    u16 *substruct3;
    u16 sum = 0;
    int i;

    substruct0 = AGB_GetBoxMonSubstruct(mon, mon->personality, 0);
    substruct1 = AGB_GetBoxMonSubstruct(mon, mon->personality, 1);
    substruct2 = AGB_GetBoxMonSubstruct(mon, mon->personality, 2);
    substruct3 = AGB_GetBoxMonSubstruct(mon, mon->personality, 3);
    for (i = 0; i < 6; i++) {
        sum += substruct0[i];
    }
    for (i = 0; i < 6; i++) {
        sum += substruct1[i];
    }
    for (i = 0; i < 6; i++) {
        sum += *substruct2++;
    }
    for (i = 0; i < 6; i++) {
        sum += *substruct3++;
    }
    return sum;
}

#pragma force_active on

u32 AGB_GetBoxMonData(AgbBoxMon *mon, int field, u8 *dest) {
    u32 result = 0;
    AgbBoxMonSubstruct *misc = NULL, *growth = NULL, *attacks = NULL, *condition = NULL;

    if (field > AGB_MON_DATA_UNKNOWN_10) {
        growth = AGB_GetBoxMonSubstruct(mon, mon->personality, 0);
        attacks = AGB_GetBoxMonSubstruct(mon, mon->personality, 1);
        condition = AGB_GetBoxMonSubstruct(mon, mon->personality, 2);
        misc = AGB_GetBoxMonSubstruct(mon, mon->personality, 3);
        AGB_DecryptBoxMon(mon);
        {
            u32 checksum = AGB_CalcBoxMonChecksum(mon);
            if (checksum != mon->checksum) {
                mon->isBadEgg = TRUE;
                mon->isEgg = TRUE;
                misc->words[1] |= 1u << 30;
            }
        }
    }
    switch (field) {
    case AGB_MON_DATA_PERSONALITY:
        result = mon->personality;
        break;
    case AGB_MON_DATA_OT_ID:
        result = mon->otId;
        break;
    case AGB_MON_DATA_NICKNAME:
        if (mon->isBadEgg) {
            result = 0;
        } else {
            for (result = 0; result < 10; result++) {
                dest[result] = mon->nickname[result];
            }
        }
        dest[result] = 0xFF;
        break;
    case AGB_MON_DATA_LANGUAGE:
        result = mon->language;
        break;
    case AGB_MON_DATA_IS_BAD_EGG:
        result = mon->isBadEgg;
        break;
    case AGB_MON_DATA_HAS_SPECIES:
        result = mon->hasSpecies;
        break;
    case AGB_MON_DATA_SANITY_IS_EGG:
        result = mon->isEgg;
        break;
    case AGB_MON_DATA_OT_NAME:
        for (result = 0; result < 7; result++) {
            dest[result] = mon->otName[result];
        }
        dest[result] = 0xFF;
        break;
    case AGB_MON_DATA_MARKINGS:
        result = mon->markings;
        break;
    case AGB_MON_DATA_CHECKSUM:
        result = mon->checksum;
        break;
    case AGB_MON_DATA_UNKNOWN_10:
        result = mon->unknown;
        break;
    case AGB_MON_DATA_SPECIES:
        if (mon->isBadEgg) {
            result = 412; // the Egg species of the GBA games
        } else {
            result = *(u16 *)growth;
        }
        break;
    case AGB_MON_DATA_HELD_ITEM:
        result = ((u16 *)growth)[1];
        break;
    case AGB_MON_DATA_EXPERIENCE:
        result = ((u32 *)growth)[1];
        break;
    case AGB_MON_DATA_PP_UPS:
        result = ((u8 *)growth)[8];
        break;
    case AGB_MON_DATA_FRIENDSHIP:
        result = ((u8 *)growth)[9];
        break;
    case AGB_MON_DATA_MOVE1:
    case AGB_MON_DATA_MOVE2:
    case AGB_MON_DATA_MOVE3:
    case AGB_MON_DATA_MOVE4:
        result = ((u16 *)attacks)[field - AGB_MON_DATA_MOVE1];
        break;
    case AGB_MON_DATA_MOVE1_PP:
    case AGB_MON_DATA_MOVE2_PP:
    case AGB_MON_DATA_MOVE3_PP:
    case AGB_MON_DATA_MOVE4_PP:
        result = attacks->attacks.pp[field - AGB_MON_DATA_MOVE1_PP];
        break;
    case AGB_MON_DATA_HP_EV:
        result = ((u8 *)condition)[0];
        break;
    case AGB_MON_DATA_ATK_EV:
        result = ((u8 *)condition)[1];
        break;
    case AGB_MON_DATA_DEF_EV:
        result = ((u8 *)condition)[2];
        break;
    case AGB_MON_DATA_SPEED_EV:
        result = ((u8 *)condition)[3];
        break;
    case AGB_MON_DATA_SPATK_EV:
        result = ((u8 *)condition)[4];
        break;
    case AGB_MON_DATA_SPDEF_EV:
        result = ((u8 *)condition)[5];
        break;
    case AGB_MON_DATA_COOL:
        result = ((u8 *)condition)[6];
        break;
    case AGB_MON_DATA_BEAUTY:
        result = ((u8 *)condition)[7];
        break;
    case AGB_MON_DATA_CUTE:
        result = ((u8 *)condition)[8];
        break;
    case AGB_MON_DATA_SMART:
        result = ((u8 *)condition)[9];
        break;
    case AGB_MON_DATA_TOUGH:
        result = ((u8 *)condition)[10];
        break;
    case AGB_MON_DATA_SHEEN:
        result = ((u8 *)condition)[11];
        break;
    // misc word 0: pokerus (8 bits), met location (8), met level (7), met game (4), ball (4), OT gender (1)
    case AGB_MON_DATA_POKERUS:
        result = (misc->words[0] << 24) >> 24;
        break;
    case AGB_MON_DATA_MET_LOCATION:
        result = (misc->words[0] << 16) >> 24;
        break;
    case AGB_MON_DATA_MET_LEVEL:
        result = (misc->words[0] << 9) >> 25;
        break;
    case AGB_MON_DATA_MET_GAME:
        result = (misc->words[0] << 5) >> 28;
        break;
    case AGB_MON_DATA_POKEBALL:
        result = (misc->words[0] << 1) >> 28;
        break;
    case AGB_MON_DATA_OT_GENDER:
        result = misc->words[0] >> 31;
        break;
    // misc word 1: six 5-bit IVs, is egg (1), ability number (1)
    case AGB_MON_DATA_HP_IV:
        result = misc->misc.hpIV;
        break;
    case AGB_MON_DATA_ATK_IV:
        result = (misc->words[1] << 22) >> 27;
        break;
    case AGB_MON_DATA_DEF_IV:
        result = (misc->words[1] << 17) >> 27;
        break;
    case AGB_MON_DATA_SPEED_IV:
        result = (misc->words[1] << 12) >> 27;
        break;
    case AGB_MON_DATA_SPATK_IV:
        result = (misc->words[1] << 7) >> 27;
        break;
    case AGB_MON_DATA_SPDEF_IV:
        result = (misc->words[1] << 2) >> 27;
        break;
    case AGB_MON_DATA_IS_EGG:
        result = (misc->words[1] << 1) >> 31;
        break;
    case AGB_MON_DATA_ABILITY_NUM:
        result = misc->words[1] >> 31;
        break;
    // misc word 2: ribbons
    case AGB_MON_DATA_COOL_RIBBONS:
        result = misc->misc.coolRibbons;
        break;
    case AGB_MON_DATA_BEAUTY_RIBBONS:
        result = (misc->words[2] << 26) >> 29;
        break;
    case AGB_MON_DATA_CUTE_RIBBONS:
        result = (misc->words[2] << 23) >> 29;
        break;
    case AGB_MON_DATA_SMART_RIBBONS:
        result = (misc->words[2] << 20) >> 29;
        break;
    case AGB_MON_DATA_TOUGH_RIBBONS:
        result = (misc->words[2] << 17) >> 29;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON:
        result = (misc->words[2] << 16) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 1:
        result = (misc->words[2] << 15) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 2:
        result = (misc->words[2] << 14) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 3:
        result = (misc->words[2] << 13) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 4:
        result = (misc->words[2] << 12) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 5:
        result = (misc->words[2] << 11) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 6:
        result = (misc->words[2] << 10) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 7:
        result = (misc->words[2] << 9) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 8:
        result = (misc->words[2] << 8) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 9:
        result = (misc->words[2] << 7) >> 31;
        break;
    case AGB_MON_DATA_CHAMPION_RIBBON + 10:
        result = (misc->words[2] << 6) >> 31;
        break;
    case AGB_MON_DATA_WORLD_RIBBON:
        result = (misc->words[2] << 5) >> 31;
        break;
    case 79:
        result = (misc->words[2] << 1) >> 28;
        break;
    case AGB_MON_DATA_FATEFUL_ENCOUNTER:
        result = misc->words[2] >> 31;
        break;
    case AGB_MON_DATA_SPECIES_OR_EGG:
        result = *(u16 *)growth;
        if (result && (((misc->words[1] << 1) >> 31) || mon->isBadEgg)) {
            result = 412;
        }
        break;
    case AGB_MON_DATA_COMBINED_IVS:
        result = misc->misc.hpIV | (misc->misc.atkIV << 5) | (misc->misc.defIV << 10) | (misc->misc.speedIV << 15) | (misc->misc.spAtkIV << 20) | (misc->misc.spDefIV << 25);
        break;
    }
    if (field > AGB_MON_DATA_UNKNOWN_10) {
        AGB_EncryptBoxMon(mon);
    }
    return result;
}

#pragma force_active reset

// Only the nickname, language, flags, OT name and species can be set
void AGB_SetBoxMonData(AgbBoxMon *mon, int field, const void *value) {
    u8 *box = (u8 *)mon;
    const u8 *in = value;
    u8 *growth;
    u8 *misc;
    int i;
    growth = NULL;
    misc = NULL;
    if (field > AGB_MON_DATA_UNKNOWN_10) {
        growth = AGB_GetBoxMonSubstruct(mon, mon->personality, 0);
        AGB_GetBoxMonSubstruct(mon, mon->personality, 1);
        AGB_GetBoxMonSubstruct(mon, mon->personality, 2);
        misc = AGB_GetBoxMonSubstruct(mon, mon->personality, 3);
        AGB_DecryptBoxMon(mon);
        u16 checksum = AGB_CalcBoxMonChecksum(mon);
        u16 stored = mon->checksum;
        if (checksum != stored) {
            box[0x13] = (box[0x13] & ~1) | 1;
            box[0x13] |= 4;
            *(u32 *)(misc + 4) |= 0x40000000;
            AGB_EncryptBoxMon(mon);
            return;
        }
    }
    switch (field) {
    case AGB_MON_DATA_NICKNAME:
        for (i = 0; i < 10; i++) {
            box[i + 8] = in[i];
        }
        break;
    case AGB_MON_DATA_LANGUAGE:
        box[0x12] = in[0];
        break;
    case AGB_MON_DATA_IS_BAD_EGG:
        box[0x13] = (box[0x13] & ~1) | (in[0] & 1);
        break;
    case AGB_MON_DATA_HAS_SPECIES:
        box[0x13] = (box[0x13] & ~2) | ((in[0] & 1) << 1);
        break;
    case AGB_MON_DATA_SANITY_IS_EGG:
        box[0x13] = (box[0x13] & ~4) | ((in[0] & 1) << 2);
        break;
    case AGB_MON_DATA_OT_NAME:
        for (i = 0; i < 7; i++) {
            box[i + 0x14] = in[i];
        }
        break;
    case AGB_MON_DATA_SPECIES:
        *(u16 *)growth = in[0] + (in[1] << 8);
        if (*(u16 *)growth != 0) {
            box[0x13] |= 2;
        } else {
            box[0x13] &= ~2;
        }
        break;
    }
    if (field > AGB_MON_DATA_UNKNOWN_10) {
        mon->checksum = AGB_CalcBoxMonChecksum(mon);
        AGB_EncryptBoxMon(mon);
    }
}

// GBA species index -> national dex number (they differ from Treecko on, index 277)
int TranslateAgbSpecies(int species) {
    u32 i;
    if (species >= 277) {
        for (i = 0; i < 144; i++) {
            if (species == ov74_0223CC5C[i * 2]) {
                return ov74_0223CC5E[i * 2];
            }
        }
    }
    return species;
}

u32 AGB_GetBoxMonLevel(AgbBoxMon *mon) {
    int species = TranslateAgbSpecies(AGB_GetBoxMonData(mon, AGB_MON_DATA_SPECIES, NULL));
    u32 exp = AGB_GetBoxMonData(mon, AGB_MON_DATA_EXPERIENCE, NULL);
    return CalcLevelBySpeciesAndExp((u16)species, exp);
}

// Species that got a second ability in DP keep their first one (sAgbKeepFirstAbilitySpecies); the others keep their ability slot
int AGB_GetBoxMonAbility(AgbBoxMon *agbMon, BoxPokemon *boxMon) {
    u16 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    u32 abilityNum = AGB_GetBoxMonData(agbMon, AGB_MON_DATA_ABILITY_NUM, NULL);
    int ability = GetMonBaseStat(species, BASE_ABILITY_2);
    u32 i;

    if (ability != 0) {
        for (i = 0; i < 94; i++) {
            if (sAgbKeepFirstAbilitySpecies[i] == species) {
                ability = GetMonBaseStat(species, BASE_ABILITY_1);
                break;
            }
        }
        if (i == 94 && !(abilityNum & 1)) {
            return GetMonBaseStat(species, BASE_ABILITY_1);
        }
    } else {
        ability = GetMonBaseStat(species, BASE_ABILITY_1);
    }
    return ability;
}

#define COPY_FIELD(agbField, field)                         \
    do {                                                    \
        data = AGB_GetBoxMonData(agbMon, (agbField), NULL); \
        SetBoxMonData(boxMon, (field), &data);              \
    } while (0)

// Sets one DP ribbon per contest ribbon level the GBA Pokemon has
#define COPY_CONTEST_RIBBONS(agbField, firstRibbon)              \
    do {                                                         \
        j = AGB_GetBoxMonData(agbMon, (agbField), NULL);         \
        if (j <= 4) {                                            \
            for (i = 0; i < j; i++) {                            \
                data = 1;                                        \
                SetBoxMonData(boxMon, i + (firstRibbon), &data); \
            }                                                    \
        }                                                        \
    } while (0)

// Converts a GBA box Pokemon into a DP one (Pal Park migration)
void MigrateBoxMon(AgbBoxMon *agbMon, BoxPokemon *boxMon) {
    u32 lock;
    u32 data;
    volatile u32 language;
    u8 srcString[12];
    u16 dstString[12];
    int j;
    int i;

    ZeroBoxMonData(boxMon);
    lock = AcquireBoxMonLock(boxMon);
    COPY_FIELD(AGB_MON_DATA_PERSONALITY, MON_DATA_PERSONALITY);
    data = AGB_GetBoxMonData(agbMon, AGB_MON_DATA_SPECIES, NULL);
    data = TranslateAgbSpecies(data);
    SetBoxMonData(boxMon, MON_DATA_SPECIES, &data);
    data = AGB_GetBoxMonData(agbMon, AGB_MON_DATA_HELD_ITEM, NULL);
    if (data != 0) {
        data = UpConvertItemId_Gen3to4((u16)data);
    }
    SetBoxMonData(boxMon, MON_DATA_HELD_ITEM, &data);
    COPY_FIELD(AGB_MON_DATA_OT_ID, MON_DATA_OT_ID);
    COPY_FIELD(AGB_MON_DATA_EXPERIENCE, MON_DATA_EXPERIENCE);
    data = 70;
    SetBoxMonData(boxMon, MON_DATA_FRIENDSHIP, &data);
    data = AGB_GetBoxMonAbility(agbMon, boxMon);
    SetBoxMonData(boxMon, MON_DATA_ABILITY, &data);
    COPY_FIELD(AGB_MON_DATA_MARKINGS, MON_DATA_MARKINGS);
    language = AGB_GetBoxMonData(agbMon, AGB_MON_DATA_LANGUAGE, NULL);
    SetBoxMonData(boxMon, MON_DATA_LANGUAGE, &language);
    COPY_FIELD(AGB_MON_DATA_HP_EV, MON_DATA_HP_EV);
    COPY_FIELD(AGB_MON_DATA_ATK_EV, MON_DATA_ATK_EV);
    COPY_FIELD(AGB_MON_DATA_DEF_EV, MON_DATA_DEF_EV);
    COPY_FIELD(AGB_MON_DATA_SPEED_EV, MON_DATA_SPEED_EV);
    COPY_FIELD(AGB_MON_DATA_SPATK_EV, MON_DATA_SPATK_EV);
    COPY_FIELD(AGB_MON_DATA_SPDEF_EV, MON_DATA_SPDEF_EV);
    COPY_FIELD(AGB_MON_DATA_COOL, MON_DATA_COOL);
    COPY_FIELD(AGB_MON_DATA_BEAUTY, MON_DATA_BEAUTY);
    COPY_FIELD(AGB_MON_DATA_CUTE, MON_DATA_CUTE);
    COPY_FIELD(AGB_MON_DATA_SMART, MON_DATA_SMART);
    COPY_FIELD(AGB_MON_DATA_TOUGH, MON_DATA_TOUGH);
    COPY_FIELD(AGB_MON_DATA_SHEEN, MON_DATA_SHEEN);
    for (i = 0, j = 0; i < 4; i++, j += 2) {
        COPY_FIELD(i + AGB_MON_DATA_MOVE1, i + MON_DATA_MOVE1);
        data = AGB_GetBoxMonData(agbMon, AGB_MON_DATA_PP_UPS, NULL);
        data = (data & (3 << j)) >> j;
        SetBoxMonData(boxMon, i + MON_DATA_MOVE1_PP_UPS, &data);
        data = GetBoxMonData(boxMon, i + MON_DATA_MOVE1_MAX_PP, NULL);
        SetBoxMonData(boxMon, i + MON_DATA_MOVE1_PP, &data);
    }
    COPY_FIELD(AGB_MON_DATA_HP_IV, MON_DATA_HP_IV);
    COPY_FIELD(AGB_MON_DATA_ATK_IV, MON_DATA_ATK_IV);
    COPY_FIELD(AGB_MON_DATA_DEF_IV, MON_DATA_DEF_IV);
    COPY_FIELD(AGB_MON_DATA_SPEED_IV, MON_DATA_SPEED_IV);
    COPY_FIELD(AGB_MON_DATA_SPATK_IV, MON_DATA_SPATK_IV);
    COPY_FIELD(AGB_MON_DATA_SPDEF_IV, MON_DATA_SPDEF_IV);
    COPY_FIELD(AGB_MON_DATA_IS_EGG, MON_DATA_IS_EGG);
    COPY_CONTEST_RIBBONS(AGB_MON_DATA_COOL_RIBBONS, MON_DATA_COOL_RIBBON);
    COPY_CONTEST_RIBBONS(AGB_MON_DATA_BEAUTY_RIBBONS, MON_DATA_BEAUTY_RIBBON);
    COPY_CONTEST_RIBBONS(AGB_MON_DATA_CUTE_RIBBONS, MON_DATA_CUTE_RIBBON);
    COPY_CONTEST_RIBBONS(AGB_MON_DATA_SMART_RIBBONS, MON_DATA_SMART_RIBBON);
    COPY_CONTEST_RIBBONS(AGB_MON_DATA_TOUGH_RIBBONS, MON_DATA_TOUGH_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON, MON_DATA_CHAMPION_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 1, MON_DATA_WINNING_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 2, MON_DATA_VICTORY_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 3, MON_DATA_ARTIST_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 4, MON_DATA_EFFORT_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 5, MON_DATA_MARINE_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 6, MON_DATA_LAND_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 7, MON_DATA_SKY_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 8, MON_DATA_COUNTRY_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 9, MON_DATA_NATIONAL_RIBBON);
    COPY_FIELD(AGB_MON_DATA_CHAMPION_RIBBON + 10, MON_DATA_EARTH_RIBBON);
    COPY_FIELD(AGB_MON_DATA_WORLD_RIBBON, MON_DATA_WORLD_RIBBON);
    COPY_FIELD(AGB_MON_DATA_FATEFUL_ENCOUNTER, MON_DATA_FATEFUL_ENCOUNTER);
    data = GetBoxMonGender(boxMon);
    SetBoxMonData(boxMon, MON_DATA_GENDER, &data);
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL) == SPECIES_UNOWN) {
        // the Unown letter comes from the personality
        u32 personality = AGB_GetBoxMonData(agbMon, AGB_MON_DATA_PERSONALITY, NULL);
        data = personality;
        u32 mask = 3;
        u32 a = ((mask << 8) & personality) >> 6;
        u32 b = ((mask << 24) & personality) >> 18;
        u32 c = ((mask << 16) & personality) >> 12;
        u32 d = personality & mask;
        data = (b | c | a | d) % 28;
        SetBoxMonData(boxMon, MON_DATA_FORM, &data);
    }
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL) == SPECIES_DEOXYS) {
        // the Deoxys form depends on the GBA game it comes from
        switch (gSystem.unk6A) {
        case 0:
        case 1:
        case 2:
        default:
            data = 0;
            break;
        case 4:
            data = 1;
            break;
        case 5:
            data = 2;
            break;
        case 3:
            data = 3;
            break;
        }
        SetBoxMonData(boxMon, MON_DATA_FORM, &data);
    }
    AGB_GetBoxMonData(agbMon, AGB_MON_DATA_NICKNAME, srcString);
    ConvertRSStringToDPStringInternational(srcString, dstString, 12, language);
    SetBoxMonData(boxMon, MON_DATA_NICKNAME_FLAT_COMPARE, dstString);
    if (AGB_GetBoxMonData(agbMon, AGB_MON_DATA_LANGUAGE, NULL) != gGameLanguage) {
        data = 1;
        SetBoxMonData(boxMon, MON_DATA_HAS_NICKNAME, &data);
    }
    COPY_FIELD(AGB_MON_DATA_MET_GAME, MON_DATA_MET_GAME);
    AGB_GetBoxMonData(agbMon, AGB_MON_DATA_OT_NAME, srcString);
    ConvertRSStringToDPStringInternational(srcString, dstString, 8, language);
    SetBoxMonData(boxMon, MON_DATA_OT_NAME, dstString);
    COPY_FIELD(AGB_MON_DATA_MET_LOCATION, MON_DATA_MET_LOCATION);
    COPY_FIELD(AGB_MON_DATA_POKERUS, MON_DATA_POKERUS);
    COPY_FIELD(AGB_MON_DATA_POKEBALL, MON_DATA_POKEBALL);
    COPY_FIELD(AGB_MON_DATA_MET_LEVEL, MON_DATA_MET_LEVEL);
    COPY_FIELD(AGB_MON_DATA_OT_GENDER, MON_DATA_OT_GENDER);
    ReleaseBoxMonLock(boxMon, lock);
}
