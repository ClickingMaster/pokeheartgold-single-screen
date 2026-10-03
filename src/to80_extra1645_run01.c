#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define U16(p,o) (*(u16*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{u8*data;u32 size,capacity;} Buffer;
extern void ov45_02237890(Buffer*,u32,u32),ov45_02238820(Buffer*,u32);
void ov45_0223795C(Buffer*,u32);
void ov45_0223795C(Buffer*b,u32 n){if(n>b->capacity || b->size>b->capacity-n){volatile u8 zero=0;(void)&zero;ov45_02237890(b,b->size+n-b->capacity,0);}else{memset(b->data+b->size,0,n);b->size+=n;return;}ov45_02238820(b,n);}
