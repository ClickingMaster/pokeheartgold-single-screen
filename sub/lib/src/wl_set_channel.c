#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Tunes the RF/BBP to a channel with the values from the flash (`direct`: FLASH_DirectRead).
// Returns 5 for a channel that is not enabled.
int WSetChannel(u16 channel, int direct) {
    void (*read)(u32, u32, void *) = direct ? FLASH_DirectRead : FLASH_Read;
    u16 saved;
    u32 state, value, address;
    if (!(wlWorkPlain->param.enableChannel & (1u << channel))) {
        return 5;
    }
    saved = WL_REG(0x040);
    WL_REG(0x040) = 0x8001;
    {
        int busy, power;
        do {
            power = WL_REG(0x03C) >> 8;
            busy = WL_REG(0x214);
        } while (power != 2 || (busy != 0 && busy != 9));
    }
    wlWorkPlain->bss.channel = channel;
    state = wlWorkPlain->rf.type;
    switch (state) {
    case 2:
    case 5: {
        u32 index = (u32)channel - 1u;
        u32 offset = index * 6;
        value = 0;
        read(offset + 0xF2, 3, &value);
        RF_Write(value);
        read(offset + 0xF5, 3, &value);
        RF_Write(value);
        value = 0;
        if (wlWorkPlain->rf.unk_0c & 0x10000) {
            if (!(wlWorkPlain->rf.unk_0c & 0x8000)) {
                read(index + 0x154, 1, &value);
                value = wlWorkPlain->rf.unk_0c | ((value << 27) >> 17);
                RF_Write(value);
            }
        } else {
            read(index + 0x146, 1, &value);
            BBP_Write(30, value);
        }
        break;
    }
    case 3: {
        u32 offset = wlWorkPlain->rf.initCount + 0xCF;
        u32 i;
        for (i = 0; i < wlWorkPlain->rf.bbpChannelCount; i++) {
            value = 0;
            address = 0;
            read(offset, 1, &address);
            read(offset + channel, 1, &value);
            BBP_Write(address, value);
            offset += 15;
        }
        for (i = 0; i < wlWorkPlain->rf.rfChannelCount; i++) {
            value = 0;
            read(offset, 1, &value);
            value <<= 8;
            read(offset + channel, 1, &value);
            value |= 0x50000;
            RF_Write(value);
            offset += 15;
        }
        break;
    }
    }
    WL_REG(0x040) = saved;
    WL_REG(0x048) = 3;
    return 0;
}
