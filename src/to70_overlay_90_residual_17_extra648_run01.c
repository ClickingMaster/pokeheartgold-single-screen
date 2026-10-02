#pragma force_active on
#include "global.h"
typedef struct{u32 x;u8 ids[4];u8 count;}Data;typedef struct{u8 a[4],b[4];void*objs[4];}Out;extern void*ov90_022588CC(Data*,int);extern u8 sub_020348A8(int),sub_020348CC(int);
void ov90_0225A204(Out*out,Data*d);
int ov90_0225A258(Data*d,u8*ids);
void ov90_0225A204(Out*out,Data*d){int i;for(i=0;i<d->count;i++){out->objs[i]=ov90_022588CC(d,d->ids[i]);GF_ASSERT(out->objs[i]);out->a[i]=sub_020348A8(d->ids[i]);out->b[i]=sub_020348CC(d->ids[i]);}}
