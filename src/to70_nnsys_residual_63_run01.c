#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct BankInfo {u32 file;u16 waves[4];} BankInfo;
typedef struct WaveInfo {u32 file:24;u32 flags:8;} WaveInfo;
BankInfo *NNS_SndArcGetBankInfo(int);WaveInfo *NNS_SndArcGetWaveArcInfo(int);void *LoadBank(u32,void*,int);void *NNS_SndArcGetFileAddress(u32);int NNSi_SndArcLoadWaveArc(int,u32,void*,int,void**);int LoadSingleWaves(void*,void*,int,u32,void*);void SND_AssignWaveArc(void*,int,void*);int NNSi_SndArcLoadBank(int,u32,void*,int,void**);
int NNSi_SndArcLoadBank(int id,u32 flags,void *heap,int setAddress,void **output){BankInfo *info=NNS_SndArcGetBankInfo(id);void *bank;if(info==0)return 4;if(flags&2){bank=LoadBank(info->file,heap,setAddress);if(bank==0)return 8;}else bank=NNS_SndArcGetFileAddress(info->file);for(int i=0;i<4;i++){if(info->waves[i]!=65535){WaveInfo *waveInfo=NNS_SndArcGetWaveArcInfo(info->waves[i]);void *wave;int result;if(waveInfo==0)return 5;result=NNSi_SndArcLoadWaveArc(info->waves[i],flags,heap,setAddress,&wave);if(result!=0)return result;if((waveInfo->flags&1)&&(flags&4)){if(!LoadSingleWaves(wave,bank,i,waveInfo->file,heap))return 9;}if(bank&&wave)SND_AssignWaveArc(bank,i,wave);}}if(output)*output=bank;return 0;}
