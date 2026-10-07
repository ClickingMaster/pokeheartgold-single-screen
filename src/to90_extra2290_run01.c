#pragma force_active on
#include "global.h"
#pragma force_active on
extern void*Heap_Alloc(int,u32);void*ov74_022369A8(void*,u32);
#pragma opt_propagation off
void*ov74_022369A8(void*unused,u32 size){void*raw;u32 result;size+=36;size&=~31u;size+=32;raw=Heap_Alloc(84,size);result=(u32)raw+32;result&=~31u;*(void**)(result-4)=raw;return(void*)result;}
