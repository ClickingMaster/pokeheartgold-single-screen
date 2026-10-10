#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/os/system.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define SPICNT_NVRAM_CONTINUOUS 0x8900
#define SPICNT_NVRAM_LAST       0x8100

typedef struct NvramRequest {
    u32 module;
    u32 command;
    u32 address;
    u32 length;
    void *data;
} NvramRequest;

void SPIi_ReturnResult(u16 command, u32 result);

void NVRAM_ExecuteProcess(NvramRequest *request);
int NvramCheckReadyToRead(void);
int NvramCheckReadyToWrite(void);
void NVRAM_WriteEnable(void);
void SPI_SendWait_03803D98(u32 data);
void NVRAM_WriteDisable(void);
void NVRAM_ReadStatusRegister(u8 *out);
void NVRAM_ReadDataBytes(u32 address, u32 length, u8 *out);
void SPI_DummyWait_03803F70(void);
void NVRAM_ReadDataBytesAtHigherSpeed(u32 address, u32 length, u8 *out);
void NVRAM_PageWrite(u32 address, u16 length, const u8 *in);
void NVRAM_PageProgram(u32 address, u16 length, const u8 *in);
void NVRAM_PageErase(u32 address);
void NVRAM_SectorErase(u32 address);
void NVRAM_DeepPowerDown(void);
void NVRAM_ReleaseFromDeepPowerDown(void);
void NVRAM_ChipErase(void);
void NVRAM_ReadSiliconId(u8 *out);
void NVRAM_SoftwareReset(void);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void NVRAM_ExecuteProcess(NvramRequest *request) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (!SPIi_CheckException(1)) {
        OS_RestoreInterrupts(irq);
        SPIi_ReturnResult((u16)request->command, 4);
        return;
    }
    SPIi_GetException(1);
    OS_RestoreInterrupts(irq);
    switch (request->command) {
    case 0x20:
        NVRAM_WriteEnable();
        break;
    case 0x21:
        NVRAM_WriteDisable();
        break;
    case 0x22:
        NVRAM_ReadStatusRegister(request->data);
        break;
    case 0x23:
        if (!NvramCheckReadyToRead()) {
            SPIi_ReturnResult((u16)request->command, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_ReadDataBytes(request->address, request->length, request->data);
        break;
    case 0x24:
        if (!NvramCheckReadyToRead()) {
            SPIi_ReturnResult((u16)request->command, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_ReadDataBytesAtHigherSpeed(request->address, request->length, request->data);
        break;
    case 0x25:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)request->command, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_PageWrite(request->address, (u16)request->length, request->data);
        break;
    case 0x26:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)request->command, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_PageProgram(request->address, (u16)request->length, request->data);
        break;
    case 0x27:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)request->command, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_PageErase(request->address);
        break;
    case 0x28:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)request->command, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_SectorErase(request->address);
        break;
    case 0x29:
        NVRAM_DeepPowerDown();
        break;
    case 0x2a:
        NVRAM_ReleaseFromDeepPowerDown();
        break;
    case 0x2b:
        if (!NvramCheckReadyToWrite()) {
            SPIi_ReturnResult((u16)request->command, 3);
            SPIi_ReleaseException(1);
            return;
        }
        NVRAM_ChipErase();
        break;
    case 0x2c:
        NVRAM_ReadSiliconId(request->data);
        break;
    case 0x2d:
        NVRAM_SoftwareReset();
        break;
    default:
        SPIi_ReleaseException(1);
        SPIi_ReturnResult((u16)request->command, 1);
        return;
    }
    SPIi_ReturnResult((u16)request->command, 0);
    SPIi_ReleaseException(1);
}

int NvramCheckReadyToRead(void) {
    u16 status;
    NVRAM_ReadStatusRegister((u8 *)&status);
    return (status & 1) == 0;
}

int NvramCheckReadyToWrite(void) {
    u16 status;
    NVRAM_ReadStatusRegister((u8 *)&status);
    if (status & 1) {
        return 0;
    }
    return (status & 2) != 0;
}

void NVRAM_WriteEnable(void) {
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_LAST;
    REG_SPIDATA = 0x06;
    while (REG_SPICNT & SPICNT_BUSY) {}
}

void SPI_SendWait_03803D98(u32 data) {
    REG_SPIDATA = data & 0xFF;
    while (REG_SPICNT & SPICNT_BUSY) {}
}

void NVRAM_WriteDisable(void) {
    volatile u16 *control = &REG_SPICNT;
    volatile u16 *data;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_LAST;
    *data = 0x04;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
}

void NVRAM_ReadStatusRegister(u8 *out) {
    volatile u16 *control = &REG_SPICNT;
    volatile u16 *data;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_CONTINUOUS;
    *data = 0x05;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_LAST;
    *data = 0;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
    *out = (u8)REG_SPIDATA;
}

void NVRAM_ReadDataBytes(u32 address, u32 length, u8 *out) {
    u16 addressBytes[3];
    int j;
    u32 k;
    if (length < 1) {
        return;
    }
    addressBytes[0] = (address & 0xff0000) >> 16;
    addressBytes[1] = (address & 0xff00) >> 8;
    addressBytes[2] = address & 0xff;
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_CONTINUOUS;
    REG_SPIDATA = 0x03;
    j = 0;
    for (; j < 3; j++) {
        while (REG_SPICNT & SPICNT_BUSY) {}
        REG_SPIDATA = addressBytes[j] & 0xff;
    }
    while (REG_SPICNT & SPICNT_BUSY) {}
    for (k = 0; k < length - 1; k++) {
        REG_SPIDATA = 0;
        while (REG_SPICNT & SPICNT_BUSY) {}
        out[k] = (u8)REG_SPIDATA;
    }
    REG_SPICNT = SPICNT_NVRAM_LAST;
    SPI_DummyWait_03803F70();
    out[k] = (u8)REG_SPIDATA;
}

void SPI_DummyWait_03803F70(void) {
    volatile u16 *data = &REG_SPIDATA;
    volatile u16 *control;
    *data = 0;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
}

void NVRAM_ReadDataBytesAtHigherSpeed(u32 address, u32 length, u8 *out) {
    u16 addressBytes[3];
    int j;
    u32 k;
    if (length < 1) {
        return;
    }
    addressBytes[0] = (address & 0xff0000) >> 16;
    addressBytes[1] = (address & 0xff00) >> 8;
    addressBytes[2] = address & 0xff;
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_CONTINUOUS;
    REG_SPIDATA = 0x0B;
    j = 0;
    for (; j < 3; j++) {
        while (REG_SPICNT & SPICNT_BUSY) {}
        REG_SPIDATA = addressBytes[j] & 0xff;
    }
    while (REG_SPICNT & SPICNT_BUSY) {}
    // one dummy byte before the data
    REG_SPIDATA = 0;
    while (REG_SPICNT & SPICNT_BUSY) {}
    for (k = 0; k < length - 1; k++) {
        REG_SPIDATA = 0;
        while (REG_SPICNT & SPICNT_BUSY) {}
        out[k] = (u8)REG_SPIDATA;
    }
    REG_SPICNT = SPICNT_NVRAM_LAST;
    SPI_DummyWait_03803F70();
    out[k] = (u8)REG_SPIDATA;
}

void NVRAM_PageWrite(u32 address, u16 length, const u8 *in) {
    u16 addressBytes[3];
    int j, limit;
    if (length < 1) {
        return;
    }
    // a page write never crosses a 256-byte page
    if (((address + length - 1) >> 8) > (address >> 8)) {
        length = (u16)(256 - (address & 0xff));
    }
    addressBytes[0] = (address & 0xff0000) >> 16;
    addressBytes[1] = (address & 0xff00) >> 8;
    addressBytes[2] = address & 0xff;
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_CONTINUOUS;
    REG_SPIDATA = 0x0A;
    j = 0;
    for (; j < 3; j++) {
        while (REG_SPICNT & SPICNT_BUSY) {}
        REG_SPIDATA = addressBytes[j] & 0xff;
    }
    limit = length - 1;
    for (j = 0; j < limit; j++) {
        while (REG_SPICNT & SPICNT_BUSY) {}
        REG_SPIDATA = in[j];
    }
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_LAST;
    SPI_SendWait_03803D98(in[j]);
}

void NVRAM_PageProgram(u32 address, u16 length, const u8 *in) {
    u16 addressBytes[3];
    int j, limit;
    if (length < 1) {
        return;
    }
    if (((address + length - 1) >> 8) > (address >> 8)) {
        length = (u16)(256 - (address & 0xff));
    }
    addressBytes[0] = (address & 0xff0000) >> 16;
    addressBytes[1] = (address & 0xff00) >> 8;
    addressBytes[2] = address & 0xff;
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_CONTINUOUS;
    REG_SPIDATA = 0x02;
    j = 0;
    for (; j < 3; j++) {
        while (REG_SPICNT & SPICNT_BUSY) {}
        REG_SPIDATA = addressBytes[j] & 0xff;
    }
    limit = length - 1;
    for (j = 0; j < limit; j++) {
        while (REG_SPICNT & SPICNT_BUSY) {}
        REG_SPIDATA = in[j];
    }
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_LAST;
    SPI_SendWait_03803D98(in[j]);
}

void NVRAM_PageErase(u32 address) {
    u16 high = (u16)((address & 0xff0000) >> 16);
    u16 mid = (u16)((address & 0xff00) >> 8);
    u16 low = (u16)(address & 0xff);
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_CONTINUOUS;
    REG_SPIDATA = 0xDB;
    while (REG_SPICNT & SPICNT_BUSY) {}
    SPI_SendWait_03803D98(high);
    SPI_SendWait_03803D98(mid);
    REG_SPICNT = SPICNT_NVRAM_LAST;
    SPI_SendWait_03803D98(low);
}

void NVRAM_SectorErase(u32 address) {
    u16 high = (u16)((address & 0xff0000) >> 16);
    u16 mid = (u16)((address & 0xff00) >> 8);
    u16 low = (u16)(address & 0xff);
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = SPICNT_NVRAM_CONTINUOUS;
    REG_SPIDATA = 0xD8;
    while (REG_SPICNT & SPICNT_BUSY) {}
    SPI_SendWait_03803D98(high);
    SPI_SendWait_03803D98(mid);
    REG_SPICNT = SPICNT_NVRAM_LAST;
    SPI_SendWait_03803D98(low);
}

void NVRAM_DeepPowerDown(void) {
    volatile u16 *control = &REG_SPICNT;
    volatile u16 *data;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_LAST;
    *data = 0xB9;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
}

void NVRAM_ReleaseFromDeepPowerDown(void) {
    volatile u16 *control = &REG_SPICNT;
    volatile u16 *data;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_LAST;
    *data = 0xAB;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
}

void NVRAM_ChipErase(void) {
    volatile u16 *control = &REG_SPICNT;
    volatile u16 *data;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_LAST;
    *data = 0xC7;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
}

void NVRAM_ReadSiliconId(u8 *out) {
    volatile u16 *control = &REG_SPICNT, *data;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_CONTINUOUS;
    *data = 0x9F;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *data = 0;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    out[0] = (u8)*data;
    *control = SPICNT_NVRAM_LAST;
    *data = 0;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
    out[1] = (u8)REG_SPIDATA;
}

void NVRAM_SoftwareReset(void) {
    volatile u16 *control = &REG_SPICNT;
    volatile u16 *data;
    while (*control & SPICNT_BUSY) {}
    data = &REG_SPIDATA;
    *control = SPICNT_NVRAM_LAST;
    *data = 0xFF;
    control = data - 1;
    while (*control & SPICNT_BUSY) {}
}

#pragma dont_inline reset
