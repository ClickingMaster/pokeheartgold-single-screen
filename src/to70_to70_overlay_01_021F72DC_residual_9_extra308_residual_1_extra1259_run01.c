#pragma force_active on
#include "global.h"
typedef struct {int a[4];} Table;extern Table ov01_02208A38;extern void BillboardObject_SetSequence(void*,int),BillboardObject_SetSequenceFrame(void*,int),BillboardObject_AdvanceFrame(void*,int);
typedef struct{s8 state,a,b;u8 flag:1,other:7;} State;
void ov01_021F8374(void*,void*,State*,int);
void ov01_021F8374(void*unused,void*s,State*p,int i){Table table=ov01_02208A38;int v;v=table.a[i];if(i!=p->state){BillboardObject_SetSequence(s,v);BillboardObject_SetSequenceFrame(s,0);p->flag=0;}else if(p->b){p->flag=1;}else if(p->flag==1){BillboardObject_SetSequence(s,v);BillboardObject_SetSequenceFrame(s,0);p->flag=0;}else BillboardObject_AdvanceFrame(s,4096);}
