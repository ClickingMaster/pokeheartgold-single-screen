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

void vct_finish_ssp(void) {
    gVctSsp.core.used = 0;
    gVctSsp.core.free = 0;
}

void vct_check_timeout(void) {
    u64 now = OS_GetTick();
    if (__vct.mode != 2) {
        return;
    }
    if (__vct.state == 0) {
        return;
    }
    if (gVctSsp.core.tick == 0) {
        return;
    }
    if (now - gVctSsp.core.tick <= sTransceiverLimit) {
        return;
    }
    gVctSsp.core.tick = 0;
    gVctSsp.server.state = 0;
    gVctSsp.server.mode = 0;
    vct_send_notify(0);
    __vct.callback(gVctSsp.server.aid, 9, &gVctServerSession, __vct.context);
}

int vct_handle_ssp(u32 aid, u8 *b, u32 n, u32 arg) {
    if (n != 16) {
        return 0;
    }
    if (b[8] != aid || b[9] != __vct.aid) {
        return 0;
    }
    if (b[5] != 16) {
        return vct_response_reject(b, arg);
    }
    if (__vct.state == 0) {
        return vct_handle_ssp_client(b, arg);
    }
    return vct_handle_ssp_server(b, arg);
}

Session *vct_create_session_impl(u32 aid) {
    Session *s;
    if (aid >= 32) {
        return 0;
    }
    if (aid == __vct.aid) {
        return 0;
    }
    s = gVctSsp.core.free;
    if (!s) {
        return 0;
    }
    gVctSsp.core.free = s->next;
    vct_init_session(s, aid);
    s->next = gVctSsp.core.used;
    gVctSsp.core.used = s;
    return s;
}

int vct_request_impl(Session *s, int kind) {
    u8 b[16];
    int result = vct_build_request_data(s, kind, b, 16);
    if (result != 0) {
        return result;
    }
    return vct_send_ssp(s, b) ? 0 : -4;
}

int vct_response_impl(Session *s, int kind) {
    u8 b[16];
    int result = vct_build_response_data(s, kind, b, 16);
    if (result != 0) {
        return result;
    }
    return vct_send_ssp(s, b) ? 0 : -4;
}

void vct_add_notify_aid_list(Session *s, u8 *b) {
    if (b[10] != 1) {
        return;
    }
    b[11] = s->aid;
    *(u32 *)(b + 12) = gVctSsp.core.mask;
}

int vct_send_notify_impl(Session *s, u8 *b) {
    int count = 0;
    vct_add_notify_aid_list(s, b);
    for (u8 aid = 0; aid < 32; aid++) {
        u32 mask = aid ? 1 << aid : 1;
        if ((gVctSsp.core.mask & mask) && aid != __vct.aid) {
            b[9] = aid;
            if (DWC_SendReliable(aid, b, 16)) {
                count++;
            }
        }
    }
    if (b[10] == 1) {
        s->mask = gVctSsp.core.mask;
    } else if (b[10] == 0) {
        s->mask = 0;
    }
    if (count == 0) {
        return -4;
    }
    return 0;
}

void vct_send_notify(u32 kind) {
    u8 b[16];
    b[4] = 255;
    vct_build_ssp_header(&gVctServerSession, b, 4);
    b[10] = kind;
    vct_send_notify_impl(&gVctServerSession, b);
}
