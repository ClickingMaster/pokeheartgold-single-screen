#pragma force_active on
#include "global.h"
extern u8 ov86_021E81DC[];extern void ov86_021E6024(void*,int,int,int,int,int,int,int);
void ov86_021E7598(void*);
void ov86_021E7598(void*w){u16 i;int value,x;for(i=0;i<26;i++){value=ov86_021E81DC[i];x=value%7;ov86_021E6024(w,4,i+56,(x<<5)+24,(value/7)<<5,4,0xf0100,2);}}
