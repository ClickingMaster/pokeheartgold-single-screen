#include "global.h"
#include "heap.h"
#include "pm_string.h"
#pragma force_active on
typedef struct FrontierDownload FrontierDownload;
typedef struct Ov67DownloadedName {u16 name[9];u8 a,b,pad[4],bytes[8];u8 b0:1,b1:1,rest:6;u8 end;} Source;
typedef Source DownloadRecords[30];
typedef struct {String *str;u8 gender,a,b,special;u8 bytes[8];} Dest;
typedef struct {enum HeapID heap;u8 prefix[0x20];FrontierDownload *download;u8 gap28[0x3c];String *genderNames[2];u8 gap6c[0x230];Dest display[30];u8 tail[0x30];} State;
typedef char StateSize[(sizeof(State)==0x4AC)?1:-1];
void *sub_0202D8E4(FrontierDownload*,enum HeapID);
void ov67_021E63E8(void*);
void ov67_021E63E8(void *raw) {
 State *s=raw;
 struct {int index;DownloadRecords *records;} query;
 query.records=sub_0202D8E4(s->download,s->heap);query.index=0;Dest *d=s->display;Source *p=*query.records;u8 *nameRecord=(u8*)query.records;
 for(;query.index<30;d++,p++,nameRecord+=sizeof(Source),query.index++){
  d->gender=p->b1;d->a=p->a;d->b=p->b;d->special=p->b0;MI_CpuCopy8(p->bytes,d->bytes,8);
  d->str=String_New(8,s->heap);
  if(d->special)String_Copy(d->str,s->genderNames[d->gender]);else CopyU16ArrayToString(d->str,(const u16*)nameRecord);
 }
 Heap_Free(query.records);
}
