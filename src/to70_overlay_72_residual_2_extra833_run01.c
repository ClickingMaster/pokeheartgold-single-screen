#pragma force_active on
#include "global.h"
typedef struct{u8 p[24];u8 a,b,c;} State;extern u8 ov72_0223B834[],ov72_0223B91B[],ov72_0223B82C[],ov72_0223B578[],ov72_0223B923[];extern State ov72_0223B900;extern int _0223B820[2];extern void ov38_0221BE84(void),ov38_0221BFEC(void);extern int ov72_02237D50(void *,void *,int,void *,int);void ov72_02237C30(int,int,int,void *);
void ov72_02237C30(int a,int b,int c,void *data){u8 *dst,*src;u32 n;ov38_0221BE84();memcpy(ov72_0223B834,data,228);ov72_0223B900.a=a-1;ov72_0223B900.b=b-1;ov72_0223B900.c=c;dst=ov72_0223B91B;src=ov72_0223B82C;n=8;do{*dst++=*src++;}while(--n);if(ov72_02237D50(ov72_0223B578,ov72_0223B834,239,ov72_0223B923,2)){_0223B820[0]=6;}else{_0223B820[0]=12;_0223B820[1]=-5;ov38_0221BFEC();}}
