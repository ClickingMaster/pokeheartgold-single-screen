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

Session *VCT_CreateSession(u32 aid) {
    if (__vct.mode == 0 || __vct.mode == 2) {
        return 0;
    }
    return vct_create_session_impl(aid);
}

int VCT_DeleteSession(Session *s) {
    Session *prev = 0;
    Session *cur = gVctSsp.core.used;
    if (!s) {
        return 0;
    }
    if (__vct.mode == 2) {
        if (s == &gVctServerSession) {
            gVctSsp.server.state = 0;
        }
        return 1;
    }
    while (cur) {
        if (cur == s) {
            s->state = 0;
            if (prev) {
                prev->next = s->next;
            } else {
                if (cur->next) {
                    gVctSsp.core.used = cur->next;
                } else {
                    gVctSsp.core.used = 0;
                }
            }
            s->next = gVctSsp.core.free;
            gVctSsp.core.free = s;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

int vct_build_request_data(Session *s, int kind, u8 *b, u32 n) {
    if (!s) {
        return -1;
    }
    if (n < 16) {
        return -1;
    }
    if (__vct.state == 1) {
        return -3;
    }
    if (kind < 0 || kind >= 5) {
        return -3;
    }
    b[4] = 255;
    vct_build_ssp_header(s, b, (u8)kind);
    if (!vct_check_send_state(s, b)) {
        return -3;
    }
    if (__vct.mode == 1 && kind == 0) {
        if (!vct_check_multisession(s)) {
            return -3;
        }
    }
    return 0;
}

int VCT_Request(Session *s, int kind) {
    if (__vct.mode == 2) {
        return -3;
    }
    return vct_request_impl(s, kind);
}

int vct_build_response_data(Session *s, int kind, u8 *b, u32 n) {
    if (!s) {
        return -1;
    }
    if (n < 16) {
        return -1;
    }
    if (kind < 0 || kind >= 6) {
        return -3;
    }
    b[4] = 0;
    vct_build_ssp_header(s, b, (u8)kind);
    if (kind == 0) {
        if (__vct.mode == 1 && s->mode == 4) {
            if (!vct_check_multisession(s)) {
                return -3;
            }
        }
        s->mask = s->aid ? 1 << s->aid : 1;
    }
    if (!vct_check_send_state(s, b)) {
        return -3;
    }
    return 0;
}

int VCT_Response(Session *s, int kind) {
    if (__vct.mode == 2) {
        return -3;
    }
    return vct_response_impl(s, kind);
}

int VCT_AddConferenceClient(u8 aid) {
    Session *s;
    if (__vct.mode != 3) {
        return -2;
    }
    if (gVctSsp.core.count == 3) {
        return -6;
    }
    if (aid == __vct.aid) {
        return -1;
    }
    if (vct_find_session(aid)) {
        return 0;
    }
    s = VCT_CreateSession(aid);
    if (!s) {
        return -1;
    }
    s->aid = aid;
    s->other = aid;
    s->mode = 2;
    s->mask = 1 << aid;
    __vct.callback(aid, 7, s, __vct.context);
    gVctSsp.core.count++;
    return 0;
}
