#pragma force_active on
#include "global.h"
int ov40_0222DA00(int *,int *,int,int);
int ov40_0222DA00(int *a,int *b,int reverse,int screen){int done=1;if(reverse==0){if(*a<8){++*a;done=0;}if(*b>7){*b-=3;done=0;}}else{if(*a>0){--*a;done=0;}if(*b<31){*b+=3;done=0;}}if(screen==0){*(volatile u16*)0x4000052=*a|(*b<<8);*(volatile u16*)0x4001052=*a|(*b<<8);}if(screen==1)*(volatile u16*)0x4000052=*a|(*b<<8);if(screen==2)*(volatile u16*)0x4001052=*a|(*b<<8);return done;}
