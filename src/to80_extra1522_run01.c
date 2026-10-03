#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
int ov43_0222BEEC(void*,int*,int,int*);int ov43_0222BD18(void*,int*,int);
int ov43_0222BFA4(void*p,int*q,int n);
int ov43_0222BFA4(void*p,int*q,int n){int found=0;int result=ov43_0222BEEC(p,q,n,&found);if(found){*q=1;return result;}return ov43_0222BD18(p,q,n);}
