#pragma force_active on
#include "global.h"

#include "to90_vct_ssp_private.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Header {
    u32 magic;
    u8 kind, size, type, pad, id, target, notify, peer;
    u32 mask;
} Header;

typedef struct Event {
    s32 event;
    Session *session;
} Event;

extern Control _021D848C;
extern int vct_handle_audio(u32, char *, u32, u64);
extern int vct_handle_ssp(u32, u8 *, u32, u32);
int VCTi_HandleData(u32, Header *, u32, Event *);

static inline int IsAudioPacket(u8 kind) {
    return (kind & 0xf0) == 0x40;
}

static inline int IsSspPacket(u8 kind) {
    return kind == 0 || kind == 0xff;
}

#pragma push
#pragma opt_common_subs off

int VCTi_HandleData(u32 sender, Header *data, u32 size, Event *out) {
    u64 tick;
    if (!data) {
        return 0;
    }
    if (data->magic != 0x5f564354) {
        return 2;
    }
    if (!_021D848C.vct.mode || !_021D848C.active) {
        return 0;
    }
    out->event = 0;
    out->session = 0;
    tick = OS_GetTick();
    if (IsAudioPacket(data->kind)) {
        _021D848C.received++;
        vct_handle_audio(sender, (char *)data, size, tick);
        return 0;
    }
    if (IsSspPacket(data->kind)) {
        return vct_handle_ssp(sender, (u8 *)data, size, (u32)out) != 0;
    }
    return 0;
}

#pragma pop
