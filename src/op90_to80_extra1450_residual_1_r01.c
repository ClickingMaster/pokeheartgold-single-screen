#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <stddef.h>
// clang-format on
typedef struct{u8 prefix20[0x20];u32 arm9Offset,entry9,arm9Address,arm9Size,arm7Offset,entry7,arm7Address,arm7Size;u8 gap40[0x20];u32 flags;u8 gap64[0x1c];u32 fileEnd;u8 tail[0x160-0x84];} W39055Header;
typedef struct{u32 reserved,arm9Offset,arm7Offset,unused;} W39055Segments;
typedef struct{u32 offset,size;void*data;u32 state;}Entry;
typedef struct{u32 busy;u8 opaque04[12];char archiveName[4];u32 nameLength;FSArchive*archive;u8 opaque1c[20];Entry entries[4];}Cache;
typedef struct Segment{u32 start;char*source,*destination;u32 size;}Segment;
typedef struct{u32 start,size;} W39055Range;
typedef char W39055HeaderSize[(sizeof(W39055Header)==0x160)?1:-1];
typedef char W41040EntrySize[(sizeof(Entry)==0x10)?1:-1];
typedef char CacheSize[(sizeof(Cache)==0x70)?1:-1];
typedef char CacheEntries[(offsetof(Cache,entries)==0x30)?1:-1];
typedef char W41040SegmentSize[(sizeof(Segment)==0x10)?1:-1];
typedef char CacheName10[(offsetof(Cache,archiveName)==0x10)?1:-1];
typedef char CacheLength14[(offsetof(Cache,nameLength)==0x14)?1:-1];
typedef char CacheArchive18[(offsetof(Cache,archive)==0x18)?1:-1];
typedef char CacheArm9Data48[(offsetof(Cache,entries)+sizeof(Entry)+offsetof(Entry,data)==0x48)?1:-1];
extern const char ov13_02245988[];extern const W39055Range*mbi_seg_header;extern void _start_AutoloadDoneCallback(void);
extern void MBi_InitCache(void*);
extern void MBi_AttachCacheBuffer(Cache*,u32,u32,void*,u32);
extern void MBi_ReadSegmentHeader(Segment*,u32,u32,int);
int MB_ReadSegment(FSFile*,void*,u32);
static inline void W41040ReadRange(Segment*info,const W39055Range*range){MBi_ReadSegmentHeader(info,range->start,range->start+range->size,0);}
static inline void ReadFullSegments(FSFile *file,W39055Header *header,W39055Segments *segments,Cache *cache,u8 *cursor){
 u32 top=file->prop.file.top;
 u32 first;
 FS_SeekFile(file,(s32)(segments->arm9Offset-top),FS_SEEK_SET);
 FS_ReadFile(file,cursor,(s32)header->arm9Size);
 MBi_AttachCacheBuffer(cache,segments->arm9Offset,header->arm9Size,cursor,3);
 first=header->arm9Size;
 FS_SeekFile(file,(s32)(segments->arm7Offset-top),FS_SEEK_SET);
 FS_ReadFile(file,cursor+first,(s32)header->arm7Size);
 MBi_AttachCacheBuffer(cache,segments->arm7Offset,header->arm7Size,cursor+first,3);
}
#pragma opt_propagation off
// @fn MB_ReadSegment
int MB_ReadSegment(FSFile*file,void*buffer,u32 length){
 FSFile local;Segment info;W39055Header*header=buffer;int result=0,own;u32 remain,origin,limit;u8*cursor;Cache*cache;W39055Segments*segments;
 if(length>=0x164){
 remain=length;cursor=buffer;own=0;cache=0;segments=0;cursor+=0x160;remain-=0x160;
 if(file){origin=file->prop.file.pos-file->prop.file.top;if(FS_ReadFile(file,buffer,0x160)<0x160)remain=0;limit=header->fileEnd;if(!limit)limit=0x1000000;}
 else{
  {const W39055Header*boot=(const W39055Header*)0x027ffe00;limit=boot->fileEnd;}if(!limit)limit=0x1000000;own=1;FS_InitFile(&local);
  FS_OpenFileDirect(&local,FS_FindArchive(ov13_02245988,3),0,limit+0x88,0xffffffffu);
  {const W39055Header*boot=(const W39055Header*)0x027ffe00;origin=local.prop.file.pos-local.prop.file.top;file=&local;MI_CpuCopy8(boot,buffer,0x160);}header->flags|=0x406000;
 }
 if(remain<0x88)remain=0;
 else{FS_SeekFile(file,(s32)(origin+limit),FS_SEEK_SET);FS_ReadFile(file,cursor,0x88);cursor+=0x88;remain-=0x88;}
 if(remain>=0x70){int count;FSArchive*archive;
  cache=(Cache*)cursor;MBi_InitCache(cache);cursor+=0x70;remain-=0x70;MBi_AttachCacheBuffer(cache,0,0x160,buffer,3);
  archive=file->arc;count=0;while(count<3&&archive->name.ptr[count])count++;
  MI_CpuCopy8(archive->name.ptr,cache->archiveName,count);cache->nameLength=count;cache->archive=archive;
 }else remain=0;
 if(remain<0x10)remain=0;
 else{segments=(W39055Segments*)cursor;segments->reserved=0;segments->arm9Offset=header->arm9Offset+(origin+file->prop.file.top);segments->arm7Offset=header->arm7Offset+(origin+file->prop.file.top);cursor+=0x10;remain-=0x10;}
 if(remain>=header->arm9Size+header->arm7Size){ReadFullSegments(file,header,segments,cache,cursor);result=1;
 }else if(remain>=0xcc00){u32 start;u32 top;top=file->prop.file.top;start=segments->arm9Offset;
  FS_SeekFile(file,(s32)(start-top),FS_SEEK_SET);FS_ReadFile(file,cursor,0x4400);MBi_AttachCacheBuffer(cache,start,0x4400,cursor,3);
  FS_SeekFile(file,(s32)(start+0x4400-top),FS_SEEK_SET);FS_ReadFile(file,cursor+0x4400,0x4400);MBi_AttachCacheBuffer(cache,start+0x4400,0x4400,cursor+0x4400,2);
  FS_SeekFile(file,(s32)(start+0x8800-top),FS_SEEK_SET);FS_ReadFile(file,cursor+0x8800,0x4400);MBi_AttachCacheBuffer(cache,start+0x8800,0x4400,cursor+0x8800,2);result=1;
 }
 FS_SeekFile(file,(s32)origin,FS_SEEK_SET);
 if(own){FS_CloseFile(&local);if(result){const W39055Range*range=mbi_seg_header;
  info.start=header->arm9Offset;info.source=(char*)(header->arm9Address-header->arm9Offset);info.destination=(char*)((u32)cache->entries[1].data-header->arm9Offset);info.size=length;
  MBi_ReadSegmentHeader(&info,0x4000,0x8000,1);
  while(range->size){W41040ReadRange(&info,range);range++;}
  {u8*image=cache->entries[1].data;u32 offset=(u32)_start_AutoloadDoneCallback-header->arm9Address;*(u32*)(image+offset)=0xe12fff1e;}
 }}
 if(result)DC_FlushRange(buffer,length);}
 return result;
}
