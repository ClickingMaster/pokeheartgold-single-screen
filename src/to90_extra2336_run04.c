#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {void *allocator;u32 unused;volatile u32 size;} Container;
extern void ov00_021EC2EC(int,void*,int);
Container *ov45_02238508(Container*);
Container *ov45_02238508(Container *s){if(s->allocator){u32 start=s->size;struct {u8 control;} local={0};s->size=s->size-start;(void)local;if(s->allocator)ov00_021EC2EC(0,s->allocator,0);}return s;}
