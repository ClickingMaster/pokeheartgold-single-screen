#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/os/valarm.h>

#include "spi_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void SPIi_ReturnResult(u16 command, u16 result);

void TP_Init(void);
void SPI_DummyWait_03800EE0(void);
void TP_AnalyzeCommand(u32 word);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void TP_Init(void) {
    int i;
    tpw.state = 0;
    tpw.range = 20;
    tpw.minRange = 20;
    for (i = 0; i < 16; i++) {
        tpw.command[i] = 0;
    }
    if (!OS_IsVAlarmAvailable()) {
        OS_InitVAlarm();
    }
    for (i = 0; i < 4; i++) {
        OS_CreateVAlarm(&tpw.alarms[i]);
        OS_SetVAlarmTag(&tpw.alarms[i], TP_VALARM_TAG);
    }
    while (REG_SPICNT & SPICNT_BUSY) {}
    REG_SPICNT = 0x8a01;
    REG_SPIDATA = 0x84;
    while (REG_SPICNT & SPICNT_BUSY) {}
    SPI_DummyWait_03800EE0();
    REG_SPICNT = 0x8201;
    SPI_DummyWait_03800EE0();
}

void SPI_DummyWait_03800EE0(void) {
    REG_SPIDATA = 0;
    while (REG_SPICNT & SPICNT_BUSY) {}
}

void TP_AnalyzeCommand(u32 word) {
    u16 command;
    int i;
    if (word & 0x02000000) {
        for (i = 0; i < 16; i++) {
            tpw.command[i] = 0;
        }
    }
    tpw.command[(word & 0xf0000) >> 16] = word;
    if (!(word & 0x01000000)) {
        return;
    }
    command = (tpw.command[0] & 0xff00) >> 8;
    switch (command) {
    case 3: {
        u16 low = (u16)(tpw.command[0] & 0xff);
        if (!low) {
            SPIi_ReturnResult(3, 2);
        } else {
            tpw.range = low;
            tpw.minRange = low;
            SPIi_ReturnResult(3, 0);
        }
        break;
    }
    case 0:
        if (!SPIi_SetEntry(0, command, 0)) {
            SPIi_ReturnResult(command, 4);
        }
        break;
    case 1: {
        u16 low;
        if (tpw.state) {
            SPIi_ReturnResult(command, 3);
            break;
        }
        low = (u16)(tpw.command[0] & 0xff);
        if (!low || low > 4) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        if (tpw.command[1] >= 263) {
            SPIi_ReturnResult(command, 2);
            break;
        }
        if (SPIi_SetEntry(0, command, 2, (u32)low, (u32)tpw.command[1])) {
            tpw.state = 1;
        } else {
            SPIi_ReturnResult(command, 4);
        }
        break;
    }
    case 2:
        if (tpw.state != 2) {
            SPIi_ReturnResult(command, 3);
            break;
        }
        if (SPIi_SetEntry(0, command, 0)) {
            tpw.state = 3;
        } else {
            SPIi_ReturnResult(command, 4);
        }
        break;
    default:
        SPIi_ReturnResult(command, 1);
        break;
    }
}

#pragma dont_inline reset
