#pragma force_active on
#include "global.h"
typedef struct{u16 code;s8 character,pad;} Mapping;
extern s8 ov75_02249ACC[],ov75_02249AD0[];extern Mapping ov75_02249ADC[];
extern void *Heap_Alloc(int,int);extern void CopyStringToU16Array(void *,u16 *,int),Heap_Free(void *);
void ov75_02249780(void *,s8 *,int);
void ov75_02249780(void *string,s8 *out,int heap){int offset=0,i=0;u16 *text;u32 j;do{out[offset++]=ov75_02249ACC[i++];}while(ov75_02249ACC[i]!=0);text=Heap_Alloc(heap,200);CopyStringToU16Array(string,text,100);for(i=0;;i++){if(text[i]==65535){out[offset]=0;break;}for(j=0;j<10;j++){if(ov75_02249ADC[j].code==text[i]){out[offset]=ov75_02249ADC[j].character;break;}}if(j>=10)GF_AssertFail();offset++;}i=0;do{out[offset++]=ov75_02249AD0[i++];}while(ov75_02249AD0[i]!=0);out[offset]=0;Heap_Free(text);}
