#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {volatile u8 id,value;} Pair;
typedef struct {u16 kind:4,level:4,value:8;} Ranking;
typedef struct {Ranking slots[3];u8 order[5],ties,count,other;} Result;
extern void sub_02032588(Ranking *,Pair *,int);
void sub_02031DA0(const u8 *,Result *);
void sub_02031DA0(const u8 *input,Result *out) {
 Pair pairs[5];Pair tmp;u8 count;int i,j;u8 minimum;
 MI_CpuFill8(out,0,14);MI_CpuFill8(pairs,0,10);count=0;
 for(i=0;i<5;i++){u8 value=input[i+3];pairs[i].id=i;pairs[i].value=value;if(value)count++;}
 if(count)for(i=0;i<5;i++)for(j=i+1;j<5;j++) {
 u8 b=pairs[j].value,a=pairs[i].value;if(a>b)continue;
 if(a==b && pairs[i].id<pairs[j].id)continue;
 tmp.id=pairs[i].id;tmp.value=pairs[i].value;
 pairs[i].id=pairs[j].id;pairs[i].value=pairs[j].value;
 pairs[j].id=tmp.id;pairs[j].value=tmp.value;
 }
 minimum=pairs[4].value;
 for(i=0;i<5;i++){out->order[i]=pairs[i].id;if(minimum==pairs[i].value)out->ties++;}
 out->count=count;
 switch(count) {
 case 0:out->slots[0].kind=6;return;
 case 1:sub_02032588(&out->slots[0],&pairs[0],0);out->slots[1].kind=6;break;
 case 2:sub_02032588(&out->slots[0],&pairs[0],0);sub_02032588(&out->slots[1],&pairs[1],1);out->slots[2].kind=6;break;
 case 3:sub_02032588(&out->slots[0],&pairs[0],0);sub_02032588(&out->slots[1],&pairs[1],1);sub_02032588(&out->slots[2],&pairs[2],2);break;
 case 4:case 5:
 if(pairs[0].value-pairs[3].value<=12) {
 out->slots[0].kind=5;out->slots[0].value=pairs[0].value;
 out->slots[1].kind=6;out->slots[1].value=pairs[1].value;
 out->slots[2].kind=6;out->slots[2].value=pairs[2].value;
 out->count=1;
 {u8 top=pairs[0].value;if(top>20)out->slots[0].level=3;
 else if(top>7)out->slots[0].level=2;
 else out->slots[0].level=1;}
 } else {
 sub_02032588(&out->slots[0],&pairs[0],0);sub_02032588(&out->slots[1],&pairs[1],1);
 if(count==4)sub_02032588(&out->slots[2],&pairs[4],2);
 else {out->slots[2].kind=5;out->slots[2].level=1;out->slots[2].value=pairs[2].value;}
 }
 break;
 }
 out->count=count;
}
