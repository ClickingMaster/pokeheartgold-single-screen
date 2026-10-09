// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { u8 header[0x38]; u32 waveCount; u32 offsets[1]; } WaveArcTable;
typedef void (*SndDisposeCb)(void*,u32,u32,u32);
typedef struct { u8 header[0x38]; u32 waveCount; } WaveArcHeader;
WaveArcHeader _021E0914;
extern void *NNS_SndArcGetFileAddress(int);
extern s32 NNS_SndArcReadFile(int,void*,s32,s32);
extern void *NNS_SndArcGetCurrent(void);
extern void *NNS_SndHeapAlloc(void*,u32,SndDisposeCb,u32,u32);
extern void NNS_SndArcSetFileAddress(int,const void*);
extern void WaveArcTableDisposeCallback(void*,u32,u32,u32);
WaveArcTable *LoadWaveArcTable(int,void*,BOOL);
// @fn LoadWaveArcTable
WaveArcTable *LoadWaveArcTable(int fileId,void *heap,BOOL setAddr){
 u32 size2;
 WaveArcTable *table;
 u32 size;
 void *arc;
 table=NNS_SndArcGetFileAddress(fileId);
 if(table==NULL){
  if(NNS_SndArcReadFile(fileId,_021E0914.header,0x3c,0)!=0x3c)return NULL;
  size=_021E0914.waveCount*sizeof(u32);
  size2=size*2;
  if(heap==NULL)return NULL;
  arc=setAddr?NNS_SndArcGetCurrent():NULL;
  table=NNS_SndHeapAlloc(heap,size2+0x5c,WaveArcTableDisposeCallback,(u32)arc,(u32)fileId);
  if(table==NULL)return NULL;
  {s32 n=NNS_SndArcReadFile(fileId,table,0x3c+size,0);if(n!=0x3c+size)return NULL;}
  MI_CpuCopy8(&table->offsets[0],&table->offsets[table->waveCount],size);
  MI_CpuFill8(&table->offsets[0],0,size);
  DC_StoreRange(table,size2+0x3c);
  if(setAddr)NNS_SndArcSetFileAddress(fileId,table);
 }
 return table;
}
