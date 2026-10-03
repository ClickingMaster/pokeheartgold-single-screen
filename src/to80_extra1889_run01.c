#pragma force_active on
#include "global.h"
#pragma force_active on
extern u16 *sub_02014A4C(void *);
extern void sub_02014A78(void *,int);
extern u8 ov90_0225C31C[][12];
typedef struct {u8 p[0x304];void *obj;} Work;
typedef struct {u8 p[0x18];u16 row;u8 pad,col;s32 value;} Entry;
void ov90_0225A134(Work *w,Entry *e);
void ov90_0225A134(Work *w,Entry *e){u16 *data;int i,stride;if(!w->obj)GF_AssertFail();data=sub_02014A4C(w->obj);stride=12;for(i=0;i<48;i++){int offset=i-1;int index=offset+ov90_0225C31C[e->row-1][e->col]*8;if(index<0)index+=192;data[index]=e->value>>12;}sub_02014A78(w->obj,1);}
