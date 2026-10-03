#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct State { u16 sequence; char gap[0x16]; int speaker,codec,active,size; char rest[0xedc]; u32 pending; } State;
typedef struct Session { int gap[3],state; } Session;
extern State _021D6040;
extern Session *sAudioSession[];
extern char sSendInfo[],_021D6234[];
extern void vct_mix_audio(void*,void*,void*,u32),vct_set_speaker_samples(void*,u32);
extern int VCTi_ReceiveAudioData(void*,u32,u32,u32*);
extern u64 _ll_udiv(u64,u64);
int VCT_SendAudio(void*,u32); int VCT_ReceiveAudio(void*,u32,u32*);
typedef struct Packet{char gap[16];u8 owner,sender;u16 pad;u32 size,sequence;char dataBytes[0x440];u32 flagged,timestamp,receiveLo,receiveHi,readyLo,readyHi;void*data;} Packet;
typedef struct Stream{u32 jitter,p4,left,pC,p10,lost,p18,p1c,retrans,codec,oldDelay,delay,seq,owner,session;Packet*previous;char gap40[24];u32 last;char gap5c[16];int vad,skip;char rest[32];} Stream;
extern Stream sStream[];
extern int vct_count_audio_queue(u32),vct_save_previous_buffer(Stream*,Packet*);
extern Packet*vct_top_audio_queue(u32);
extern void vct_free_packet_buffer(Packet*);
int VCT_ReceiveAudio(void *data,u32 size,u32*mask) {
 u32 i,j; int result; if(mask)*(volatile u32*)mask=0; result=0;
 if(size==_021D6040.size && _021D6040.active) {
 for(i=0;i<3;i++) if(VCTi_ReceiveAudioData(data,size,i,mask)==1) {result=1;break;}
 if(result) {j=i+1;if(j==3)result=1;else for(;j<3;j++) if(VCTi_ReceiveAudioData(_021D6234,size,j,mask)==1) vct_mix_audio(data,_021D6234,data,size);}
 else MIi_CpuClearFast(0,data,size);
 }else { MIi_CpuClearFast(0,data,size);result=0;}
 if(_021D6040.speaker) vct_set_speaker_samples(data,size);return result;
}
