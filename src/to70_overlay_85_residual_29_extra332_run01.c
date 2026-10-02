#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int state,a,b,c,d,id;} Entry;
void ov85_021E7FC0(Entry *,int);
void ov85_021E8008(u8 *,Entry *);
void ov85_021E7D78(u8 *);int ov85_021E7DA8(u8 *);void PlaySE(u16);
void ov85_021E7FC0(Entry *base,int id){int i=0;Entry *p;base=(Entry *)((u8 *)base+3000);p=base;do{if(p->state==1&&p->id==id){p->a=0;return;}i++;p++;}while(i<5);i=0;do{if(base->state==0){base->state=1;base->id=id;return;}i++;base++;}while(i<5);GF_ASSERT(FALSE);}

void ov85_021E8008(u8 *w,Entry *e){int *p=(int *)(w+212);switch(e->state){case 1:e->d=16384;e->c=0;e->state++;
case 2:p[13]-=4096;if(p[13]<=-32768)e->state++;break;
case 3:p[13]+=8192;if(p[13]>=0){p[13]=0;ov85_021E7D78(w);e->a=1;e->state++;PlaySE(1622);}break;
case 4:p[10]=e->d;e->d=-e->d;e->c++;if(!(e->c&3)){if(e->d<=4096){p[10]=0;e->state++;}else e->d>>=1;}break;
case 5:if(!ov85_021E7DA8(w)){e->b=0;e->a=0;e->state=0;PlaySE(1584);}break;}}
