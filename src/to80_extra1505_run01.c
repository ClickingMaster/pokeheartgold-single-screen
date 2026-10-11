#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void *EasyChatApp_GetMessageString(void*,int);void *String_cstr(void*);
void ov102_021EAC44(void*p,void*q);
void ov102_021EAC44(void*p,void*q){W(p,0)=(u32)EasyChatApp_GetMessageString(PTR(q,4),35);W(p,4)=(u32)String_cstr(PTR(p,0));if(H(PTR(p,4),0)==0xfffe)W(p,8)=1;else W(p,8)=0;}
