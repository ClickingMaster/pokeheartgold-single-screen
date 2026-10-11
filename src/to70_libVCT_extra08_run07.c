#pragma force_active on
#include "global.h"

#include "vct/vct_session.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Packet {
    u32 a;
    struct Packet *prev, *next;
    u8 pad[5];
    u8 channel;
    u8 pad2[6];
    u32 seq;
} Packet;

extern Packet *_021D6F58[2];
extern u32 _021D6F60[3];
extern u32 _021D6F78[3];
extern Packet *_021D6F6C[3];
extern Packet *_021D6F84[3];
extern Session gVctServerSession;
extern Session gVctRejectSession;
extern u64 sTransceiverLimit;
extern int ulaw_segment[8];
extern void vct_free_packet_buffer(void *);
extern Packet *vct_get_packet_from_decoder(u32);
extern u32 vct_remove_packet_from_decoder(Packet *);
extern u32 vct_pop_audio_queue(u32);
extern void vct_init_sequence(u32 *, u32);
extern Session *vct_create_session_impl(u32);
extern void vct_init_session(Session *, u32);
extern int vct_request_impl(Session *, int);
extern int vct_response_impl(Session *, int);
extern int vct_check_send_state(Session *, u8 *);
extern int vct_check_multisession(Session *);
extern int vct_send_ssp(Session *, u8 *);
extern void vct_build_ssp_header(Session *, u8 *, u32);
extern Session *vct_find_session(u32);
extern int vct_response_reject(u8 *, u32);
extern int vct_handle_ssp_client(u8 *, u32);
extern int vct_handle_ssp_server(u8 *, u32);
extern int DWC_SendReliable(u32, void *, u32);
extern void vct_send_notify(u32);
extern int vct_send_notify_impl(Session *, u8 *);

void vct_flush_decoder(u32 ch);
void vct_flush_all_decoder(void);
u32 vct_insert_decoder(Packet *p);
Packet *vct_top_decoder(void);
u32 vct_count_decoder_queue(u32 ch);
void vct_encode_g711_ulaw(u8 *out, s16 *in, u32 count);
void vct_decode_g711_ulaw(u8 *in, s16 *out, u32 n);
void vct_encode_8bit_raw(u8 *out, s16 *in, u32 n);
void vct_decode_8bit_raw(s8 *in, s16 *out, u32 n);
void vct_init_audio_queue(u32 ch);
void vct_flush_audio_queue(u32 ch);
void vct_flush_all_audio_queue(void);
int vct_insert_audio_queue(Packet *p, u32 ch);
Packet *vct_top_audio_queue(u32 ch);
u32 vct_count_audio_queue(u32 ch);
void vct_init_stream(u32 *s, u32 ch, u32 mode);
void vct_reset_stream(u32 *s);
void vct_save_previous_buffer(u32 *s, Packet *p);
int vct_update_sequence(u32 *s, Packet *p, u32 seq);
Session *VCT_CreateSession(u32 aid);
int VCT_DeleteSession(Session *s);
int vct_build_request_data(Session *s, int kind, u8 *b, u32 n);
int VCT_Request(Session *s, int kind);
int vct_build_response_data(Session *s, int kind, u8 *b, u32 n);
int VCT_Response(Session *s, int kind);
int VCT_AddConferenceClient(u8 aid);
void vct_finish_ssp(void);
void vct_check_timeout(void);
int vct_handle_ssp(u32 aid, u8 *b, u32 n, u32 arg);
void vct_add_notify_aid_list(Session *s, u8 *b);

void vct_flush_decoder(u32 ch) {
    OSIntrMode irq = OS_DisableInterrupts();
    Packet *p;
    while ((p = vct_get_packet_from_decoder(ch)) != 0) {
        vct_remove_packet_from_decoder(p);
        vct_free_packet_buffer(p);
    }
    _021D6F60[ch] = 0;
    OS_RestoreInterrupts(irq);
}

void vct_flush_all_decoder(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    Packet *p = _021D6F58[0];
    while (p) {
        Packet *q = p;
        p = p->next;
        vct_free_packet_buffer(q);
    }
    _021D6F58[0] = 0;
    _021D6F58[1] = 0;
    for (int i = 0; i < 3; i++) {
        _021D6F60[i] = 0;
    }
    OS_RestoreInterrupts(irq);
}

u32 vct_insert_decoder(Packet *p) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (!_021D6F58[0]) {
        _021D6F58[0] = p;
        p->prev = 0;
        _021D6F58[0]->next = 0;
        _021D6F58[1] = _021D6F58[0];
    } else {
        p->prev = _021D6F58[1];
        p->next = 0;
        _021D6F58[1]->next = p;
        _021D6F58[1] = p;
    }
    _021D6F60[p->channel]++;
    OS_RestoreInterrupts(irq);
    return _021D6F60[p->channel];
}

Packet *vct_top_decoder(void) {
    return _021D6F58[0];
}

u32 vct_count_decoder_queue(u32 ch) {
    return _021D6F60[ch];
}

Packet *vct_get_packet_from_decoder(u32 ch) {
    Packet *p = _021D6F58[0];
    OSIntrMode irq = OS_DisableInterrupts();
    while (p) {
        if (p->channel == ch) {
            OS_RestoreInterrupts(irq);
            return p;
        }
        p = p->next;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

u32 vct_remove_packet_from_decoder(Packet *p) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (p->prev) {
        p->prev->next = p->next;
    } else {
        Packet *q = p->next;
        _021D6F58[0] = q;
        if (q) {
            q->prev = 0;
        }
    }
    if (p->next) {
        p->next->prev = p->prev;
    } else {
        Packet *q = p->prev;
        _021D6F58[1] = q;
        if (q) {
            q->next = 0;
        }
    }
    _021D6F60[p->channel]--;
    OS_RestoreInterrupts(irq);
    return _021D6F60[p->channel];
}
