#pragma force_active on
// clang-format off
#include "global.h"
#pragma force_active on
#pragma opt_propagation off
#include <nitro/code32.h>
// clang-format on
#include <string.h>
extern int sscanf(const char *,const char *,...);
extern const char ov00_02219EF0[];
typedef struct {u8 p[0x88];void (*write)(u8,void *,void *);void (*write1)(u8,int,void *,void *);void (*write2)(u8,int,void *,void *);void (*enumerate)(int,void *,void *);int (*count)(int,void *);u8 p1[0xa8 - 0x9c];void (*notify)(u32,u16,void *);u8 p2[0x104-0xac];u32 address;u16 port,pad;void *arg;} Work;
static inline u32 swap32(u32 address){return ((address>>24)&255)|((address>>8)&0xff00)|((address<<8)&0xff0000)|((address<<24)&0xff000000);}
extern char ov00_02219EFC[],ov00_02219F04[];
extern char *ov00_0221A0B8[];
extern void ov00_02210DFC(void *,const char *);
typedef struct {u8 data[2048];u32 used;} Buffer;
static inline void CopyWire(Buffer*b,const volatile u8*source){u8 first=*source;u32 offset=((volatile Buffer*)b)->used;u8 last=source[1];u8*dest=b->data+offset;b->data[offset]=first;dest[1]=last;}
void ov00_02211358(Work *,Buffer *,int,int,u8 *);
void ov00_022112A8(Work *,const char *);
#pragma opt_lifetimes off
void ov00_02211358(Work *w,Buffer *b,int mode,int count,u8 *indices){struct{u8 ids[256];int count;} all;volatile u16 wire;int rows,i,j;all.count=0;if(count==0)return;if((u32)(mode-1)<=1){if(2048-b->used<2)return;rows=w->count(mode,w->arg);{int value=(u16)rows;wire=((value>>8)&255)|((value<<8)&0xff00);memcpy(b->data+((volatile Buffer*)b)->used,(const void*)&wire,2);}b->used+=2;}else rows=1;if(count==255){w->enumerate(mode,&all,w->arg);j=0;if(all.count>0){indices=all.ids;do{u32 before;const char *name=ov00_0221A0B8[*indices];if(!name)name=ov00_02219EFC;ov00_02210DFC(b,name);if(mode==0){before=b->used;w->write(*indices,b,w->arg);if(before==b->used)ov00_02210DFC(b,ov00_02219F04);}j++;indices++;}while(j<all.count);}if((int)(2048-b->used)<1)return;b->data[b->used++]=0;count=all.count;indices=all.ids;if(mode==0)return;}i=0;if(rows<=0)return;do{j=0;if(count>0){do{u32 before=b->used;if(mode==0)w->write(indices[j],b,w->arg);else if(mode==1)w->write1(indices[j],i,b,w->arg);else if(mode==2)w->write2(indices[j],i,b,w->arg);if(before==b->used)ov00_02210DFC(b,ov00_02219F04);j++;}while(j<count);}i++;}while(i<rows);}
