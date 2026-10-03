#pragma force_active on
#include "global.h"
extern void ov43_0222DD40(void*,int,int),ov43_0222DDA0(void*,int),ov43_0222DDF0(void*,int,void*,int,int),ov43_0222DE14(void*,int,int,int,int,int);extern int ov43_0222EDAC[],ov43_0222F10C[];extern u8 ov43_0222EE08[],ov43_0222F0D0[];
void ov43_0222D584(u8*w,int unused,int a,int b);
void ov43_0222D584(u8*w,int unused,int a,int b){int*r;u8*s;int j;int i;int *p;u8*q;ov43_0222DD40(w+248,a,b);ov43_0222DDA0(w,a);i=0;p=ov43_0222EDAC;q=ov43_0222EE08;w+=20;do{ov43_0222DDF0(w,a,q,*p,b);r=ov43_0222F10C;s=ov43_0222F0D0;j=0;do{ov43_0222DE14(w,a,j,*s,*r,b);r++;s++;j++;}while(j<8);w+=76;p++;q+=6;i++;}while(i<3);}
