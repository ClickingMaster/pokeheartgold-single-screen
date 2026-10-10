// libVCT audio translation unit (vct_init_audio .. vct_decode_audio_buffer) as one object.
// Owns the TU's pooled .bss 0x021D6040..0x021D6F58 (MWCC -ipa file pools it, sorted by size):
// _021D6040 (u16 sequence) 0, scalars 4..0x2B (static, slot order from definition order),
// sAdpcmState 0x10, sAudioSession 0x2C, sStream 0x38, _021D6234 0x1F4, sSendInfo 0x640.
#include "global.h"

#include "to90_vct_ssp_private.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct AdpcmState {
    s16 value;
    u8 index, pad;
} AdpcmState;

typedef struct SendInfo {
    u8 buffer[0x8c0];
    u32 read, write, buffer2;
    volatile u32 mask;
    int balanceIndex, balance;
} SendInfo;

typedef struct {
    u8 payload, header;
} Codec;

typedef struct Packet {
    char pad[12];
    int codec;
    u8 owner, sender;
    u16 gap;
    u32 size, sequence;
    char samples[0x440];
    int flagged, timestamp, receiveLo, receiveHi;
    u32 readyLo, readyHi;
    void *data;
} Packet;

typedef struct RPacket {
    char gap[16];
    u8 owner;
    u8 sender;
    u16 pad;
    u32 size;
    u32 sequence;
    char samples[0x440];
    int flagged;
    u32 timestamp;
    u64 receive;
    u64 ready;
    void *data;
} RPacket;

typedef struct Stream {
    u32 jitter, p4, left, pC, p10, lost, p18, p1c, retrans, codec, oldDelay, delay, seq, owner, session;
    void *previous;
    u32 g40, g44, g48, silence, g50, g54, last, g5c, g60, g64, g68;
    int vad, skip;
    u32 rest[8];
} Stream;

u16 _021D6040;
static u32 sAudioVad;
static u32 sAudioReset;
static u32 sAudioEcho;
static u32 sAudioPacketSize;
static u32 sAudioMask;
AdpcmState sAdpcmState;
static u32 sAudioFlag;
static s32 sAudioCodec;
static u32 sAudioSessions;
static u32 sAudioSize;
Session *sAudioSession[3];
Stream sStream[3];
u32 _021D6234[0x44c / 4];
SendInfo sSendInfo;
extern Vct __vct;
extern u8 sCodecInfo[];
extern u8 sTransceiverBalance[];
extern s32 sEnableVAD;
extern void vct_init_vad(void);
extern void vct_init_audio_queue(u32);
extern void vct_init_stream(Stream *, u32, u32);
extern void vct_init_decoder(void);
extern void InitFIRFilter(void);
extern void VCT_ResetVAD(void);
extern void vct_reset_stream(Stream *);
extern void vct_flush_audio_queue(u32);
extern void vct_flush_decoder(u32);
extern void vct_flush_all_audio_queue(void);
extern void vct_flush_all_decoder(void);
extern int vct_count_audio_queue(u32);
extern int vct_save_previous_buffer(Stream *, void *);
extern void *vct_top_audio_queue(u32);
extern void vct_free_packet_buffer(void *);
extern void vct_mix_audio(void *, void *, void *, u32);
extern void vct_set_speaker_samples(void *, u32);
extern u64 _ll_udiv(u64, u64);
u32 HPFFilter(void *, u32);
void vct_process_fir(void *, u32, u32);
int VCTi_GetVADStatus(u32, u32, int);
void vct_encode_adpcm(void *, void *, u32, void *, int);
void vct_encode_g711_ulaw(void *, void *, u32);
void vct_encode_8bit_raw(void *, void *, u32);
int MATH_CountPopulation(u32);
extern Packet *vct_top_decoder(void);
extern Packet *vct_get_packet_from_decoder(int);
extern int vct_count_decoder_queue(int);
extern int vct_insert_audio_queue(Packet *, int);
extern void vct_insert_decoder(Packet *);
extern void vct_decode_adpcm(void *, void *, u32, int);
extern void vct_decode_g711_ulaw(void *, void *, u32);
extern void vct_decode_8bit_raw(void *, void *, u32);
extern void vct_remove_packet_from_decoder(Packet *);
extern Packet *vct_alloc_packet_buffer(void);
extern int vct_calc_jitter_and_skew(Stream *, Packet *);
s32 vct_init_audio(void);
void vct_finish_audio(void);
int VCT_StartStreaming(Session *);
void VCT_StopStreaming(Session *);
int VCT_SendAudio(void *, u32);
int VCTi_ReceiveAudioData(void *dst, u32 size, u32 ch, u32 *mask);
int VCT_ReceiveAudio(void *, u32, u32 *);
s32 VCT_SetCodec(s32);
void VCT_EnableVAD(s32);
void VCT_EnableEchoCancel(s32);
void *vct_prepare_send_buffer(void);
int vct_handle_audio(u32, char *, u32, u64);
int vct_decode_audio_buffer(void);
#define F(p, n, t) (*(t *)((char *)(p) + (n)))

static inline void InitializeAudioStreams(void) {
    u32 i;
    for (i = 0; i < 3; i++) {
        sAudioSession[i] = 0;
        vct_init_audio_queue(i);
        vct_init_stream(&sStream[i], i, (u32)-1);
    }
}

#pragma push
#pragma optimization_level 2

s32 vct_init_audio(void) {
    _021D6040 = OS_GetTickLo();
    sEnableVAD = 1;
    sAudioSessions = 0;
    sAudioMask = 0;
    if (__vct.mode == 1) {
        sAudioCodec = 0;
    } else {
        sAudioCodec = 4;
    }
    sAudioSize = 0x440;
    sAudioPacketSize = ((const Codec *)sCodecInfo)[sAudioCodec].payload * 0x44 + ((const Codec *)sCodecInfo)[sAudioCodec].header + 12;
    sSendInfo.read = 0;
    sSendInfo.write = 0;
    sSendInfo.mask = 0;
    sSendInfo.buffer2 = 0;
    sSendInfo.balanceIndex = 0;
    sSendInfo.balance = 0;
    vct_init_vad();
    InitializeAudioStreams();
    vct_init_decoder();
    InitFIRFilter();
    return 1;
}

#pragma pop

void vct_finish_audio(void) {
    sAudioSessions = 0;
}

#pragma push
#pragma optimization_level 2

int VCT_StartStreaming(Session *s) {
    u32 i;
    u32 n;
    if (!s) {
        return 0;
    }
    if (sAudioSessions == 3) {
        return 0;
    }
    if (s->mode != 2 && s->mode != 3) {
        return 0;
    }
    n = sizeof(sAudioSession) / sizeof(sAudioSession[0]);
    for (i = 0; i < n; i++) {
        if (sAudioSession[i] == s) {
            return 1;
        }
    }
    for (i = 0; i < n; i++) {
        if (sAudioSession[i] == 0) {
            sAudioSession[i] = s;
            break;
        }
    }
    sAudioSessions++;
    sAudioMask |= s->mask;
    vct_init_stream(&sStream[i], i, s->other);
    vct_init_audio_queue(i);
    if (s->mode != 2) {
        return 1;
    }
    if (sAudioSessions == 1) {
        VCT_ResetVAD();
        _021D6040 = OS_GetTickLo();
        sAdpcmState.value = 0;
        sAdpcmState.index = 0;
        sAudioReset = 1;
        sSendInfo.read = 0;
        sSendInfo.write = 0;
        sSendInfo.mask = 0;
        sSendInfo.buffer2 = 0;
        sSendInfo.balanceIndex = 0;
        sSendInfo.balance = 0;
    }
    return 1;
}

#pragma pop

void VCT_StopStreaming(Session *session) {
    u32 i;
    for (i = 0; i < 3; i++) {
        if (sAudioSession[i] == session) {
            sAudioSession[i] = 0;
            sAudioSessions--;
            vct_reset_stream(&sStream[i]);
            vct_flush_audio_queue(i);
            vct_flush_decoder(i);
            sAudioMask &= ~session->mask;
            break;
        }
    }
    if (sAudioSessions != 0) {
        return;
    }
    sAudioReset = 1;
    VCT_ResetVAD();
    sAudioMask = 0;
}

#pragma push
#pragma opt_rotateloops off

int VCT_SendAudio(void *data, u32 size) {
    char *send;
    register u32 i;
    int found;
    found = 0;
    if (size != sAudioSize) {
        return 0;
    }
    if (!sAudioSessions) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (sAudioSession[i] && sAudioSession[i]->mode == 2) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return 0;
    }
    send = (char *)sSendInfo.buffer + ((sSendInfo.write & 1) ? 0 : 0x460);
    *(u32 *)send = 0x5f564354;
    if (sAudioCodec >= 2) {
        MIi_CpuCopyFast(data, (u32 *)(send + 16), size);
    } else {
        MIi_CpuCopyFast(data, (u32 *)(send + 12), size);
    }
    {
        u16 sequence = _021D6040;
        send[4] = 0x40;
        _021D6040 = sequence + 1;
        *(u16 *)(send + 6) = sequence;
    }
    *(u32 *)(send + 8) = _ll_udiv(OS_GetTick(), 64);
    sSendInfo.write++;
    return 1;
}

#pragma pop

int VCTi_ReceiveAudioData(void *dst, u32 size, u32 ch, u32 *mask) {
    int count;
    RPacket *p;
    Stream *stream;
    int first;
    u64 now;
    stream = &sStream[ch];
    first = 1;
    count = vct_count_audio_queue(ch);
    while (count) {
        now = OS_GetTick();
        p = vct_top_audio_queue(ch);
        if (p == NULL) {
            goto empty;
        }
        if (p->ready > now + 52) {
            if (p->flagged == 0 && stream->previous != NULL) {
                stream->retrans++;
                MIi_CpuCopyFast(((RPacket *)stream->previous)->data, dst, size);
                vct_free_packet_buffer(stream->previous);
                stream->previous = NULL;
                if (mask != NULL) {
                    *mask |= 1 << p->owner;
                }
                return 1;
            }
            return 0;
        }
        if (now > p->ready + 0x8B4C) {
            stream->last = p->sequence;
            count = vct_save_previous_buffer(stream, p);
            vct_top_audio_queue(ch);
            first = 0;
            continue;
        }
        {
            u32 seq = p->sequence;
            u32 last = stream->last;
            if (last + 1 != seq && last != 0 && last <= seq && p->flagged == 0) {
                stream->lost++;
            }
        }
        stream->last = p->sequence;
        if (first && stream->skip) {
            count = vct_save_previous_buffer(stream, p);
            vct_top_audio_queue(ch);
            stream->skip--;
            continue;
        }
        {
            u32 delta = now - p->receive;
            u32 diff = delta - stream->oldDelay;
            u32 d;
            stream->oldDelay = delta;
            d = delta + ((diff - delta) >> 4);
            stream->delay = d;
            stream->jitter = d;
        }
        MIi_CpuCopyFast(p->data, dst, size);
        vct_save_previous_buffer(stream, p);
        if (mask != NULL) {
            *mask |= 1 << p->owner;
        }
        return 1;
empty:
        stream->left = count;
        return 0;
    }
    if (stream->previous != NULL) {
        vct_free_packet_buffer(stream->previous);
        stream->previous = NULL;
    }
    return 0;
}

int VCT_ReceiveAudio(void *data, u32 size, u32 *mask) {
    u32 i, j;
    int result;
    if (mask) {
        *(volatile u32 *)mask = 0;
    }
    result = 0;
    if (size == sAudioSize && sAudioSessions) {
        for (i = 0; i < 3; i++) {
            if (VCTi_ReceiveAudioData(data, size, i, mask) == 1) {
                result = 1;
                break;
            }
        }
        if (result) {
            j = i + 1;
            if (j == 3) {
                result = 1;
            } else {
                for (; j < 3; j++) {
                    if (VCTi_ReceiveAudioData(_021D6234, size, j, mask) == 1) {
                        vct_mix_audio(data, _021D6234, data, size);
                    }
                }
            }
        } else {
            MIi_CpuClearFast(0, data, size);
        }
    } else {
        MIi_CpuClearFast(0, data, size);
        result = 0;
    }
    if (sAudioEcho) {
        vct_set_speaker_samples(data, size);
    }
    return result;
}

s32 VCT_SetCodec(s32 codec) {
    if (codec >= 5) {
        return 0;
    }
    if (__vct.mode != 1 && (u32)codec <= 1) {
        return 0;
    }
    sAudioPacketSize = sCodecInfo[codec * 2] * 0x44 + (sCodecInfo + 1)[codec * 2] + 12;
    sAudioCodec = codec;
    sAdpcmState.value = 0;
    sAdpcmState.index = 0;
    return 1;
}

void VCT_EnableVAD(s32 enable) {
    sEnableVAD = enable;
    VCT_ResetVAD();
    if (enable == 0) {
        sAudioFlag = 1;
    }
}

void VCT_EnableEchoCancel(s32 enable) {
    sAudioEcho = enable;
}

void *vct_prepare_send_buffer(void) {
    char *audio, *packet;
    u32 power, root;
    int mode;
    u32 pending;
    mode = sAudioCodec;
    pending = sSendInfo.write - sSendInfo.read;
    if (pending > 1) {
        sSendInfo.read = sSendInfo.write - 1;
        pending = 1;
    }
    if (!pending) {
        return 0;
    }
    packet = (char *)sSendInfo.buffer + ((sSendInfo.read & 1) ? 0 : 0x460);
    audio = packet + 12;
    if (mode >= 2) {
        audio += 4;
    }
    power = HPFFilter(audio, sAudioSize);
    *(volatile u16 *)0x040002b0 = 0;
    *(volatile u32 *)0x040002b8 = power;
    while (*(volatile u16 *)0x040002b0 & 0x8000) {}
    root = *(volatile u32 *)0x040002b4;
    if (sAudioEcho) {
        vct_process_fir(audio, sAudioSize, root);
    }
    F(packet, 5, u8) = mode;
    if (sEnableVAD) {
        int status = VCTi_GetVADStatus(power, root, 0);
        sAudioVad = status;
        switch (status) {
        case 0:
            sSendInfo.read++;
            return 0;
        case 1:
            F(packet, 5, u8) |= 128;
            break;
        case 3:
            F(packet, 4, u8) = 65;
            break;
        default:
            break;
        }
    }
    if (!sEnableVAD && sAudioFlag) {
        {
            int zero = 0;
            u8 flags = F(packet, 5, u8);
            sAudioFlag = zero;
            F(packet, 5, u8) = flags | 128;
        }
    }
    if (sAudioReset) {
        {
            int zero = 0;
            u8 flags = F(packet, 5, u8);
            sAudioReset = zero;
            F(packet, 5, u8) = flags | 128;
        }
    }
    if (mode >= 2) {
        if (F(packet, 5, u8) & 128) {
            sAdpcmState.value = 0;
            sAdpcmState.index = 0;
        }
        vct_encode_adpcm(audio - 4, audio, sAudioSize >> 1, &sAdpcmState, mode);
    } else if (mode == 1) {
        vct_encode_g711_ulaw(audio, audio, sAudioSize >> 1);
    } else if (mode == 0) {
        vct_encode_8bit_raw(audio, audio, sAudioSize >> 1);
    }
    {
        u32 mask = sAudioMask;
        sSendInfo.mask = mask;
        mask &= ~(1u << __vct.aid);
        sSendInfo.mask = mask;
        sSendInfo.balance = MATH_CountPopulation(mask) - 1;
    }
    if (sSendInfo.balance > 7) {
        return 0;
    }
    sSendInfo.balanceIndex = 0;
    return packet;
}

int vct_handle_audio(u32 sender, char *header, u32 size, u64 ticks) {
    register Packet *p;
    Stream *stream = 0;
    u32 i;
    int codec;
    void *destination;
    if (!sAudioSessions) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (sender == sStream[i].session) {
            stream = &sStream[i];
            break;
        }
    }
    if (!stream) {
        return 0;
    }
    if ((u8)header[4] == 0x41) {
        stream->seq = *(u16 *)(header + 6);
        stream->silence = 0;
        stream->vad = 1;
        return 0;
    }
    if ((u8)header[4] != 0x40) {
        return 0;
    }
    codec = (u8)header[5] & 0x7f;
    stream->codec = codec;
    if (codec >= 5) {
        return 0;
    }
    if (size != 68 * sCodecInfo[codec * 2] + (sCodecInfo + 1)[codec * 2] + 12) {
        return 0;
    }
    if (!(sAudioMask & (sender ? (1 << sender) : 1))) {
        return 0;
    }
    p = vct_alloc_packet_buffer();
    if (!p) {
        vct_flush_audio_queue(stream->owner);
        vct_flush_decoder(stream->owner);
        p = vct_alloc_packet_buffer();
        if (!p) {
            vct_flush_all_audio_queue();
            vct_flush_all_decoder();
            p = vct_alloc_packet_buffer();
        }
        if (!p) {
            return 0;
        }
    }
    destination = (char *)p + 0x45c - (size - 12);
    MI_CpuCopy8(header + 12, destination, size - 12);
    p->sender = stream->owner;
    p->codec = codec;
    p->data = destination;
    p->size = size - 12;
    p->receiveLo = (u32)ticks;
    p->receiveHi = (u32)(ticks >> 32);
    p->owner = sender;
    p->flagged = (u8)header[5] & 0x80;
    p->timestamp = *(u32 *)(header + 8);
    p->sequence = *(u16 *)(header + 6);
    if (!vct_calc_jitter_and_skew(stream, p)) {
        vct_free_packet_buffer(p);
        return 0;
    }
    vct_insert_decoder(p);
    return 1;
}

int vct_decode_audio_buffer(void) {
    int i, count, owner;
    OSIntrMode irq;
    Packet *p = vct_top_decoder();
    if (!p) {
        return 0;
    }
    irq = OS_DisableInterrupts();
    owner = p->sender;
    count = vct_count_decoder_queue(owner);
    for (i = 0; i < count; i++) {
        p = vct_get_packet_from_decoder(owner);
        if (p->codec >= 2) {
            vct_decode_adpcm(p->data, p->samples, p->size, p->codec);
        } else if (p->codec == 1) {
            vct_decode_g711_ulaw(p->data, p->samples, p->size);
        } else {
            vct_decode_8bit_raw(p->data, p->samples, p->size);
        }
        p->data = p->samples;
        vct_remove_packet_from_decoder(p);
        if (vct_insert_audio_queue(p, p->sender) < 0) {
            vct_free_packet_buffer(p);
        }
    }
    OS_RestoreInterrupts(irq);
    return count;
}
