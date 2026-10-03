#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Packet {char pad[12];int codec;u8 owner,sender;u16 gap;u32 size,sequence;char samples[0x440];int flagged,timestamp,receiveLo,receiveHi;u32 readyLo,readyHi;void *data;} Packet;
extern char _021D6040[],__vct[],sTransceiverBalance[];
extern Packet *vct_top_decoder(void),*vct_get_packet_from_decoder(int);
extern int vct_count_decoder_queue(int),vct_insert_audio_queue(Packet*,int),DWC_SendUnreliable(u8,const void*,u32);
extern void vct_decode_adpcm(void*,void*,u32,int),vct_decode_g711_ulaw(void*,void*,u32),vct_decode_8bit_raw(void*,void*,u32),vct_remove_packet_from_decoder(Packet*),vct_free_packet_buffer(Packet*);
extern void *vct_prepare_send_buffer(void);
extern int MATH_CountLeadingZerosInline(u32);
int vct_decode_audio_buffer(void);int vct_flush_send_buffer(void);
typedef struct Stream{u32 jitter,p4,left,pC,p10,lost,p18,p1c,retrans,codec,oldDelay,delay,seq,owner,session;Packet*previous;char gap40[24];u32 last;char gap5c[16];int vad,skip;char rest[32];} Stream;
extern Stream sStream[];extern u8 sCodecInfo[];
extern Packet*vct_alloc_packet_buffer(void);
extern void vct_flush_audio_queue(int),vct_flush_decoder(int),vct_flush_all_audio_queue(void),vct_flush_all_decoder(void),vct_insert_decoder(Packet*);
extern int vct_calc_jitter_and_skew(Stream*,Packet*);
int vct_handle_audio(u32,char*,u32,u32,u32);
int vct_decode_audio_buffer(void){int i,count,owner;OSIntrMode irq;Packet*p=vct_top_decoder();if(!p)return 0;irq=OS_DisableInterrupts();owner=p->sender;count=vct_count_decoder_queue(owner);for(i=0;i<count;i++){p=vct_get_packet_from_decoder(owner);if(p->codec>=2)vct_decode_adpcm(p->data,p->samples,p->size,p->codec);else if(p->codec==1)vct_decode_g711_ulaw(p->data,p->samples,p->size);else vct_decode_8bit_raw(p->data,p->samples,p->size);p->data=p->samples;vct_remove_packet_from_decoder(p);if(vct_insert_audio_queue(p,p->sender)<0)vct_free_packet_buffer(p);}OS_RestoreInterrupts(irq);return count;}
