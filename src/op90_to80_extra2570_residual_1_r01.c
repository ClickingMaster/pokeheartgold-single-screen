#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 a,b,c,d,pad[2],words[16];} Block;
extern const u32 _02110D6C[],_02110CAC[];
#define ROT(x,s) (((x)<<(s))|((x)>>(32-(s))))
#define F(b,c,d) (((b)&(c))|(~(b)&(d)))
#define G(b,c,d) (((b)&(d))|((c)&~(d)))
#define H(b,c,d) (((b)^(c))^(d))
#define I(b,c,d) ((c)^((b)|~(d)))
#define STEP(f,a,b,c,d,x,k,s) {u32 t=(k)+((x)+((a)+f(b,c,d)));(a)=(b)+ROT(t,s);}
void ProcessBlock(Block*);
// @fn ProcessBlock
void ProcessBlock(Block*p){const u32*index;u32 a=p->a,b=p->b,c=p->c,d=p->d;u32*words=p->words;const u32*k=_02110D6C;u32*w=words;int i;for(i=0;i<4;i++){STEP(F,a,b,c,d,*w++,*k++,7);STEP(F,d,a,b,c,*w++,*k++,12);STEP(F,c,d,a,b,*w++,*k++,17);STEP(F,b,c,d,a,*w++,*k++,22);}
index=_02110CAC;for(i=0;i<4;i++){STEP(G,a,b,c,d,words[*index++],*k++,5);STEP(G,d,a,b,c,words[*index++],*k++,9);STEP(G,c,d,a,b,words[*index++],*k++,14);STEP(G,b,c,d,a,words[*index++],*k++,20);}
for(i=0;i<4;i++){STEP(H,a,b,c,d,words[*index++],*k++,4);STEP(H,d,a,b,c,words[*index++],*k++,11);STEP(H,c,d,a,b,words[*index++],*k++,16);STEP(H,b,c,d,a,words[*index++],*k++,23);}
for(i=0;i<4;i++){STEP(I,a,b,c,d,words[*index++],*k++,6);STEP(I,d,a,b,c,words[*index++],*k++,10);STEP(I,c,d,a,b,words[*index++],*k++,15);STEP(I,b,c,d,a,words[*index++],*k++,21);}p->a+=a;p->b+=b;p->c+=c;p->d+=d;}
