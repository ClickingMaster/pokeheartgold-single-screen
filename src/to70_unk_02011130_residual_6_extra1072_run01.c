#pragma force_active on
#include "global.h"
typedef struct {int a,b,c,d;} Params;extern u16 *sub_02010EE0(void*,int);extern void sub_02010A00(int,int*,int,int);extern int sub_02010A7C(int,int);

void sub_02011D08(void*w);

void sub_02011D08(void*w){Params*p=w;int values[192],i;u16*out=sub_02010EE0(w,0);sub_02010A00(p->d/128,values,192,0);for(i=0;i<192;i++){out[0x180+i]=sub_02010A7C(128,-values[i]);out[0x240+i]=sub_02010A7C(128,values[i]);}}
