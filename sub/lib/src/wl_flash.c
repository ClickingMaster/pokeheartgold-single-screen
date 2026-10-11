#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Checks the CRC of the wireless settings in the flash image (WlWork.flashImage: CRC, size, data).
// Returns 0 when it matches (2: bad size); `out` gets the stored CRC | computed CRC << 16.
u32 FLASH_VerifyCheckSum(u32 *out) {
    u8 *image = wlWorkPlain->flashImage;
    u32 crc;
    u32 count = ((u16 *)image)[1];
    u8 *cursor;
    if (count < 0xA4 || count > 0x1D6) {
        return 2;
    }
    cursor = image + 2;
    crc = 0;
    while (count) {
        u8 byte = WL_ReadByte(cursor);
        cursor++;
        crc = calc_NextCRC(byte & 0xFF, (u16)crc); // (narrowed like the original's u8 parameter)
        count--;
    }
    image = wlWorkPlain->flashImage;
    {
        u32 saved = ((u16 *)image)[0];
        if (out) {
            *out = saved | (crc << 16);
        }
        return crc != saved;
    }
}

// Waits until the flash is not busy (resetting it after a write error).
void FLASH_Wait(void) {
    vu32 status;
    for (;;) {
        __VENEER_NVRAM_ReadStatusRegister((u8 *)&status);
        if (status & 0x20) {
            __VENEER_NVRAM_SoftwareReset();
            continue;
        }
        if (!(status & 1)) {
            break;
        }
    }
}

// clang-format off
asm void __VENEER_NVRAM_ReadStatusRegister(u8 *out) {
    ldr pc, =NVRAM_ReadStatusRegister
}
// clang-format on

// clang-format off
asm void __VENEER_NVRAM_SoftwareReset(void) {
    ldr pc, =NVRAM_SoftwareReset
}
// clang-format on

// Reads `size` bytes at flash address `offset` (0x2A and up) from the flash image.
void FLASH_Read(u32 offset, u32 size, void *destination) {
    u8 *image = wlWorkPlain->flashImage;
    u8 *out = destination;
    const u8 *source;
    if (image) {
        source = (const u8 *)((u32)image + offset - 0x2A);
        while (size) {
            u8 byte = WL_ReadByte(source);
            source++;
            WL_WriteByte(out, byte);
            out++;
            size--;
        }
    }
}

// Reads the flash itself.
void FLASH_DirectRead(u32 offset, u32 size, void *destination) {
    __VENEER_SPI_Lock(wlWorkPlain->lockId);
    FLASH_Wait();
    __VENEER_NVRAM_ReadDataBytes(offset, size, destination);
    __VENEER_SPI_Unlock(wlWorkPlain->lockId);
}

// clang-format off
asm void __VENEER_SPI_Lock(u32 id) {
    ldr pc, =SPI_Lock
}
// clang-format on

// clang-format off
asm void __VENEER_NVRAM_ReadDataBytes(u32 address, u32 length, void *out) {
    ldr pc, =NVRAM_ReadDataBytes
}
// clang-format on

// clang-format off
asm void __VENEER_SPI_Unlock(u32 id) {
    ldr pc, =SPI_Unlock
}
// clang-format on

// Copies the wireless settings of the flash (from 0x2A; the size is at 0x2C) into a driver heap buffer.
int FLASH_MakeImage(void) {
    u32 size;
    __VENEER_SPI_Lock(wlWorkPlain->lockId);
    FLASH_Wait();
    size = 0;
    __VENEER_NVRAM_ReadDataBytes(0x2C, 2, &size);
    __VENEER_SPI_Unlock(wlWorkPlain->lockId);
    if (size < 0xA4 || size > 0x1D6) {
        return 0;
    }
    size += 2;
    wlWorkPlain->flashImage = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], size);
    if (!wlWorkPlain->flashImage) {
        return 0;
    }
    wlWorkPlain->flashImage += 0xC;
    __VENEER_SPI_Lock(wlWorkPlain->lockId);
    FLASH_Wait();
    __VENEER_NVRAM_ReadDataBytes(0x2A, size, wlWorkPlain->flashImage);
    __VENEER_SPI_Unlock(wlWorkPlain->lockId);
    return 1;
}

#pragma dont_inline reset
