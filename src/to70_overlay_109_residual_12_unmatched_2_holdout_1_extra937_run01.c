#pragma force_active on
#include "global.h"
typedef struct {u32 pad;u8 flag:1;u8 value:7;} W;extern void ov109_021E7114(void *,u8,u8,u8,u8);void ov109_021E7178(void *,W *,int,u32,u8);
void ov109_021E7178(void *w,W *a,int b,u32 c,u8 d){u8 count=0;if(a)count=a->value+1;ov109_021E7114(w,count,(u8)b,(u8)((c*6+4)+(u8)(c>>1)),(u8)(d*5+1));}
