#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
typedef struct Format {u8 justify,sign,precisionSet,alternate,length,conversion;u8 pad[2];int width,precision;}Format;
char *long2str(s32,char*,Format);
typedef struct Dec {u8 sign;u8 pad;s16 exponent;u8 count;s8 digits[33];}Dec;
void round_decimal(Dec*,int);
void round_decimal(Dec*d,int n){s8 *p,*last,*first;int round;u8 count;if(n<0){zero:d->exponent=0;d->count=1;d->digits[0]='0';return;}count=d->count;if(n>=count)return;first=d->digits;p=(s8*)((u32)first+n);p=(s8*)((u32)p+1);{s8 digit=(s8)(*--p-'0');if(digit==5){last=first+count;do{--last;if(last<=p)break;}while(*last=='0');if(last==p)round=p[-1]&1;else round=1;}else round=digit>5;}while(n){s8 digit=(s8)(*--p-'0'+round);round=digit>9;if(round||digit==0)--n;else{*p=digit+'0';break;}}if(round){d->exponent++;d->count=1;d->digits[0]='1';return;}if(!n)goto zero;d->count=n;}
