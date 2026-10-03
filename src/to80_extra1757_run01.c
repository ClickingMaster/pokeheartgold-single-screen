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
typedef struct Stream{u32 jitter,p4,left,pC,p10,lost,p18,p1c,retrans,codec,oldDelay,delay,seq,owner,session;Packet*previous;char gap40[12];u32 silence;char gap50[8];u32 last;char gap5c[16];int vad,skip;char rest[32];} Stream;
extern Stream sStream[];extern u8 sCodecInfo[];
extern Packet*vct_alloc_packet_buffer(void);
extern void vct_flush_audio_queue(int),vct_flush_decoder(int),vct_flush_all_audio_queue(void),vct_flush_all_decoder(void),vct_insert_decoder(Packet*);
extern int vct_calc_jitter_and_skew(Stream*,Packet*);
int vct_handle_audio(u32,char*,u32,u64);
int vct_handle_audio(u32 sender,char*header,u32 size,u64 ticks){register Packet*p;Stream*stream=0;u32 i;int codec;void*destination;
 if(!*(int*)(_021D6040+0x20))return 0;for(i=0;i<3;i++)if(sender==sStream[i].session){stream=&sStream[i];break;}if(!stream)return 0;
 if((u8)header[4]==0x41){stream->seq=*(u16*)(header+6);stream->silence=0;stream->vad=1;return 0;}if((u8)header[4]!=0x40)return 0;codec=(u8)header[5]&0x7f;stream->codec=codec;if(codec>=5)return 0;if(size!=68*sCodecInfo[codec*2]+(sCodecInfo+1)[codec*2]+12)return 0;if(!(*(u32*)(_021D6040+4)&(sender?(1<<sender):1)))return 0;
 p=vct_alloc_packet_buffer();if(!p){vct_flush_audio_queue(stream->owner);vct_flush_decoder(stream->owner);p=vct_alloc_packet_buffer();if(!p){vct_flush_all_audio_queue();vct_flush_all_decoder();p=vct_alloc_packet_buffer();}if(!p)return 0;}
 destination=(char*)p+0x45c-(size-12);MI_CpuCopy8(header+12,destination,size-12);p->sender=stream->owner;p->codec=codec;p->data=destination;p->size=size-12;p->receiveLo=(u32)ticks;p->receiveHi=(u32)(ticks>>32);p->owner=sender;p->flagged=(u8)header[5]&0x80;p->timestamp=*(u32*)(header+8);p->sequence=*(u16*)(header+6);
 if(!vct_calc_jitter_and_skew(stream,p)){vct_free_packet_buffer(p);return 0;}vct_insert_decoder(p);return 1;
}
