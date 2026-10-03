#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *sub_0202C6F4(void*);extern int sub_0202C2DC(void*,int);
int ov43_0222B574(int*a,void**b);
int ov43_0222B574(int*a,void**b){int i;void*p;if(*a==1){p=sub_0202C6F4(b[1]);for(i=0;i<32;i++){if(sub_0202C2DC(p,i)==0)return 1;}}else return 1;return 0;}
