#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Packet {char pad[12];int codec;u8 owner,sender;u16 gap;u32 size,sequence;char samples[0x440];int flagged,timestamp,receiveLo,receiveHi;u32 readyLo,readyHi;void *data;} Packet;
extern char __vct[],sTransceiverBalance[];
typedef struct{u8 pad[12];u32 size;u8 p[8];int send,p1c,enabled;u8 q[0xf00-0x24];u32 sequence,pad2;void*packet;u32 targets,slot,balance;}Sender;extern Sender _021D6040;
extern Packet *vct_top_decoder(void),*vct_get_packet_from_decoder(int);
extern int vct_count_decoder_queue(int),vct_insert_audio_queue(Packet*,int),DWC_SendUnreliable(u8,const void*,u32);
extern void vct_decode_adpcm(void*,void*,u32,int),vct_decode_g711_ulaw(void*,void*,u32),vct_decode_8bit_raw(void*,void*,u32),vct_remove_packet_from_decoder(Packet*),vct_free_packet_buffer(Packet*);
extern void *vct_prepare_send_buffer(void);
static inline u32 Clz(u32 x) { asm { clz x, x } return x; }
int vct_decode_audio_buffer(void);int vct_flush_send_buffer(void);
typedef struct Stream{u32 jitter,p4,left,pC,p10,lost,p18,p1c,retrans,codec,oldDelay,delay,seq,owner,session;Packet*previous;char gap40[24];u32 last;char gap5c[16];int vad,skip;char rest[32];} Stream;
extern Stream sStream[];extern u8 sCodecInfo[];
extern Packet*vct_alloc_packet_buffer(void);
extern void vct_flush_audio_queue(int),vct_flush_decoder(int),vct_flush_all_audio_queue(void),vct_flush_all_decoder(void),vct_insert_decoder(Packet*);
extern int vct_calc_jitter_and_skew(Stream*,Packet*);
int vct_handle_audio(u32,char*,u32,u32,u32);
// @fn vct_flush_send_buffer
int vct_flush_send_buffer(void)
{
    int i, limit;
    if (!_021D6040.enabled)
        return 1;
    if (!_021D6040.packet) {
        void *p = vct_prepare_send_buffer();
        _021D6040.packet = p;
        if (p && _021D6040.send)
            return 0;
        return 1;
    }
    limit = 1;
    if (*(int *)(__vct + 16) == 2) {
        u32 slot = _021D6040.slot;
        _021D6040.slot = slot + 1;
        limit = ((u8 (*)[3])sTransceiverBalance)[_021D6040.balance][slot];
    }
    for (i = 0; i < limit; i++) {
        u32 mask = _021D6040.targets;
        u32 leading = Clz(mask);
        if (leading == 32)
            break;
        _021D6040.targets = mask & ~(0x80000000u >> leading);
        DWC_SendUnreliable((u8)(31 - leading), _021D6040.packet, _021D6040.size);
        if (!_021D6040.targets) {
            _021D6040.sequence++;
            _021D6040.packet = 0;
            _021D6040.targets = 0;
            break;
        }
    }
    return 1;
}
