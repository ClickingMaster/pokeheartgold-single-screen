#pragma force_active on
#include "global.h"

#include "to90_vct_ssp_private.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
#pragma force_active on

typedef struct Header {
    u32 magic;
    u8 kind, size, type, pad, id, target, notify, peer;
    u32 mask;
} Header;

typedef struct Event {
    s32 event;
    Session *session;
} Event;

typedef struct Packet {
    struct Packet *next;
    u32 flags, other;
    u8 data[0x46c];
} Packet;

extern u8 sRequestEvent[];
extern u8 sResponseEvent[];
extern s8 sRecvState[][11];
extern s8 sTransRecvState[][11];
extern s8 sSendState[][11];
extern s8 sTransSendState[][11];

extern Session _021D6FCC;
extern Session _021D6FB4;
extern Packet *_021D6FE4[];
extern u32 E2scales[];

typedef struct VADState {
    s32 level, index, count;
} VADState;

extern VADState _021D6FEC;
extern s32 sVADInfo[];
extern s32 _021D7008[];
extern s32 sTap[];
extern s32 sH[];
extern s16 sImpulseResponse[];
extern s16 sDelayLine[];
extern u32 sSpkBuffer[];

extern s32 vct_send_notify_impl(Session *, Header *);
extern s32 DWC_SendReliable(u8, const void *, u32);
extern void vct_response_impl(Session *, s32);
extern void vct_finish_audio(void);
extern void vct_finish_ssp(void);
extern void vct_check_timeout(void);
extern s32 vct_flush_send_buffer(void);
extern s32 vct_decode_audio_buffer(void);
extern void DoFIRFilter(void *, void *, u32, u32);
extern s32 vct_handle_ssp(u32, Header *, u32, Event *);
extern s32 vct_handle_audio(u32, Header *, u32, u64);
extern void vct_init_session(Session *, u8);
extern s32 vct_send_ssp(Session *, Header *);
extern s32 vct_dispatch_session(Header *, Session *);
extern void vct_build_ssp_header(Session *, Header *, u8);
extern s32 vct_check_recv_state(Session *, Header *);
extern s32 vct_check_multisession(Session *);
extern s32 vct_check_send_state(Session *, Header *);
extern Session *vct_find_session(u8);
extern void vct_cleanup_packet_buffer(void);
extern Packet *vct_alloc_packet_buffer(void);
extern void vct_free_packet_buffer(Packet *);
extern void VCT_ResetVAD(void);
extern void VCT_GetVADInfo(void *);
extern void vct_init_vad(void);
extern void InitFIRFilter(void);
extern void vct_set_speaker_samples(void *, u32);
extern void vct_process_fir(void *, u32, u32);
extern void vct_mix_audio(s16 *, s16 *, s16 *, u32);
extern void VCT_Cleanup(void);
extern void VCT_Main(void);
extern s32 VCT_HandleData(u32, Header *, u32);
extern s32 VCTi_HandleData(u32, Header *, u32, Event *);
extern s32 vct_handle_notify(Header *, Event *);
extern s32 vct_response_reject(Header *, Event *);
extern s32 vct_handle_ssp_client(Header *, Event *);
extern Session *vct_create_session_impl(u8);
extern void VCT_DeleteSession(Session *);
extern s32 vct_init_packet_buffer(void *, u32);
extern char _version_Abiosso_libVCT[];
extern void OSi_ReferSymbol(const void *);
extern s32 vct_init_ssp(InitArgs *);
extern s32 vct_init_audio(void);
extern s32 VCT_Init(InitArgs *);
extern void vct_send_notify(s32);
extern s32 vct_handle_ssp_server(Header *, Event *);

s32 VCT_Init(InitArgs *args) {
    OSi_ReferSymbol(_version_Abiosso_libVCT);
    if (!args) {
        return 0;
    }
    if (_021D848C.active) {
        return 1;
    }
    switch (args->mode) {
    case 1:
    case 2:
    case 3:
        break;
    default:
        return 0;
    }
    if (!args->callback) {
        return 0;
    }
    if (!args->buffer || !args->size) {
        return 0;
    }
    if ((s32)(u32)args->buffer % 32) {
        return 0;
    }
    MI_CpuFill8(&__vct, 0, 24);
    _021D848C.vct.callback = args->callback;
    _021D848C.vct.context = args->arg;
    _021D848C.vct.mode = args->mode;
    _021D848C.vct.state = 0;
    if (args->id >= 32) {
        return 0;
    }
    _021D848C.vct.aid = args->id;
    if (!vct_init_packet_buffer((void *)args->buffer, args->size)) {
        vct_cleanup_packet_buffer();
        return 0;
    }
    if (!vct_init_ssp(args)) {
        vct_cleanup_packet_buffer();
        return 0;
    }
    if (!vct_init_audio()) {
        vct_finish_ssp();
        vct_cleanup_packet_buffer();
        return 0;
    }
    _021D848C.active = 1;
    return 1;
}

void VCT_Cleanup(void) {
    vct_finish_audio();
    vct_finish_ssp();
    vct_cleanup_packet_buffer();
    _021D848C.vct.mode = 0;
    _021D848C.active = 0;
}

void VCT_Main(void) {
    if (!_021D848C.active) {
        return;
    }
    if ((++_021D848C.frame & 15) == 0) {
        vct_check_timeout();
    }
    if (!vct_flush_send_buffer()) {
        return;
    }
    vct_decode_audio_buffer();
}

s32 VCT_HandleData(u32 id, Header *header, u32 size) {
    Event event;
    switch (VCTi_HandleData(id, header, size, &event)) {
    case 0:
        break;
    case 1:
        _021D848C.vct.callback(id, event.event, event.session, _021D848C.vct.context);
        return 1;
    case 2:
        return 0;
    }
    return 1;
}
