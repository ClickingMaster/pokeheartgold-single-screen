#pragma force_active on
#pragma force_active on
#include "global.h"
extern void ManagedSprite_SetAnimateFlag(void *,int);void ov96_02217820(void *);int ov96_0221785C(int);
void ov96_02217820(void *w){int i=0;int j,flag=0;u8 *group=w;do{j=0;u8*p=group;do{ManagedSprite_SetAnimateFlag(*(void**)(p+20),flag);j++;p+=4;}while(j<5);group+=232;i++;}while(i<4);ManagedSprite_SetAnimateFlag(*(void**)((u8*)w+12),0);}
