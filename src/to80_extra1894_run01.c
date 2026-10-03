#pragma force_active on
#include "global.h"
#pragma force_active on
void ov96_021F6B50(int size,int amount,int*out);
void ov96_021F6B50(int size,int amount,int*out){int negative=-amount,n=127-size,i;int*p;i=0;if(size>0){p=out;do{int v=(u8)(i+1);*p++=-(negative*v/(size+1));i++;}while(i<size);}for(i=0;i<n;i++)out[(u8)(size+1+i)]=-((u8)(n-i)*negative/(n+1));}
