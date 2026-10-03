#pragma force_active on
#include "global.h"

void ov13_0222281C(int start,s8*out,int length,s8*key,int keylen);
void ov13_02222850(s8*in,u8*out,int length);

void ov13_0222281C(int start,s8*out,int length,s8*key,int keylen){int half=length/2,pos=start%keylen,i;for(i=0;i<half;i++){out[i]=i;out[i]^=key[pos++];if(pos>=keylen)pos=0;}}
