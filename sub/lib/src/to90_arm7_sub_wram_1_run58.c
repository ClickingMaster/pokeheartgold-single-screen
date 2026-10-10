#include "canonical-api.h"
#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35227Track {
    u8 active : 1;
    u8 flag1 : 1;
    u8 mute : 1;
    u8 other : 5;
    u8 mod;
    u8 b2, b3;
    u8 volume, volume2;
    s8 bend;
    u8 range;
    s8 pan, pan2;
    s16 extvol, pitch;
    u8 rest[10];
    u16 mod1, mod2, mod3;
    u8 rest2[30];
    struct W35227Channel *channels;
} W35227Track;

typedef struct W35227Player {
    u8 active : 1;
    u8 flags : 7;
    u8 number, b2, b3, b4, volume;
    s16 extvol;
    u8 tracks[16];
} W35227Player;

typedef struct W35227Channel {
    u8 b0, b1, type, b3, mod, key;
    s16 extvol;
    u8 note, velocity;
    s8 pan, pan2;
    s16 volume, pitch;
    u8 rest[18];
    u8 release;
    u8 rest2[5];
    u16 mod1, mod2, mod3;
    u8 rest3[6];
    void *callback;
    u8 rest4[24];
    struct W35227Channel *next;
} W35227Channel;

extern u8 SNDi_Work[];

typedef struct W35227Inst {
    u8 type, pad;
    u16 wave, arc;
    u8 key, attack, decay, sustain, release, pan;
} W35227Inst;

typedef struct W35227Bank {
    u8 pad[24];

    struct {
        u8 *data;
        u32 other;
    } arcs[4];
} W35227Bank;

extern void *SND_GetWaveDataAddress(void *, int);
extern int SND_StartExChannelPcm(W35227Channel *, void *, void *, int);
extern int SND_StartExChannelPsg(W35227Channel *, int, int);
extern int SND_StartExChannelNoise(W35227Channel *, int);
extern void SND_SetExChannelDecay(W35227Channel *, int);
extern void SND_SetExChannelSustain(W35227Channel *, int);
extern void SND_SetExChannelRelease(W35227Channel *, int);
int SND_NoteOn(W35227Channel *ch, int note, int velocity, int length, W35227Bank *bank, W35227Inst *inst);

int SND_NoteOn(W35227Channel *ch, int note, int velocity, int length, W35227Bank *bank, W35227Inst *inst) {
    int rel = inst->release;
    int ok;
    void *wave;
    u8 *arc;
    int waveIndex;
    if (rel == 255) {
        length = -1;
        rel = 0;
    }
    switch (inst->type) {
    case 1:
    case 4:
        if (inst->type == 1) {
            arc = bank->arcs[inst->arc].data;
            waveIndex = inst->wave;
            if (!arc) {
                wave = 0;
            } else if ((u32)waveIndex >= *(u32 *)(arc + 56)) {
                wave = 0;
            } else {
                wave = SND_GetWaveDataAddress(arc, waveIndex);
            }
        } else {
            wave = (void *)(((u32)inst->arc << 16) | inst->wave);
        }
        if (!wave) {
            ok = 0;
            break;
        }
        ok = SND_StartExChannelPcm(ch, wave, (u8 *)wave + 12, length);
        break;
    case 2:
        ok = SND_StartExChannelPsg(ch, inst->wave, length);
        break;
    case 3:
        ok = SND_StartExChannelNoise(ch, length);
        break;
    default:
        ok = 0;
        break;
    }
    if (!ok) {
        return 0;
    }
    ch->note = note;
    ch->key = inst->key;
    ch->velocity = velocity;
    SND_SetExChannelAttack(ch, inst->attack);
    SND_SetExChannelDecay(ch, inst->decay);
    SND_SetExChannelSustain(ch, inst->sustain);
    SND_SetExChannelRelease(ch, rel);
    ch->pan = inst->pan - 64;
    return 1;
}
