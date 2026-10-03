#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Queue{u32 a;volatile u32 b;void**base;u32 start;u32 count,capacity;}Queue;extern void ov00_021EC2EC(int,void*,int),ov45_02238508(void*);Queue*ov45_02236AE8(Queue*);
Queue*ov45_02236AE8(Queue*p){void**end,**cursor,**base,**limit;u32 capacity,start;u32 index=0;volatile u8 tag=0;p->b=0;if(p->count){if(p->capacity)index=(p->start+((volatile Queue*)p)->count)%p->capacity;base=p->base;start=p->start;capacity=((volatile Queue*)p)->capacity;end=base+index;cursor=base+start;limit=base+capacity;while(cursor!=end){if(*cursor)ov00_021EC2EC(0,*cursor,0);if(++cursor==limit)cursor=base;}}ov45_02238508(&p->base);return p;}
