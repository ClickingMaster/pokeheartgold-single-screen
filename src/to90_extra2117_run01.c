#pragma force_active on
// clang-format off
#include "global.h"
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern char ov45_02255120[];int printf(const char*,...);void abort(void);void*ov45_02242394(u32);void ov00_021EC2EC(int,void*,int);
typedef union {u32 raw;struct {u32 heap:1;u32 capacity:31;} h;struct {u8 heap:1;u8 smallSize:7;u8 rest[3];} small;} Header;
typedef struct {Header header;u32 length;u8*ptr;} Text;
void ov45_02237BFC(Text*w,u32 size);
void ov45_02237BFC(Text*w,u32 size){
 u32 len,capacity;u8*dst,*old;int wasHeap,makeHeap;
 if(size>0x7ffffffe){printf(ov45_02255120);abort();}
 wasHeap=w->header.h.heap!=0;if(wasHeap){capacity=w->header.raw>>1;len=w->length;}else{capacity=11;len=w->header.small.smallSize;}
 if(size<len)size=len;
 if(size+1<=11)size=11;else size=(size+16)&~15;
 if(size==capacity)return;
 if(size==11){dst=(u8*)w+1;old=w->ptr;makeHeap=0;}
 else{if(size>capacity)dst=ov45_02242394(size);else{dst=ov45_02242394(size);if(!dst)return;}old=wasHeap?w->ptr:(u8*)w+1;makeHeap=1;}
 memmove(dst,old,len);dst[len]=0;if(wasHeap&&old)ov00_021EC2EC(0,old,0);
 w->header.h.heap=makeHeap;
 if(!makeHeap)w->header.small.smallSize=(u8)len;else{w->length=len;w->ptr=dst;w->header.raw=(w->header.raw&1)|(size<<1);}
}
