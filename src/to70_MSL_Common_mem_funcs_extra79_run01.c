#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
void __fill_mem(void *,int,u32);
void __fill_mem(void *dst,int value,u32 count) {u8 *out=dst; u32 word=(u8)value,n; if(count>=32) {n=-(u32)out&3; if(n) {count-=n;do {*out++=(u8)word;}while(--n);} if(word)word|=(word<<24)|(word<<16)|(word<<8); n=count>>5; if(n) {do {u32 *p=(u32 *)out;p[0]=word;p[1]=word;p[2]=word;p[3]=word;p[4]=word;p[5]=word;p[6]=word;p[7]=word;out+=32;}while(--n);} n=(count&31)>>2;if(n) {do {*(u32 *)out=word;out+=4;}while(--n);}count&=3;} if(count) {do {*out++=(u8)word;}while(--count);} }
