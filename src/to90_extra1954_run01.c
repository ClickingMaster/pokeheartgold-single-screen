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
#pragma force_active on
#pragma opt_rotateloops off
int VCT_SendAudio(void *data,u32 size) {
 char *send;register u32 i;int found;found=0;
 if(size!=_021D6040.size) return 0; if(!_021D6040.active) return 0;
 for(i=0;i<3;i++) {if(sAudioSession[i] && sAudioSession[i]->state==2) {found=1;break;}}
 if(!found) return 0;
 send=sSendInfo+((_021D6040.pending&1)?0:0x460); *(u32*)send=0x5f564354;
 if(_021D6040.codec>=2) MIi_CpuCopyFast(data,(u32*)(send+16),size); else MIi_CpuCopyFast(data,(u32*)(send+12),size);
 {u16 sequence=_021D6040.sequence; send[4]=0x40; _021D6040.sequence=sequence+1; *(u16*)(send+6)=sequence;}
 *(u32*)(send+8)=_ll_udiv(OS_GetTick(),64); _021D6040.pending++; return 1;
}
