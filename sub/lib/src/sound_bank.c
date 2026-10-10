#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/SND_bank_shared.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);
int SND_StartExChannelPcm(struct SNDExChannel *chn, const struct SNDWaveParam *wave, const void *data, int length);
int SND_StartExChannelPsg(struct SNDExChannel *chn, s32 duty, int length);
int SND_StartExChannelNoise(struct SNDExChannel *chn, int length);
void SND_SetExChannelAttack(struct SNDExChannel *chn, int attack);
void SND_SetExChannelDecay(struct SNDExChannel *chn, int decay);
void SND_SetExChannelSustain(struct SNDExChannel *chn, int sustain);
void SND_SetExChannelRelease(struct SNDExChannel *chn, int release);

int SND_ReadInstData(struct SNDBankData *bank, int index, int key, struct SNDInstData *out);
const void *SND_GetWaveDataAddress(const struct SNDWaveArc *waveArc, int index);
int SND_NoteOn(struct SNDExChannel *chn, int key, int velocity, int length, const struct SNDBankData *bank, const struct SNDInstData *inst);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

int SND_ReadInstData(struct SNDBankData *bank, int index, int key, struct SNDInstData *out) {
    int region;
    u8 high, low;
    u32 offset, word;
    const struct SNDDrumSet *drum;
    const struct SNDKeySplit *split;
    if (index < 0) {
        return 0;
    }
    SNDi_LockMutex();
    if ((u32)index >= bank->instCount) {
        SNDi_UnlockMutex();
        return 0;
    }
    word = bank->instOffsets[index];
    out->type = word;
    offset = word >> 8;
    switch (out->type) {
    case SND_INST_PCM:
    case SND_INST_PSG:
    case SND_INST_NOISE:
    case SND_INST_DIRECTPCM:
    case SND_INST_DUMMY:
        out->param = *(const struct SNDInstParam *)((const u8 *)bank + offset);
        break;
    case SND_INST_DRUM_TABLE: {
        drum = (const struct SNDDrumSet *)((const u8 *)bank + offset);
        low = drum->minKey;
        high = drum->maxKey;
        if (key < low || key > high) {
            SNDi_UnlockMutex();
            return 0;
        }
        *out = drum->instruments[key - low];
    } break;
    case SND_INST_KEY_SPLIT: {
        region = 0;
        split = (const struct SNDKeySplit *)((const u8 *)bank + offset);
        while (key > split->key[region]) {
            if (++region >= SND_INST_MAX_KEYSPLIT) {
                SNDi_UnlockMutex();
                return 0;
            }
        }
        *out = split->instruments[region];
    } break;
    case SND_INST_ILLEGAL:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    default:
        SNDi_UnlockMutex();
        return 0;
    }
    SNDi_UnlockMutex();
    return 1;
}

// Wave offsets below main memory are relative to the wave archive.
const void *SND_GetWaveDataAddress(const struct SNDWaveArc *waveArc, int index) {
    const u8 *address;
    SNDi_LockMutex();
    address = (const u8 *)waveArc->waveOffsets[index];
    if (address) {
        if ((u32)address < 0x02000000) {
            address = (const u8 *)waveArc + (u32)address;
        }
    } else {
        address = NULL;
    }
    SNDi_UnlockMutex();
    return address;
}

// Starts the instrument on a channel; inst->param.wave holds {wave index, wave archive} for PCM
// instruments, the wave address halves for direct PCM and the duty cycle for PSG.
int SND_NoteOn(struct SNDExChannel *chn, int key, int velocity, int length, const struct SNDBankData *bank, const struct SNDInstData *inst) {
    int release = inst->param.envRelease;
    int ok;
    const struct SNDWaveData *wave;
    const struct SNDWaveArc *waveArc;
    int waveIndex;
    if (release == 255) {
        length = -1;
        release = 0;
    }
    switch (inst->type) {
    case SND_INST_PCM:
    case SND_INST_DIRECTPCM:
        if (inst->type == SND_INST_PCM) {
            waveArc = bank->waveArcLinks[inst->param.wave[1]].waveArc;
            waveIndex = inst->param.wave[0];
            if (!waveArc) {
                wave = NULL;
            } else if ((u32)waveIndex >= waveArc->waveCount) {
                wave = NULL;
            } else {
                wave = SND_GetWaveDataAddress(waveArc, waveIndex);
            }
        } else {
            wave = (const struct SNDWaveData *)(((u32)inst->param.wave[1] << 16) | inst->param.wave[0]);
        }
        if (!wave) {
            ok = 0;
            break;
        }
        ok = SND_StartExChannelPcm(chn, &wave->param, wave->sampleData, length);
        break;
    case SND_INST_PSG:
        ok = SND_StartExChannelPsg(chn, inst->param.wave[0], length);
        break;
    case SND_INST_NOISE:
        ok = SND_StartExChannelNoise(chn, length);
        break;
    default:
        ok = 0;
        break;
    }
    if (!ok) {
        return 0;
    }
    chn->midiKey = key;
    chn->rootMidiKey = inst->param.rootKey;
    chn->velocity = velocity;
    SND_SetExChannelAttack(chn, inst->param.envAttack);
    SND_SetExChannelDecay(chn, inst->param.envDecay);
    SND_SetExChannelSustain(chn, inst->param.envSustain);
    SND_SetExChannelRelease(chn, release);
    chn->initPan = inst->param.pan - 64;
    return 1;
}

#pragma dont_inline reset
