#pragma force_active on
#include "global.h"
typedef struct {int a[4];} Table;extern Table ov01_02208A38;extern void sub_02023EE0(void*,int),sub_02023F40(void*,int),sub_02023F04(void*,int);
typedef struct{s8 state,a,b;u8 flag:1,other:7;} State;
void ov01_021F8374(void*,void*,State*,int);
void ov01_021F8374(void*unused,void*s,State*p,int i){Table table=ov01_02208A38;int v;v=table.a[i];if(i!=p->state){sub_02023EE0(s,v);sub_02023F40(s,0);p->flag=0;}else if(p->b){p->flag=1;}else if(p->flag==1){sub_02023EE0(s,v);sub_02023F40(s,0);p->flag=0;}else sub_02023F04(s,4096);}
