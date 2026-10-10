#pragma force_active on
#include <nitro/types.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Sound channel registers: channel c at 0x04000400 + 16 * c.
#define R8(c, o)  (*(volatile u8 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R16(c, o) (*(volatile u16 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R32(c, o) (*(volatile u32 *)(0x04000000u + ((u32)(c) << 4) + (o)))

typedef struct SoundChannelRegisters {
    volatile u32 control;
    volatile u32 source;
    volatile u16 timer;
    volatile u16 loop;
    volatile u32 length;
} SoundChannelRegisters;

typedef struct SoundRegisters {
    u8 unk_000[0x400];
    SoundChannelRegisters channels[16];
} SoundRegisters;

extern int sMasterPan;
extern int sSurroundDecay;
extern u8 sOrgPan[16];
extern u8 sOrgVolume[16];
extern s8 SinTable[];

u32 SVC_GetPitchTable(int index);
u32 SVC_GetVolumeTable(int index);

void SND_SetupChannelPcm(int c, u32 src, int format, int repeat, int loop, u32 len, int volume, int shift, int timer, int pan);
void SND_SetupChannelPsg(int c, int duty, int volume, int shift, int timer, int pan);
void SND_SetupChannelNoise(int c, int volume, int shift, int timer, int pan);
void SND_StopChannel(int c, int flags);
void SND_SetChannelVolume(int c, int volume, int shift);
void SND_SetChannelTimer(int c, u32 timer);
void SND_SetChannelPan(int c, int pan);
int SND_IsChannelActive(int c);
void SND_SetMasterPan(int pan);
u32 SND_GetChannelControl(int c);
void SNDi_SetSurroundDecay(int decay);
int CalcSurroundDecay(int volume, int pan);
u16 SND_CalcTimer(int base, int pitch);
u32 __VENEER_SVC_GetPitchTable(int index);
u16 SND_CalcChannelVolume(int db);
u32 __VENEER_SVC_GetVolumeTable(int index);
s8 SND_SinIdx(int index);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SND_SetupChannelPcm(int c, u32 src, int format, int repeat, int loop, u32 len, int volume, int shift, int timer, int pan) {
    int master = sMasterPan;
    u32 offset = (u32)c << 4;
    sOrgPan[c] = pan;
    if (master >= 0) {
        pan = master;
    }
    sOrgVolume[c] = volume;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c))) {
        volume = CalcSurroundDecay(volume, pan);
    }
    R32(0, offset + 0x400) = ((u32)format << 29) | ((u32)repeat << 27) | ((u32)pan << 16) | ((u32)shift << 8) | (u32)volume;
    R16(0, offset + 0x408) = 0x10000u - timer;
    R16(0, offset + 0x40a) = loop;
    R32(0, offset + 0x40c) = len;
    R32(0, offset + 0x404) = (u32)src;
}

void SND_SetupChannelPsg(int c, int duty, int volume, int shift, int timer, int pan) {
    u32 channelOffset = (u32)c << 4;
    {
        int master = sMasterPan;
        sOrgPan[c] = pan;
        if (master >= 0) {
            pan = master;
        }
    }
    sOrgVolume[c] = volume;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c))) {
        volume = CalcSurroundDecay(volume, pan);
    }
    {
        u32 control = ((u32)duty << 24) | 0x60000000u | ((u32)pan << 16) | ((u32)shift << 8);
        *(volatile u32 *)(0x04000400u + channelOffset) = (u32)volume | control;
    }
    *(volatile u16 *)(0x04000408u + channelOffset) = 0x10000u - (u32)timer;
}

void SND_SetupChannelNoise(int c, int volume, int shift, int timer, int pan) {
    u32 channelOffset = (u32)c << 4;
    {
        int master = sMasterPan;
        sOrgPan[c] = pan;
        if (master >= 0) {
            pan = master;
        }
    }
    sOrgVolume[c] = volume;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c))) {
        volume = CalcSurroundDecay(volume, pan);
    }
    {
        u32 control = ((u32)pan << 16) | 0x60000000u | ((u32)shift << 8);
        *(volatile u32 *)(0x04000400u + channelOffset) = (u32)volume | control;
    }
    *(volatile u16 *)(0x04000408u + channelOffset) = 0x10000u - (u32)timer;
}

void SND_StopChannel(int c, int flags) {
    u32 offset = (u32)c << 4;
    SoundRegisters *input = (SoundRegisters *)(0x04000000u + offset);
    u32 value = input->channels[0].control & ~0x80000000u;
    volatile u32 *output = (volatile u32 *)(offset + 0x400u + 0x04000000u);
    if (flags & 1) {
        value |= 0x8000;
    }
    *output = value;
}

void SND_SetChannelVolume(int c, int volume, int shift) {
    int decay = sSurroundDecay;
    sOrgVolume[c] = volume;
    if (decay > 0 && (0xfff5u & (1u << c))) {
        int pan = R8(c, 0x402);
        volume = CalcSurroundDecay(volume, pan);
    }
    R16(c, 0x400) = (u32)volume | ((u32)shift << 8);
}

void SND_SetChannelTimer(int c, u32 timer) {
    R16(c, 0x408) = 0x10000u - timer;
}

void SND_SetChannelPan(int c, int pan) {
    sOrgPan[c] = pan;
    if (sMasterPan >= 0) {
        pan = sMasterPan;
    }
    R8(c, 0x402) = pan;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c))) {
        R8(c, 0x400) = CalcSurroundDecay(sOrgVolume[c], pan);
    }
}

int SND_IsChannelActive(int c) {
    return (R8(c, 0x403) & 0x80) != 0;
}

void SND_SetMasterPan(int pan) {
    int i;
    sMasterPan = pan;
    if (pan >= 0) {
        for (i = 0; i < 16; i++) {
            R8(i, 0x402) = pan;
        }
    } else {
        for (i = 0; i < 16; i++) {
            R8(i, 0x402) = sOrgPan[i];
        }
    }
}

u32 SND_GetChannelControl(int c) {
    return R32(c, 0x400);
}

#pragma dont_inline reset

static inline void ApplyChannelSurroundDecay(int i, const u8 *volume) {
    u32 offset = (u32)i << 4;
    int pan = *(volatile u8 *)(0x04000000u + offset + 0x402u);
    int value = CalcSurroundDecay(volume[i], pan);
    *(volatile u8 *)(0x04000000u + offset + 0x400u) = value;
}

void SNDi_SetSurroundDecay(int decay) {
    u32 mask = 0xfff5;
    int i;
    sSurroundDecay = decay;
    for (i = 0; i < 16; i++) {
        if (mask & (1u << i)) {
            ApplyChannelSurroundDecay(i, sOrgVolume);
        }
    }
}

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

int CalcSurroundDecay(int volume, int pan) {
    u32 factor;
    if (pan < 24) {
        factor = (u32)sSurroundDecay * (u32)(pan + 40) + ((0x7fffu - (u32)sSurroundDecay) << 6);
        return (s32)((u32)volume * factor) >> 21;
    }
    if (pan > 104) {
        factor = (0u - (u32)sSurroundDecay) * (u32)(pan - 40) + (((u32)sSurroundDecay + 0x7fffu) << 6);
        return (s32)((u32)volume * factor) >> 21;
    }
    return volume;
}

u16 SND_CalcTimer(int base, int pitch) {
    int octave = 0;
    int rem = (s32)(0u - (u32)pitch);
    u64 timer, rate;
    while (rem < 0) {
        octave--;
        rem += 768;
    }
    while (rem >= 768) {
        octave++;
        rem -= 768;
    }
    rate = (u64)__VENEER_SVC_GetPitchTable(rem) + 0x10000u;
    timer = rate * (u64)(s64)base;
    octave -= 16;
    if (octave <= 0) {
        u32 shift = 0u - (u32)octave;
        timer >>= shift;
    } else {
        if (octave < 32) {
            if (timer & (~(u64)0 << (32 - octave))) {
                return 65535;
            }
            timer <<= octave;
        } else {
            return 65535;
        }
    }
    if (timer < 16) {
        timer = 16;
    } else if (timer > 65535) {
        timer = 65535;
    }
    return (u16)timer;
}

u32 __VENEER_SVC_GetPitchTable(int index) {
    return SVC_GetPitchTable(index);
}

u16 SND_CalcChannelVolume(int db) {
    u32 value, shift;
    if (db < -723) {
        db = -723;
    } else if (db > 0) {
        db = 0;
    }
    value = __VENEER_SVC_GetVolumeTable(db + 723);
    if (db < -240) {
        shift = 3;
    } else if (db < -120) {
        shift = 2;
    } else if (db < -60) {
        shift = 1;
    } else {
        shift = 0;
    }
    return (u16)(value | (shift << 8));
}

u32 __VENEER_SVC_GetVolumeTable(int index) {
    return SVC_GetVolumeTable(index);
}

s8 SND_SinIdx(int index) {
    if (index < 32) {
        return SinTable[index];
    }
    if (index < 64) {
        return SinTable[64 - index];
    }
    if (index < 96) {
        return -SinTable[index - 64];
    }
    index -= 96;
    index = 32 - index;
    return -SinTable[index];
}

#pragma dont_inline reset
