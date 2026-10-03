#pragma force_active on
#include "global.h"
typedef struct {int timer;u16 msg,flag;int mode;}C;
typedef struct {s16 x,y;}P;
typedef struct {s16 x,y,dir,flag,mode;}T;
extern void*ov49_02259FF0(void*),*ov49_02258D70(void*,int),*ov49_0225EF84(void*),*ov49_02259FE8(void*),*ov49_0225EF40(void*,int),*ov49_0225A010(void*);extern T*ov49_02259FEC(void*);extern P ov49_02258E34(void*);
extern int ov49_0225A040(void*),ov49_0225EF88(void*),ov49_02260C58(int),ov45_0222A330(void*),ov45_0222A208(void*),ov45_0222EBF0(int),ov45_0222EB38(int),ov45_0222EB74(void),ov45_0222AB68(void*),ov45_0222AB78(void*,int),ov49_02260CC0(int),ov49_02260D28(int),ov49_02258E60(void*,int),ov42_022282A4(int),ov49_02258F38(void*),ov49_0225A0AC(void*);
extern void ov49_0225EF8C(void*,int),ov49_0225EF90(void*),ov45_0222AC14(void*,int,int,int,int,int,int,int),ov49_0225A034(void*,int),ov49_0225A038(void*,int),ov45_0222A5E8(void*,int),ov45_0222B118(void*,int),ov49_0225EF68(void*),ov49_02258EEC(void*,void*,int),ov49_02258EAC(void*,void*,int,int),ov49_0225A37C(void*,int,int),ov49_0225A08C(void*,void*),ov49_0225A0EC(void*),ov49_0225EF98(void*,int,const void*,int);extern void*ov49_0225A30C(void*,int,int);extern const u8 ov49_02269B38[];
int ov49_02260428(void*,void*,int);
int ov49_02260428(void*owner,void*ui,int index){void*map=ov49_02259FF0(ui);void*mon=ov49_02258D70(map,index);void*net;int obj=ov49_0225A040(ui);C*c=ov49_0225EF84(owner);net=ov49_02259FE8(ui);
switch(ov49_0225EF88(owner)){
case 0:{u8 kind;c=ov49_0225EF40(owner,12);switch((u8)ov49_02260C58(obj)){case 0:c->mode=3;break;case 1:c->mode=4;break;case 2:c->mode=5;break;case 3:c->mode=6;break;default:GF_AssertFail();c->mode=5;break;}if(ov45_0222A330(net)==1){c->msg=22;c->flag=0;ov49_0225EF8C(owner,5);}else if(ov45_0222A208(net)==1){c->msg=72;c->flag=1;ov49_0225EF8C(owner,5);}else{kind=ov49_02260C58(obj);if(!ov45_0222EBF0(kind)){c->msg=20;c->flag=0;ov49_0225EF8C(owner,3);}else{if(!ov45_0222EB38(kind))GF_AssertFail();ov49_0225EF90(owner);}}break;}
case 1:{int result=ov45_0222EB74();if(result==1){void*n;u8 value;ov49_0225EF8C(owner,2);{u8 p[4]={0,0,0,0};n=ov49_02259FE8(ui);value=ov45_0222AB68(n);if((u32)(c->mode-5)<=1){int i;u8 count=0;for(i=0;i<4;i++){int idx=ov45_0222AB78(n,i);if(idx!=-1){if(count<4)p[count]=idx;count++;}}}else p[0]=index;ov45_0222AC14(n,c->mode,value,p[0],p[1],p[2],p[3],0);}}else if(result==2){c->msg=20;c->flag=0;ov49_0225EF8C(owner,3);}break;}
case 2:{int outA=ov49_02260CC0(obj),outB=ov49_02260D28(obj);int n,dir;T*t;P p;ov49_0225A034(ui,1);ov49_0225A038(ui,(u8)outA);ov45_0222A5E8(ov49_02259FE8(ui),outB);switch(c->mode){case 3:case 4:n=5;break;case 5:n=3;break;case 6:n=4;break;default:GF_AssertFail();break;}ov45_0222B118(net,n);dir=ov42_022282A4(ov49_02258E60(mon,6));t=ov49_02259FEC(ui);p=ov49_02258E34(mon);t->flag=1;t->x=p.x/16;t->y=p.y/16;t->dir=dir;t->mode=c->mode;ov49_0225EF68(owner);return 1;}
case 3:{int n;switch((u8)ov49_02258E60(mon,6)){case 0:n=6;break;case 1:n=5;break;case 2:n=8;break;case 3:n=7;break;default:GF_AssertFail();break;}ov49_02258EEC(map,mon,n);ov49_0225EF90(owner);break;}
case 4:if(ov49_02258F38(mon)==1){c->timer=8;ov49_0225EF8C(owner,7);}break;
case 5:{u8 dir=ov42_022282A4((u8)ov49_02258E60(mon,6));ov49_02258EAC(map,mon,2,dir);ov49_0225EF90(owner);break;}
case 6:if(!ov49_02258E60(mon,5)){c->timer=8;ov49_0225EF8C(owner,7);}break;
case 7:if(--c->timer<=0){ov49_0225A37C(ui,c->mode,0);ov49_0225A08C(ui,ov49_0225A30C(ui,c->flag,c->msg));ov49_0225EF90(owner);}break;
case 8:if(ov49_0225A0AC(ui)){ov49_0225EF68(owner);ov49_02258EEC(map,mon,1);ov49_0225A0EC(ui);ov49_0225EF98(ov49_0225A010(ui),index,ov49_02269B38,0);}break;}
return 0;}
