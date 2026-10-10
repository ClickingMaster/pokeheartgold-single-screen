#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

struct Stream {
    u8 pad[32];
    int format;

    struct {
        signed int enabled : 1;
        signed int running : 1;
        signed int rest : 30;
    } flags;

    u32 bytes;
    int blocks;
    void (*callback)(int, int, void **, u32, int, void *);
    void *user;
    int current;
    u32 pad3c;
    int alarm;
    u32 channels;
    int count;
    u8 ids[16];
};

struct Buffer {
    u8 *pointer;
    u32 pad;
};
extern struct Buffer _021E0294[];
extern void *_021E0254[];
extern u8 _021E0314[];
extern u8 _021E035C[];
extern void SND_StopTimer(u32, u32, u32, u32);
extern void SND_StartTimer(u32, u32, u32, u32);
extern u32 SND_GetCurrentCommandTag(void);
extern BOOL SND_FlushCommand(u32);
extern void SND_WaitForCommandProc(u32);
extern void NNSi_SndCaptureStop(void);
extern void NNSi_SndFaderSet(void *, int, int);
void StrmCallback(struct Stream *, int);
void BeginSleep__Strm(struct Stream *);
void EndSleep__Strm(struct Stream *);
void NNS_SndCaptureStopReverb(int);

void StrmCallback(struct Stream *stream, int reason) {
    u32 chunk = stream->bytes / stream->blocks;
    u32 offset = chunk * stream->current;
    int i;
    for (i = 0; i < stream->count; i++) {
        _021E0254[i] = _021E0294[stream->ids[i]].pointer + offset;
    }
    stream->callback(reason, stream->count, _021E0254, chunk, stream->format, stream->user);
    stream->current++;
    if (stream->current >= stream->blocks) {
        stream->current = 0;
    }
}

void BeginSleep__Strm(struct Stream *stream) {
    u32 tag;
    if (!stream->flags.running) {
        return;
    }
    SND_StopTimer(stream->channels, 0, 1 << stream->alarm, 0);
    tag = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(tag);
}

void EndSleep__Strm(struct Stream *stream) {
    if (!stream->flags.running) {
        return;
    }
    while (stream->current != 0) {
        OSIntrMode mode = OS_DisableInterrupts();
        StrmCallback(stream, 1);
        OS_RestoreInterrupts(mode);
    }
    SND_StartTimer(stream->channels, 0, 1 << stream->alarm, 0);
}

void NNS_SndCaptureStopReverb(int frames) {
    u8 *state = _021E035C;
    if (!*(int *)(_021E0314 + 0x48)) {
        return;
    }
    if (*(int *)(state + 4)) {
        return;
    }
    if (frames == 0) {
        NNSi_SndCaptureStop();
    } else {
        NNSi_SndFaderSet(state + 0x3c, 0, frames);
        *(int *)(state + 0x4c) = 1;
    }
}
