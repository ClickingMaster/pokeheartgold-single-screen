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
void MBi_MakeDownloadFileInfo(Info*out,char*header) {
 u32 address=0x22c0000;u32*list=MBi_defaultLoadSegList;char*segment=out->segments;int i;
 out->a=*(u32*)(header+0x24);out->b=*(u32*)(header+0x34);
 for(i=0;i<3;i++){MBi_SetSegmentInfo(header,list,segment,&address);segment+=16;list++;}
 MI_CpuCopy8(header+0x160,out->tail,0x88);
}
