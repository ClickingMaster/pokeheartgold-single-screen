#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define U16(p,o) (*(u16*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{u8 byte;} Allocator;typedef struct{void*data;volatile u32 size;u32 capacity;} Buffer;
extern void ov00_021EC2EC(int,void*,int),ov45_0223CE30(Buffer*,u32,Allocator);
void ov45_0223BD24(Buffer*,Allocator,u32);
void ov45_0223BD24(Buffer*b,Allocator incoming,u32 n){u32 size=b->size;Allocator old={0};volatile Allocator copy;(void)incoming;(void)old;b->size-=size;if(b->data){if(b->data)ov00_021EC2EC(0,b->data,0);b->data=0;b->capacity=0;}Allocator fresh={0};copy.byte=0;ov45_0223CE30(b,n,fresh);}
