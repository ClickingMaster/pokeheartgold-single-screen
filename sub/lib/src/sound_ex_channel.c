#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/SND_work_shared.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define SND_ENV_ATTENUATION_START (-92544) // -723 dB * 128

// Weak (bit 0 of the lock kind) and strong channel locks.
extern u32 sWeakLockChannel[2];
extern const s16 SNDi_DecibelTable[];
// Function-local tables of the original code, allocated by the sound data object.
extern const u8 attack_table$3790[19];
extern const u8 shift$4005[];
extern const u8 channel_order$3829[];

void SND_SetupChannelPcm(int channel, u32 src, int format, int repeat, int loop, u32 length, int volume, int shift, int timer, int pan);
void SND_SetupChannelPsg(int channel, u32 duty, int volume, int shift, int timer, int pan);
void SND_SetupChannelNoise(int channel, int volume, int shift, int timer, int pan);
void SND_StopChannel(int channel, int flags);
void SND_SetChannelTimer(int channel, int timer);
void SND_SetChannelVolume(int channel, int volume, int shift);
void SND_SetChannelPan(int channel, int pan);
int SND_IsChannelActive(int channel);
u16 SND_CalcChannelVolume(int db);
u16 SND_CalcTimer(int base, int pitch);
s8 SND_SinIdx(int index);

void SND_ExChannelInit(void);
void SND_UpdateExChannel(void);
void SND_ExChannelMain(int update);
int SND_StartExChannelPcm(struct SNDExChannel *chn, const struct SNDWaveParam *wave, const void *data, int length);
int SND_StartExChannelPsg(struct SNDExChannel *chn, s32 duty, int length);
int SND_StartExChannelNoise(struct SNDExChannel *chn, int length);
int SND_UpdateExChannelEnvelope(struct SNDExChannel *chn, int update);
void SND_SetExChannelAttack(struct SNDExChannel *chn, int attack);
void SND_SetExChannelDecay(struct SNDExChannel *chn, int decay);
void SND_SetExChannelSustain(struct SNDExChannel *chn, int sustain);
void SND_SetExChannelRelease(struct SNDExChannel *chn, int release);
void SND_ReleaseExChannel(struct SNDExChannel *chn);
int SND_IsExChannelActive(struct SNDExChannel *chn);
struct SNDExChannel *SND_AllocExChannel(u32 mask, int priority, int weak, SNDExChannelCallback callback, void *userData);
void SND_FreeExChannel(struct SNDExChannel *chn);
void SND_StopUnlockedChannel(u32 mask, u32 unused);
void SND_LockChannel(u32 mask, u32 kind);
void SND_UnlockChannel(u32 mask, u32 kind);
u32 SND_GetLockedChannel(u32 kind);
void SND_InvalidateWave(u32 start, u32 end);
void SND_InitLfoParam(struct SNDLfoParam *param);
void SND_UpdateLfo(struct SNDLfo *lfo);
int SND_GetLfoValue(struct SNDLfo *lfo);
u16 CalcRelease(int release);
void StartExChannel(struct SNDExChannel *chn, s32 length);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SND_ExChannelInit(void) {
    int i;
    for (i = 0; i < 16; i++) {
        SNDi_Work.channels[i].id = i;
        SNDi_Work.channels[i].flags.syncFlag = 0;
        SNDi_Work.channels[i].flags.active = 0;
    }
    sWeakLockChannel[1] = 0;
    sWeakLockChannel[0] = 0;
}

void SND_UpdateExChannel(void) {
    struct SNDExChannel *chn;
    int i;
    for (i = 0; i < 16; i++) {
        chn = &SNDi_Work.channels[i];
        u32 sync = chn->flags.syncFlag;
        if (!sync) {
            continue;
        }
        if (sync & SND_CHN_SYNC_STOP) {
            SND_StopChannel(i, 0);
        }
        sync = chn->flags.syncFlag;
        if (sync & SND_CHN_SYNC_START) {
            switch (chn->type) {
            case SND_CHN_TYPE_PCM:
                SND_SetupChannelPcm(i, (u32)chn->waveDataPtr, chn->waveParam.format, chn->waveParam.loopEnabled ? 1 : 2, chn->waveParam.loopStart, chn->waveParam.loopLength, chn->volume & 0xff, chn->volume >> 8, chn->timer, chn->pan);
                break;
            case SND_CHN_TYPE_PSG:
                SND_SetupChannelPsg(i, chn->dutyCycle, chn->volume & 0xff, chn->volume >> 8, chn->timer, chn->pan);
                break;
            case SND_CHN_TYPE_NOISE:
                SND_SetupChannelNoise(i, chn->volume & 0xff, chn->volume >> 8, chn->timer, chn->pan);
                break;
            }
        } else {
            if (sync & SND_CHN_SYNC_TIMER) {
                SND_SetChannelTimer(i, chn->timer);
            }
            if (chn->flags.syncFlag & SND_CHN_SYNC_VOLUME) {
                SND_SetChannelVolume(i, chn->volume & 0xff, chn->volume >> 8);
            }
            if (chn->flags.syncFlag & SND_CHN_SYNC_PAN) {
                SND_SetChannelPan(i, chn->pan);
            }
        }
    }
    for (i = 0; i < 16; i++) {
        chn = &SNDi_Work.channels[i];
        u32 sync = chn->flags.syncFlag;
        if (sync) {
            if (sync & SND_CHN_SYNC_START) {
                *(volatile u8 *)(0x04000403 + i * 16) |= 0x80;
            }
            chn->flags.syncFlag = 0;
        }
    }
}

void SND_ExChannelMain(int update) {
    struct SNDExChannel *chn;
    int i;
    int volume, pitch, pan;
    s64 lfo;
    int minimum = -32768;
    int run = update;
    for (i = 0; i < 16; i++) {
        int sweep;
        u16 timer, level;
        chn = &SNDi_Work.channels[i];
        volume = 0;
        pitch = 0;
        pan = 0;
        if (!chn->flags.active) {
            continue;
        }
        if (chn->flags.start) {
            chn->flags.syncFlag |= SND_CHN_SYNC_START;
            chn->flags.start = 0;
        } else if (!SND_IsChannelActive(i)) {
            if (!chn->callback) {
                chn->priority = 0;
            } else {
                chn->callback(chn, 1, chn->callbackUserData);
            }
            chn->volume = 0;
            chn->flags.active = 0;
            continue;
        }
        volume += SNDi_DecibelTable[chn->velocity];
        pitch += (chn->midiKey - chn->rootMidiKey) * 64;
        volume += SND_UpdateExChannelEnvelope(chn, run);
        if (!chn->sweepPitch) {
            sweep = 0;
        } else {
            // sweepCounter is read and written as a volatile here
            int counter = *(volatile s32 *)&chn->sweepCounter;
            if (counter >= chn->sweepLength) {
                sweep = 0;
            } else {
                sweep = (s32)((s64)chn->sweepPitch * (s32)((u32)chn->sweepLength - (u32)counter) / chn->sweepLength);
                if (run && chn->flags.autoSweep) {
                    *(volatile s32 *)&chn->sweepCounter = (s32)((u32) * (volatile s32 *)&chn->sweepCounter + 1);
                }
            }
        }
        pitch += sweep;
        volume += chn->userDecay;
        volume += chn->userDecay2;
        pitch += chn->userPitch;
        lfo = SND_GetLfoValue(&chn->lfo);
        if (lfo) {
            switch (chn->lfo.param.target) {
            case SND_LFO_VOLUME:
                lfo *= 60;
                break;
            case SND_LFO_PITCH:
                lfo = (s64)((u64)lfo << 6);
                break;
            case SND_LFO_PAN:
                lfo = (s64)((u64)lfo << 6);
                break;
            }
            lfo >>= 14;
        }
        if (run) {
            SND_UpdateLfo(&chn->lfo);
        }
        switch (chn->lfo.param.target) {
        case SND_LFO_VOLUME:
            if (volume > minimum) {
                volume += (s32)lfo;
            }
            break;
        case SND_LFO_PITCH:
            pitch += (s32)lfo;
            break;
        case SND_LFO_PAN:
            pan += (s32)lfo;
            break;
        }
        pan += chn->initPan;
        if (chn->panRange != 127) {
            pan = (pan * chn->panRange + 64) >> 7;
        }
        pan += chn->userPan;
        if (chn->envStatus == SND_ENV_RELEASE && volume <= -723) {
            chn->flags.syncFlag = SND_CHN_SYNC_STOP;
            if (!chn->callback) {
                chn->priority = 0;
            } else {
                chn->callback(chn, 1, chn->callbackUserData);
            }
            chn->volume = 0;
            chn->flags.active = 0;
            continue;
        }
        level = SND_CalcChannelVolume(volume);
        timer = SND_CalcTimer(chn->waveParam.timer, pitch);
        if (chn->type == SND_CHN_TYPE_PSG) {
            timer &= 0xfffc;
        }
        pan += 64;
        if (pan < 0) {
            pan = 0;
        } else if (pan > 127) {
            pan = 127;
        }
        if (level != chn->volume) {
            chn->volume = level;
            chn->flags.syncFlag |= SND_CHN_SYNC_VOLUME;
        }
        if (timer != chn->timer) {
            chn->timer = timer;
            chn->flags.syncFlag |= SND_CHN_SYNC_TIMER;
        }
        if (pan != chn->pan) {
            chn->pan = pan;
            chn->flags.syncFlag |= SND_CHN_SYNC_PAN;
        }
    }
}

int SND_StartExChannelPcm(struct SNDExChannel *chn, const struct SNDWaveParam *wave, const void *data, int length) {
    chn->type = SND_CHN_TYPE_PCM;
    chn->waveParam = *wave;
    chn->waveDataPtr = data;
    StartExChannel(chn, length);
    return 1;
}

// PSG duty-cycle channels are 8-13, noise channels 14-15.
int SND_StartExChannelPsg(struct SNDExChannel *chn, s32 duty, int length) {
    if (chn->id < 8) {
        return 0;
    }
    if (chn->id > 13) {
        return 0;
    }
    chn->type = SND_CHN_TYPE_PSG;
    chn->dutyCycle = duty;
    chn->waveParam.timer = 8006;
    StartExChannel(chn, length);
    return 1;
}

int SND_StartExChannelNoise(struct SNDExChannel *chn, int length) {
    if (chn->id < 14) {
        return 0;
    }
    if (chn->id > 15) {
        return 0;
    }
    chn->type = SND_CHN_TYPE_NOISE;
    chn->waveParam.timer = 8006;
    StartExChannel(chn, length);
    return 1;
}

int SND_UpdateExChannelEnvelope(struct SNDExChannel *chn, int update) {
    if (update) {
        switch (chn->envStatus) {
        case SND_ENV_ATTACK:
            chn->envAttenuation = (s32)(0u - ((u32)((s32)((0u - (u32)chn->envAttenuation) * chn->envAttack) >> 8)));
            if (chn->envAttenuation == 0) {
                chn->envStatus = SND_ENV_DECAY;
            }
            break;
        case SND_ENV_DECAY: {
            int sustain = SNDi_DecibelTable[chn->envSustain] * 128;
            chn->envAttenuation = (s32)((u32)chn->envAttenuation - chn->envDecay);
            if (chn->envAttenuation <= sustain) {
                chn->envAttenuation = sustain;
                chn->envStatus = SND_ENV_SUSTAIN;
            }
            break;
        }
        case SND_ENV_SUSTAIN:
            break;
        case SND_ENV_RELEASE:
            chn->envAttenuation = (s32)((u32)chn->envAttenuation - chn->envRelease);
            break;
        }
    }
    return chn->envAttenuation >> 7;
}

void SND_SetExChannelAttack(struct SNDExChannel *chn, int attack) {
    if (attack < 109) {
        chn->envAttack = (u8)(255u - (u32)attack);
    } else {
        chn->envAttack = attack_table$3790[127 - attack];
    }
}

void SND_SetExChannelDecay(struct SNDExChannel *chn, int decay) {
    chn->envDecay = CalcRelease(decay);
}

void SND_SetExChannelSustain(struct SNDExChannel *chn, int sustain) {
    chn->envSustain = sustain;
}

void SND_SetExChannelRelease(struct SNDExChannel *chn, int release) {
    chn->envRelease = CalcRelease(release);
}

void SND_ReleaseExChannel(struct SNDExChannel *chn) {
    chn->envStatus = SND_ENV_RELEASE;
}

int SND_IsExChannelActive(struct SNDExChannel *chn) {
    return chn->flags.active;
}

#pragma dont_inline reset

// Compares two channel volume words (low byte: volume, high byte: shift); 1 when a is quieter.
static inline int CmpVol(int a, int b) {
    int bv = (b & 0xff) << 4;
    int av = (a & 0xff) << 4;
    bv >>= shift$4005[b >> 8];
    av >>= shift$4005[a >> 8];
    if (av != bv) {
        if (av < bv) {
            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

struct SNDExChannel *SND_AllocExChannel(u32 mask, int priority, int weak, SNDExChannelCallback cb, void *userData) {
    SNDExChannelCallback callback;
    struct SNDExChannel *best;
    int i;
    struct SNDExChannel *chn;
    int channel;
    callback = cb;
    mask &= ~sWeakLockChannel[1];
    if (!weak) {
        mask &= ~sWeakLockChannel[0];
    }
    best = 0;
    for (i = 0; i < 16; i++) {
        channel = channel_order$3829[i];
        if (mask & (1u << channel)) {
            chn = &SNDi_Work.channels[channel];
            if (!best) {
                best = chn;
            } else if (chn->priority <= best->priority) {
                if (chn->priority == best->priority && CmpVol(best->volume, chn->volume) >= 0) {
                    continue;
                }
                best = chn;
            }
        }
    }
    if (!best) {
        return 0;
    }
    if (priority < best->priority) {
        return 0;
    }
    if (best->callback) {
        best->callback(best, 0, best->callbackUserData);
    }
    best->flags.syncFlag = 2;
    best->flags.active = 0;
    best->channelLLNext = 0;
    best->callback = callback;
    best->callbackUserData = userData;
    best->length = 0;
    best->priority = priority;
    best->volume = 127;
    best->flags.start = 0;
    best->flags.autoSweep = 1;
    best->midiKey = 60;
    best->rootMidiKey = 60;
    best->velocity = 127;
    best->initPan = 0;
    best->userDecay = 0;
    best->userDecay2 = 0;
    best->userPitch = 0;
    best->userPan = 0;
    best->panRange = 127;
    best->sweepPitch = 0;
    best->sweepLength = 0;
    best->sweepCounter = 0;
    best->envAttack = 0;
    best->envDecay = 65535;
    best->envSustain = 127;
    best->envRelease = 65535;
    SND_InitLfoParam(&best->lfo.param);
    return best;
}

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SND_FreeExChannel(struct SNDExChannel *chn) {
    if (chn) {
        chn->callback = NULL;
        chn->callbackUserData = NULL;
    }
}

void SND_StopUnlockedChannel(u32 mask, u32 unused) {
    struct SNDExChannel *chn;
    int i = 0;
    while (i < 16 && mask) {
        if (mask & 1) {
            u32 locked = sWeakLockChannel[1];
            chn = &SNDi_Work.channels[i];
            if (!(locked & (1u << i))) {
                if (chn->callback) {
                    chn->callback(chn, 0, chn->callbackUserData);
                }
                SND_StopChannel(i, 0);
                chn->priority = 0;
                SND_FreeExChannel(chn);
                chn->flags.syncFlag = 0;
                chn->flags.active = 0;
            }
        }
        i++;
        mask >>= 1;
    }
}

void SND_LockChannel(u32 mask, u32 kind) {
    struct SNDExChannel *chn;
    u32 bits;
    int i = 0;
    bits = mask;
    while (i < 16 && bits) {
        if (bits & 1) {
            u32 locked = sWeakLockChannel[1];
            chn = &SNDi_Work.channels[i];
            if (!(locked & (1u << i))) {
                if (chn->callback) {
                    chn->callback(chn, 0, chn->callbackUserData);
                }
                SND_StopChannel(i, 0);
                chn->priority = 0;
                SND_FreeExChannel(chn);
                chn->flags.syncFlag = 0;
                chn->flags.active = 0;
            }
        }
        i++;
        bits >>= 1;
    }
    if (kind & 1) {
        sWeakLockChannel[0] |= mask;
    } else {
        sWeakLockChannel[1] |= mask;
    }
}

void SND_UnlockChannel(u32 mask, u32 kind) {
    if (kind & 1) {
        sWeakLockChannel[0] &= ~mask;
    } else {
        sWeakLockChannel[1] &= ~mask;
    }
}

u32 SND_GetLockedChannel(u32 kind) {
    return kind & 1 ? sWeakLockChannel[0] : sWeakLockChannel[1];
}

void SND_InvalidateWave(u32 start, u32 end) {
    u8 i;
    for (i = 0; i < 16; i++) {
        struct SNDExChannel *chn = &SNDi_Work.channels[i];
        if (chn->flags.active && chn->type == SND_CHN_TYPE_PCM && start <= (u32)chn->waveDataPtr && (u32)chn->waveDataPtr <= end) {
            chn->flags.start = 0;
            SND_StopChannel(i, 0);
        }
    }
}

void SND_InitLfoParam(struct SNDLfoParam *param) {
    param->target = SND_LFO_PITCH;
    param->depth = 0;
    param->range = 1;
    param->speed = 16;
    param->delay = 0;
}

// The LFO phase (counter) is accessed as a volatile here.
#define LFO_PHASE(lfo) (*(volatile u16 *)&(lfo)->counter)

void SND_UpdateLfo(struct SNDLfo *lfo) {
    u32 counter;
    if (lfo->delayCounter < lfo->param.delay) {
        lfo->delayCounter++;
        return;
    }
    counter = LFO_PHASE(lfo);
    counter = (counter + ((u32)lfo->param.speed << 6)) >> 8;
    while (counter >= 128) {
        counter -= 128;
    }
    LFO_PHASE(lfo) += lfo->param.speed << 6;
    LFO_PHASE(lfo) &= 0xff;
    LFO_PHASE(lfo) |= counter << 8;
}

int SND_GetLfoValue(struct SNDLfo *lfo) {
    struct SNDLfo *p = lfo;
    const u8 *coeff = (const u8 *)lfo + 2;
    int value;
    if (!p->param.depth) {
        return 0;
    }
    if (p->delayCounter < p->param.delay) {
        return 0;
    }
    value = SND_SinIdx((u32)p->counter >> 8);
    return (int)coeff[1] * ((int)coeff[0] * value);
}

u16 CalcRelease(int release) {
    u16 result;
    if (release == 127) {
        result = 65535;
    } else if (release == 126) {
        result = 0x3c00;
    } else if (release < 50) {
        result = (u16)((u32)release * 2 + 1);
    } else {
        result = (u16)(0x1e00 / (s32)(126u - (u32)release));
    }
    return result;
}

void StartExChannel(struct SNDExChannel *chn, s32 length) {
    chn->envAttenuation = SND_ENV_ATTENUATION_START;
    chn->envStatus = SND_ENV_ATTACK;
    chn->length = length;
    chn->lfo.counter = 0;
    chn->lfo.delayCounter = 0;
    chn->flags.start = 1;
    chn->flags.active = 1;
}

#pragma dont_inline reset
