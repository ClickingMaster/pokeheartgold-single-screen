#pragma force_active on
#include "global.h"
void ov75_02249884(s8*s,int*out);
void ov75_02249884(s8*s,int*out){int i,j,k=0;s8*p;for(i=0;i<4;i++){*out=0;for(j=0;j<4;j++){while((p=s+k,*p<48)||*p>57){if(*p==0){GF_ASSERT(*p!=0);break;}k++;}*out*=10;*out+=*p-48;k++;}out++;}}
