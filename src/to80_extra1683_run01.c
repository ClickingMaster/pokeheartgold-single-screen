#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int SND_CalcChannelVolume(int);
extern void SND_SetChannelVolume(u32, int, int);

typedef struct {
    u8 pad[0x3c];
    int volume;
    u8 pad2[8];
    int count;
    u8 channels[16];
} S;

typedef struct {
    int x, volume;
} V;

extern V _021E0294[];
void NNS_SndStrmSetVolume(S *, int);

void NNS_SndStrmSetVolume(S *s, int volume) {
    int ch, i;
    s->volume = volume;
    for (i = 0; i < s->count; i++) {
        ch = s->channels[i];
        int v = SND_CalcChannelVolume(s->volume + _021E0294[ch].volume);
        SND_SetChannelVolume(1 << ch, v & 255, v >> 8);
    }
}
