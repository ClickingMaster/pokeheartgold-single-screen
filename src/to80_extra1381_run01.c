#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
typedef struct Dec{u8 sign,pad;s16 exponent;volatile u8 count;u8 digits[33];}Dec;
int __equals_dec(Dec*,Dec*);
int __less_dec(Dec*,Dec*);
int __equals_dec(Dec*a,Dec*b){int i,n;if(a->digits[0]==0)return b->digits[0]==0;if(b->digits[0]==0)return a->digits[0]==0;if(a->exponent==b->exponent){int bc;int ac=a->count;bc=b->count;n=ac; if(ac>bc)n=bc;for(i=0;i<n;i++){if(a->digits[i]!=b->digits[i])return 0;}if(n==ac)a=b;for(;i<a->count;i++)if(a->digits[i])return 0;return 1;}return 0;}
int __less_dec(Dec*a,Dec*b){int i,n;if(a->digits[0]==0)return b->digits[0]!=0;if(b->digits[0]==0)return 0;if(a->exponent==b->exponent){int bc;int ac=a->count;bc=b->count;n=ac;if(ac>bc)n=bc;for(i=0;i<n;i++){if(a->digits[i]<b->digits[i])return 1;if(b->digits[i]<a->digits[i])return 0;}if(n==ac&&i<bc){do{if(b->digits[i])return 1;i++;}while(i<b->count);}return 0;}return a->exponent<b->exponent;}
