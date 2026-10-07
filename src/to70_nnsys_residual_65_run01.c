#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
typedef void (*W17HeapCallback)(void *,u32,void *,int);
// clang-format on
typedef struct Wave {u32 p00[2],size;u8 p0c[44];u32 index;u32 offset[1];} Wave;
typedef struct SeqInfo {u32 data;u16 bank;u8 volume,channel,priority,player;} SeqInfo;
typedef struct StreamInfo {u8 count,p01;u16 channel;u32 size;} StreamInfo;
typedef struct InstPos {u32 p[2];} InstPos;
typedef struct InstData {u8 type,p01;u16 wave,bank;u8 tail[4];} InstData;
typedef struct Stream {u8 p00[0xe8];u8 fader[0x28];s32 active:1,started:1,ready:1,stopping:1,padFlags:28;int available,readyCount;u8 p11c[0x30];void **handle;u32 p150;int initialVolume,otherVolume,currentVolume;u32 p160[2];void (*dispose)(struct Stream*);u32 p16c;void (*stop)(struct Stream*);} Stream;
extern Stream _021E0FEC[4];
extern const s16 _0210D750[];
extern OSMutex _021E0FC8;
extern u8 _021E0FE0[];
extern void *_021E0950[];
void *SND_GetWaveDataAddress(Wave *,int);
int SND_GetWaveDataCount(Wave *);
void SND_SetWaveDataAddress(Wave *,int,void *);
InstPos SND_GetFirstInstDataPos(void);
int SND_GetNextInstData(void *,InstData *,InstPos *);
void SingleWaveDisposeCallback(void *,u32,void *,int);
void *NNS_SndHeapAlloc(void *,u32,W17HeapCallback,void *,int);
int NNS_SndArcReadFile(int,void *,u32,u32);
void *NNS_SndArcGetCurrent(void);
StreamInfo *NNS_SndArcGetStrmInfo(int);
void NNS_SndPlayerSetPlayableSeqCount(int,int);
void NNS_SndPlayerSetAllocatableChannel(int,u32);
int NNS_SndPlayerCreateHeap(int,struct NNSSndHeap *,u32);
SeqInfo *NNS_SndArcGetSeqInfo(int);
void *NNS_SndArcGetSeqArcInfo(int);
void *NNS_SndArcGetFileAddress(int);
SeqInfo *NNSi_SndSeqArcGetSeqInfo(void *,int);
void *NNSi_SndPlayerAllocSeqPlayer(void *,int,int);
void *NNSi_SndPlayerAllocHeap(int,void *);
void NNSi_SndPlayerFreeSeqPlayer(void *);
int NNSi_SndArcLoadBank(int,int,void *,int,void **);
int NNSi_SndArcLoadSeq(int,int,void *,int,void **);
void NNSi_SndPlayerStartSeq(void *,void *,u32,void *);
void NNS_SndPlayerSetInitialVolume(void *,int);
void NNS_SndPlayerSetChannelPriority(void *,int);
void NNS_SndPlayerSetSeqNo(void *,int);
void NNS_SndPlayerSetSeqArcNo(void *,int,int);
void NNS_SndStrmStart(Stream *);
void NNS_SndStrmStop(Stream *);
void NNS_SndStrmSetVolume(Stream *,int);
void NNSi_SndFaderUpdate(void *);
int NNSi_SndFaderGet(void *);
int NNSi_SndFaderIsFinished(void *);
void RemoveCommandByPlayer(void *,Stream *);
void FreeChannel(Stream *);
int LoadSingleWave(Wave *,int,int,struct NNSSndHeap *);
int LoadSingleWaves(Wave *,void *,int,int,void *);
int NNS_SndArcPlayerSetup(struct NNSSndHeap *);
int NNS_SndArcPlayerStartSeqEx(struct NNSSndHandle *,int,int,int,int);
int NNS_SndArcPlayerStartSeqArc(void *,int,int);
int StartSeq(void *,int,int,int,SeqInfo *,int);
int StartSeqArc(void *,int,int,int,SeqInfo *,u8 *,int,int);
void NNSi_SndArcStrmMain(void);
void FreePlayer(Stream *);
void ForceStopStrm__SndArc(Stream *);
void ShutdownPlayer__SndArc(Stream *);
int LoadSingleWaves(Wave *wave,void *bank,int bankNo,int file,void *heap){InstPos pos;InstPos initial;InstData inst;initial=SND_GetFirstInstDataPos();pos=initial;if(!bank)return 0;while(SND_GetNextInstData(bank,&inst,&pos)){if(inst.type==1&&bankNo==inst.bank){if(!LoadSingleWave(wave,inst.wave,file,heap))return 0;}}return 1;}
int NNS_SndArcPlayerSetup(struct NNSSndHeap *heap){int i;NNS_SndArcGetCurrent();for(i=0;i<32;i++){StreamInfo *info=NNS_SndArcGetStrmInfo(i);if(info){int j;NNS_SndPlayerSetPlayableSeqCount(i,info->count);NNS_SndPlayerSetAllocatableChannel(i,info->channel);if(info->size&&heap){for(j=0;j<info->count;j++){if(!NNS_SndPlayerCreateHeap(i,heap,info->size))return 0;}}}}return 1;}
int NNS_SndArcPlayerStartSeqEx(struct NNSSndHandle *handle,int player,int bank,int priority,int seq){SeqInfo *info=NNS_SndArcGetSeqInfo(seq);if(!info)return 0;if(priority<0)priority=info->priority;if(bank<0)bank=info->bank;if(player<0)player=info->player;return StartSeq(handle,player,bank,priority,info,seq);}
int NNS_SndArcPlayerStartSeqArc(void *handle,int arc,int index){u32 *info=NNS_SndArcGetSeqArcInfo(arc);u8 *data;SeqInfo *seq;if(!info)return 0;data=NNS_SndArcGetFileAddress(*info);if(!data)return 0;seq=NNSi_SndSeqArcGetSeqInfo(data,index);if(!seq)return 0;return StartSeqArc(handle,seq->player,seq->bank,seq->priority,seq,data,arc,index);}
int StartSeq(void *handle,int player,int bank,int priority,SeqInfo *info,int seq){void *p=NNSi_SndPlayerAllocSeqPlayer(handle,player,priority);void *heap,*seqData,*bankData;if(!p)return 0;heap=NNSi_SndPlayerAllocHeap(player,p);if(NNSi_SndArcLoadBank(bank,6,heap,0,&bankData)){NNSi_SndPlayerFreeSeqPlayer(p);return 0;}if(NNSi_SndArcLoadSeq(seq,1,heap,0,&seqData)){NNSi_SndPlayerFreeSeqPlayer(p);return 0;}NNSi_SndPlayerStartSeq(p,(u8 *)seqData+((u32 *)seqData)[6],0,bankData);NNS_SndPlayerSetInitialVolume(handle,info->volume);NNS_SndPlayerSetChannelPriority(handle,info->channel);NNS_SndPlayerSetSeqNo(handle,seq);return 1;}
int StartSeqArc(void *handle,int player,int bank,int priority,SeqInfo *info,u8 *data,int arc,int index){void *p=NNSi_SndPlayerAllocSeqPlayer(handle,player,priority);void *heap,*bankData;if(!p)return 0;heap=NNSi_SndPlayerAllocHeap(player,p);if(NNSi_SndArcLoadBank(bank,6,heap,0,&bankData)){NNSi_SndPlayerFreeSeqPlayer(p);return 0;}NNSi_SndPlayerStartSeq(p,data+((u32 *)data)[6],info->data,bankData);NNS_SndPlayerSetInitialVolume(handle,info->volume);NNS_SndPlayerSetChannelPriority(handle,info->channel);NNS_SndPlayerSetSeqArcNo(handle,arc,index);return 1;}
