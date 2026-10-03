#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Segment {u32 start; char *source,*destination;u32 size;} Segment;
typedef struct Info {u32 a,b,pad;char segments[48],tail[136];} Info;
extern u32 MBi_defaultLoadSegList[];
extern void MBi_SetSegmentInfo(void*,u32*,void*,u32*);
void MBi_ReadSegmentHeader(Segment*,u32,u32,int);void MBi_MakeDownloadFileInfo(Info*,char*);int MBi_IsAbleToRecv(u32,u32,u32);int IsAbleToLoad(u32,u32,u32);
typedef struct SegmentInfo{u32 destination,load,size,extra;} SegmentInfo;
typedef struct Download{u32 fields[3];SegmentInfo segments[3];} Download;
typedef struct BlockTable{u32 offsets[3];u16 boundaries[3],count;} BlockTable;
typedef struct Block{u32 address,size,position;u8 segment;} Block;
extern char *mbc;
int MBi_MakeBlockInfoTable(BlockTable*,Download*);int MBi_get_blockinfo(Block*,BlockTable*,u16,Download*);
int MBi_MakeBlockInfoTable(BlockTable*out,Download*info){u32 offset=0;u8 i;u16 end;u16*boundaries=out->boundaries;if(!info)return 0;for(i=0;i<3;i++){out->offsets[i]=offset;offset+=info->segments[i].size;}boundaries[0]=0;for(i=0;i<3;i++){SegmentInfo*s=&info->segments[i];u32 block=*(u32*)(mbc+0x1318);u32 size=s->size;u16 count=(u16)((size+block-1)/block);end=boundaries[i]+count;if(!IsAbleToLoad(i,s->load,size))return 0;if(i<2)boundaries[i+1]=end;else out->count=end;}return 1;}
