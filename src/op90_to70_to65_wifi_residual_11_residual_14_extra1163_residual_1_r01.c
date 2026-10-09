#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 first,p1;u16 total,id,frag;u8 p8[4];u16 sourceHigh,sourceLow;} Packet;
typedef struct{u16 end,count;u32 covered,index;} Coverage;
typedef struct {u32 key;u16 count,id,end,total,start[8],stop[8];u32 tick;u8 *data,*allocation;} Frag;
typedef struct {u8 p[0x14];void *(*alloc)(u32);u8 q[0x28];void (*free)(void*);} Hooks;
extern Frag _021D4948[8];extern const Hooks _021D4620[];
static inline u16 Swap(u16 x){return (u16)((x>>8)|(x<<8));}
static inline u32 SourceKey(const Packet*p){u16 low=Swap(p->sourceLow);u16 high=Swap(p->sourceHigh);return ((u32)high<<16)|low;}
u8 *check_frag(Packet*,int*);
// @fn check_frag
u8 *check_frag(Packet *packet,int *complete){u16 id;u32 header;Frag *unused=NULL,*f;u32 key;u32 flags,n,offset,size,last,i;u32 count;u32 end;u32 index;u32 covered;*complete=0;flags=Swap(packet->frag);if(!(flags&0x3fff))return (u8*)packet;id=packet->first;header=(id&15)*4;key=SourceKey(packet);id=packet->id;for(i=0,f=_021D4948;i<8;i++,f++){if(f->count&&f->key==key&&f->id==id)break;if(!f->count&&!unused)unused=f;}n=Swap(packet->total)-header;offset=flags&0x1fff;size=n+(offset<<3);if(i==8){if(!unused||size>4096)return NULL;f=unused;f->allocation=_021D4620[0].alloc(header+14+4096);if(!f->allocation)return NULL;f->key=key;f->id=id;f->end=0;f->tick=(u32)(OS_GetTick()>>16);f->data=f->allocation+14+header;MI_CpuCopy8(packet,f->allocation+14,header);}if(f->count==8||size>4096){f->count=0;_021D4620[0].free(f->allocation);return NULL;}last=offset+((n+7)>>3);if(!(flags&0x2000)){f->total=size;f->end=last;}f->start[f->count]=offset;f->stop[f->count]=last;f->count++;MI_CpuCopy8((u8*)packet+header,f->data+(offset<<3),n);end=f->end;if(!end)return NULL;count=f->count;covered=0;index=0;while(index<count){if(f->start[index]<=covered&&covered<f->stop[index]){covered=f->stop[index];index=0;}else index++;}if(covered<end)return NULL;{u8 *out=f->allocation;u16 total=f->total+((out[14]&15)*4);*(u16*)(out+16)=(total>>8)|(total<<8);f->count=0;*complete=1;return out+14;}}
