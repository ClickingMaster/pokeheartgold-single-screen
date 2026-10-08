#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern signed char *ov00_02216BA8;
int ov00_021EBE08(const u8*,u32,char*,u32);
// @fn ov00_021EBE08
int ov00_021EBE08(const u8 *src,u32 n,char *dst,u32 cap){int count;const u8 *next,*end;char *nextOut;char*out;struct {u8 first,second,third;} in;u32 len=(n/3)*4+(n%3?4:0);if(!dst)return len;if(cap<len)return -1;next=src;end=src+n;nextOut=dst;while(next!=end){int rem=end-next;int bits=rem*8;count=bits/6+(bits%6!=0);if(rem>=3)rem=3;MI_CpuFill8((void*)&in,0,3);out=nextOut;MI_CpuCopy8(next,(void*)&in,rem);out[0]=ov00_02216BA8[in.first>>2];if(count<2)out[1]='*';else{int a=in.first,b=in.second;out[1]=ov00_02216BA8[((a<<4)&63)|(b>>4)];}if(count<3)out[2]='*';else{int a=in.second,b=in.third;out[2]=ov00_02216BA8[((a<<2)&63)|(b>>6)];}if(count<4)out[3]='*';else{int a=in.third;out[3]=ov00_02216BA8[a&63];}next+=rem;nextOut+=4;}return nextOut-dst;}
