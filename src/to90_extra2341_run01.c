#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
typedef void (*W17HeapCallback)(void *,u32,void *,int);
// clang-format on
#pragma force_active on
extern void InfoDisposeCallback(void *,u32,void *,int);
extern void FatDisposeCallback(void *,u32,void *,int);
extern void SymbolDisposeCallback(void *,u32,void *,int);
extern void *NNS_SndHeapAlloc(void *,u32,W17HeapCallback,void *,int);
BOOL NNS_SndArcSetup(NNSSndArc *,NNSSndHeapHandle,BOOL);
BOOL NNS_SndArcSetup(NNSSndArc*arc,NNSSndHeapHandle heap,BOOL symbols){s32 got;if(!FS_SeekFile(&arc->file,0,FS_SEEK_SET))return FALSE;if(FS_ReadFile(&arc->file,arc,48)!=48)return FALSE;if(heap){arc->info=NNS_SndHeapAlloc(heap,arc->header.infoSize,InfoDisposeCallback,arc,0);if(!arc->info)return FALSE;if(!FS_SeekFile(&arc->file,arc->header.infoOffset,FS_SEEK_SET))return FALSE;got=FS_ReadFile(&arc->file,arc->info,arc->header.infoSize);if(got!=(s32)arc->header.infoSize)return FALSE;arc->fat=NNS_SndHeapAlloc(heap,arc->header.fatSize,FatDisposeCallback,arc,0);if(!arc->fat)return FALSE;if(!FS_SeekFile(&arc->file,arc->header.fatOffset,FS_SEEK_SET))return FALSE;got=FS_ReadFile(&arc->file,arc->fat,arc->header.fatSize);if(got!=(s32)arc->header.fatSize)return FALSE;if(symbols&&arc->header.symbolDataSize){arc->symbol=NNS_SndHeapAlloc(heap,arc->header.symbolDataSize,SymbolDisposeCallback,arc,0);if(!arc->symbol)return FALSE;if(!FS_SeekFile(&arc->file,arc->header.symbolDataOffset,FS_SEEK_SET))return FALSE;got=FS_ReadFile(&arc->file,arc->symbol,arc->header.symbolDataSize);if(got!=(s32)arc->header.symbolDataSize)return FALSE;}}return TRUE;}
