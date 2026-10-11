#pragma force_active on
#include "global.h"

#include "vct/vct_session.h"

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

extern Session gVctServerSession;
extern Session gVctRejectSession;
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

void vct_cleanup_packet_buffer(void) {
    _021D6FE4[0] = 0;
    _021D6FE4[1] = 0;
}

Packet *vct_alloc_packet_buffer(void) {
    Packet *packet = 0;
    OSIntrMode irq = OS_DisableInterrupts();
    if (_021D6FE4[0]) {
        packet = _021D6FE4[0];
        _021D6FE4[0] = packet->next;
    }
    OS_RestoreInterrupts(irq);
    return packet;
}

void vct_free_packet_buffer(Packet *packet) {
    OSIntrMode irq = OS_DisableInterrupts();
    packet->next = _021D6FE4[0];
    _021D6FE4[0] = packet;
    packet->flags = 0;
    packet->other = 0;
    OS_RestoreInterrupts(irq);
}

void VCT_ResetVAD(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        E2scales[i] = 0x1000000;
    }
    ((volatile VADState *)&_021D6FEC)->index = 0;
    ((volatile VADState *)&_021D6FEC)->level = 0;
    ((volatile s32 *)sVADInfo)[0] = 0;
    ((volatile s32 *)sVADInfo)[1] = 0;
    ((volatile VADState *)&_021D6FEC)->count = 0;
}

void VCT_GetVADInfo(void *out) {
    if (out) {
        MI_CpuCopy8(sVADInfo, out, 16);
    }
}
