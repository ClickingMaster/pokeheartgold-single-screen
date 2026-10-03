#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern const u8 _02110718[];extern const u32 _02110798[];extern void long_from_char(u32*,const u8*,u32);void CPSi_md5_block(u32*,const u8*);
#define R(x,n) (((x)<<(n))|((x)>>(32-(n))))
#define F(b,c,d) ((d)^((b)&((c)^(d))))
#define G(b,c,d) ((c)^((d)&((b)^(c))))
#define H(b,c,d) ((d)^((b)^(c)))
#define I(b,c,d) ((c)^((b)|~(d)))
#define STEP(f,a,b,c,d,n,j) {u32 t=(a)+(f(b,c,d)+words[_02110718[j]]+_02110798[j]);(a)=(b)+R(t,n);}
void CPSi_md5_block(u32*state,const u8*input){u32 a=state[0],b=state[1],c=state[2],d=state[3],words[16];int index,i;long_from_char(words,input,64);index=0;
for(i=0;i<4;i++){STEP(F,a,b,c,d,7,index+0);STEP(F,d,a,b,c,12,index+1);STEP(F,c,d,a,b,17,index+2);STEP(F,b,c,d,a,22,index+3);index+=4;}for(i=0;i<4;i++){STEP(G,a,b,c,d,5,index+0);STEP(G,d,a,b,c,9,index+1);STEP(G,c,d,a,b,14,index+2);STEP(G,b,c,d,a,20,index+3);index+=4;}for(i=0;i<4;i++){STEP(H,a,b,c,d,4,index+0);STEP(H,d,a,b,c,11,index+1);STEP(H,c,d,a,b,16,index+2);STEP(H,b,c,d,a,23,index+3);index+=4;}for(i=0;i<4;i++){STEP(I,a,b,c,d,6,index+0);STEP(I,d,a,b,c,10,index+1);STEP(I,c,d,a,b,15,index+2);STEP(I,b,c,d,a,21,index+3);index+=4;}state[0]+=a;state[1]+=b;state[2]+=c;state[3]+=d;}
