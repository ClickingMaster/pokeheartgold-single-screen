#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef union{struct{u32 large:1,other:31;u32 size;char*ptr;}heap;struct{u8 header;char data[11];}small;}W30615String;
typedef struct{u8 pad[0xb8];W30615String name;u32 unusedC4,a,b;u8 padD0[0x18];u8 collection[0x5c];s32 count;u32 active;}W30615Context;extern W30615Context*_022577C4;extern void ov45_022320B4(W30615String*,void*),ov45_02237BC8(W30615String*);extern int ov45_02240A80(char*),ov45_0223E4A8(void*);int ov45_02234688(void);
static inline int enough(s32 n){int result;if(n>=4)result=1;else result=0;return result;}
#pragma opt_propagation off
int ov45_02234688(void){W30615String local;int empty;W30615Context*c;ov45_022320B4(&local,&_022577C4->name);empty=ov45_02240A80(!local.heap.large?local.small.data:local.heap.ptr)==0;ov45_02237BC8(&local);if(empty)return 0;c=_022577C4;if(c->a==0)return 0;if(c->b==0)return 0;if(c->active==0)return 0;if(!enough(c->count))return 0;return ov45_0223E4A8(c->collection)!=0;}
