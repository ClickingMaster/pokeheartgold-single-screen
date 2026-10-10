#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 type, speed, depth, range;
    u16 delay, count, phase;
} Lfo37005;
typedef struct Ch37005 Ch37005;

struct Ch37005 {
    u8 id, type, state;

    union {
        u8 flags;

        struct {
            u8 active : 1, started : 1, glide : 1, updates : 5;
        };
    };

    u8 vol, key;
    s16 tune;
    u8 key2, vel;
    s8 panbase, panadd;
    s16 volumeadd, pitchadd;
    s32 env;
    volatile s32 progress;
    s32 duration;
    u8 attack, sustain;
    u16 decay, release;
    u8 priority, pan;
    u16 volume, timer;
    Lfo37005 lfo;
    s16 pitch;
    u32 next;

    struct {
        u8 format, loop;
        u16 rate, timer, start;
        u32 length;
    } wave;

    u32 data;
    void (*cb)(Ch37005 *, int, void *);
    void *arg;
    u32 last;
};

extern Ch37005 SNDi_Work[16];
extern const s16 SNDi_DecibelTable[];
extern int SND_IsChannelActive(int);
extern int SND_UpdateExChannelEnvelope(Ch37005 *, int);
extern int SND_GetLfoValue(Lfo37005 *);
extern void SND_UpdateLfo(Lfo37005 *);
extern u16 SND_CalcChannelVolume(int);
extern u16 SND_CalcTimer(int, int);
void SND_ExChannelMain(int update);

void SND_ExChannelMain(int update) {
    Ch37005 *p;
    int i;
    int volume, pitch, pan;
    s64 lfo;
    int minimum = -32768;
    int run = update;
    for (i = 0; i < 16; i++) {
        int slide;
        u16 timer, level;
        p = &SNDi_Work[i];
        volume = 0;
        pitch = 0;
        pan = 0;
        if (!p->active) {
            continue;
        }
        if (p->started) {
            p->updates |= 1;
            p->started = 0;
        } else if (!SND_IsChannelActive(i)) {
            if (!p->cb) {
                p->priority = 0;
            } else {
                p->cb(p, 1, p->arg);
            }
            p->volume = 0;
            p->active = 0;
            continue;
        }
        volume += SNDi_DecibelTable[p->vel];
        pitch += (p->key2 - p->key) * 64;
        volume += SND_UpdateExChannelEnvelope(p, run);
        if (!p->pitch) {
            slide = 0;
        } else {
            int progress = p->progress;
            if (progress >= p->duration) {
                slide = 0;
            } else {
                slide = (s32)((s64)p->pitch * (s32)((u32)p->duration - (u32)progress) / p->duration);
                if (run && p->glide) {
                    p->progress = (s32)((u32)p->progress + 1);
                }
            }
        }
        pitch += slide;
        volume += p->volumeadd;
        volume += p->tune;
        pitch += p->pitchadd;
        lfo = SND_GetLfoValue(&p->lfo);
        if (lfo) {
            switch (p->lfo.type) {
            case 1:
                lfo *= 60;
                break;
            case 0:
                lfo = (s64)((u64)lfo << 6);
                break;
            case 2:
                lfo = (s64)((u64)lfo << 6);
                break;
            }
            lfo >>= 14;
        }
        if (run) {
            SND_UpdateLfo(&p->lfo);
        }
        switch (p->lfo.type) {
        case 1:
            if (volume > minimum) {
                volume += (s32)lfo;
            }
            break;
        case 0:
            pitch += (s32)lfo;
            break;
        case 2:
            pan += (s32)lfo;
            break;
        }
        pan += p->panbase;
        if (p->vol != 127) {
            pan = (pan * p->vol + 64) >> 7;
        }
        pan += p->panadd;
        if (p->state == 3 && volume <= -723) {
            p->updates = 2;
            if (!p->cb) {
                p->priority = 0;
            } else {
                p->cb(p, 1, p->arg);
            }
            p->volume = 0;
            p->active = 0;
            continue;
        }
        level = SND_CalcChannelVolume(volume);
        timer = SND_CalcTimer(p->wave.timer, pitch);
        if (p->type == 1) {
            timer &= 0xfffc;
        }
        pan += 64;
        if (pan < 0) {
            pan = 0;
        } else if (pan > 127) {
            pan = 127;
        }
        if (level != p->volume) {
            p->volume = level;
            p->updates |= 8;
        }
        if (timer != p->timer) {
            p->timer = timer;
            p->updates |= 4;
        }
        if (pan != p->pan) {
            p->pan = pan;
            p->updates |= 16;
        }
    }
}
