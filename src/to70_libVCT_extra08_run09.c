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

void vct_flush_audio_queue(u32 ch) {
    OSIntrMode irq = OS_DisableInterrupts();
    while (_021D6F6C[ch]) {
        Packet *p = _021D6F6C[ch];
        Packet *q = p->next;
        _021D6F6C[ch] = q;
        if (q) {
            q->prev = 0;
        }
        vct_free_packet_buffer(p);
    }
    _021D6F84[ch] = 0;
    _021D6F78[ch] = 0;
    OS_RestoreInterrupts(irq);
}

void vct_flush_all_audio_queue(void) {
    OSIntrMode irq = OS_DisableInterrupts();
    for (int ch = 0; ch < 3; ch++) {
        while (_021D6F6C[ch]) {
            Packet *p = _021D6F6C[ch];
            Packet *q = p->next;
            _021D6F6C[ch] = q;
            if (q) {
                q->prev = 0;
            }
            vct_free_packet_buffer(p);
        }
        _021D6F84[ch] = 0;
        _021D6F78[ch] = 0;
    }
    OS_RestoreInterrupts(irq);
}

int vct_insert_audio_queue(Packet *p, u32 ch) {
    OSIntrMode irq;
    if (!p) {
        return -1;
    }
    if (ch >= 3) {
        return -1;
    }
    irq = OS_DisableInterrupts();
    if (!_021D6F6C[ch]) {
        _021D6F6C[ch] = p;
        p->prev = 0;
        _021D6F6C[ch]->next = 0;
        _021D6F84[ch] = _021D6F6C[ch];
    } else {
        Packet *q = _021D6F84[ch];
        while (q) {
            if (q->seq < p->seq) {
                p->prev = q;
                p->next = q->next;
                q->next = p;
                if (p->next) {
                    p->next->prev = p;
                }
                if (q == _021D6F84[ch]) {
                    _021D6F84[ch] = p;
                }
                goto inserted;
            }
            q = q->prev;
        }
        {
            p->prev = 0;
            p->next = _021D6F6C[ch];
            _021D6F6C[ch]->prev = p;
            _021D6F6C[ch] = p;
        }
    }
inserted:
    _021D6F78[ch]++;
    OS_RestoreInterrupts(irq);
    return _021D6F78[ch];
}

Packet *vct_top_audio_queue(u32 ch) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (_021D6F6C[ch]) {
        OS_RestoreInterrupts(irq);
        return _021D6F6C[ch];
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

u32 vct_pop_audio_queue(u32 ch) {
    OSIntrMode irq = OS_DisableInterrupts();
    if (_021D6F6C[ch]) {
        Packet *q = _021D6F6C[ch]->next;
        _021D6F6C[ch] = q;
        _021D6F78[ch]--;
        if (q) {
            q->prev = 0;
        } else {
            _021D6F84[ch] = 0;
        }
    }
    OS_RestoreInterrupts(irq);
    return _021D6F78[ch];
}

u32 vct_count_audio_queue(u32 ch) {
    return _021D6F78[ch];
}

void vct_init_stream(u32 *s, u32 ch, u32 mode) {
    MI_CpuFill8(s, 0, 0x28);
    s[1] = 0x134df;
    s[10] = 0;
    s[11] = 0;
    s[13] = ch;
    s[14] = mode;
    s[31] = 0;
    s[32] = 0;
    s[33] = 0;
    s[34] = 0;
    s[35] = 0x134df;
    s[36] = 0;
    s[19] = 0;
    s[17] = 0;
    s[18] = 0;
    s[27] = 0;
    s[28] = 0;
    s[20] = 0;
    s[21] = 0;
    s[22] = 0;
    s[23] = 1;
    s[15] = 0;
    s[16] = 0;
    s[12] = 0;
}

void vct_reset_stream(u32 *s) {
    if (s[15]) {
        vct_free_packet_buffer((void *)s[15]);
        s[15] = 0;
    }
    s[14] = -1;
}

void vct_save_previous_buffer(u32 *s, Packet *p) {
    if (s[15]) {
        vct_free_packet_buffer((void *)s[15]);
    }
    s[15] = (u32)p;
    vct_pop_audio_queue(s[13]);
}

void vct_init_sequence(u32 *s, u32 seq) {
    *(u16 *)((u8 *)s + 0x68) = seq;
    s[25] = 0x10001;
    s[24] = 0;
    s[23] = 0;
}

int vct_update_sequence(u32 *s, Packet *p, u32 seq) {
    u16 max = *(u16 *)((u8 *)s + 0x68);
    u16 delta = seq - max;
    if (delta < 3000) {
        if (seq < max) {
            s[24] += 0x10000;
        }
        *(u16 *)((u8 *)s + 0x68) = seq;
    } else if (delta <= 0xff9c) {
        if (seq == s[25]) {
            vct_init_sequence(s, seq);
            s[21] = (u16)(seq - 1);
        } else {
            s[25] = (u16)(seq + 1);
            return 0;
        }
    } else {
        u32 extended = seq + s[24];
        if (extended > s[21] + 0xff9c) {
            p->seq = extended - 0x10000;
            return 1;
        }
    }
    p->seq = seq + s[24];
    return 1;
}
