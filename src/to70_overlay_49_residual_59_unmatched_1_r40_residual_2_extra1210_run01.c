#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct {s16 x,y;}P;
extern void*ov49_0225EF84(void*),*ov49_0225EF40(void*,int),*ov49_0225A010(void*),*ov49_02259FE8(void*),*ov49_02259FF0(void*),*ov49_0225A000(void*),*ov49_02259FF8(void*),*ov49_02258DB0(void*),*ov49_02258DAC(void*),*ov49_02258D70(void*,int),*ov49_02258F40(void*,void*);
extern P ov49_02258E34(void*),ov42_02228270(P,int);
extern int ov49_0225EF88(void*),ov45_0222A330(void*),ov45_0222A3A0(void*),ov45_0222A2F8(void*),ov49_02258F3C(void*),ov49_02258E60(void*,int),ov49_022589C4(void*,u16,u16),ov49_0225A4E0(void*),ov49_0225A084(void*,u8),ov49_0225A500(void*),ov49_02258A30(int),ov49_0225A55C(void*),ov45_0222B00C(void*),ov49_0225A064(void*,u8),ov49_02258A90(int),ov49_02258A70(int),ov49_0225F224(int),ov49_02258A50(int),ov49_0225A4F0(void*),ov45_0222A53C(void*);
extern void ov49_0225EF90(void*),ov49_0225A53C(void*,int),ov49_02258EEC(void *, void *, int),ov49_0225A4D0(void*),ov49_0225F260(void*,void*,int,const void*,int),ov49_0225A03C(void*,u8),ov49_0225EFC4(void*,int,const void*,int),ov49_0225E58C(void*,u8,u8),ov49_0225A428(void*,int,int),PlaySE(int);
extern const u8 ov49_02269D20[],ov49_02269B80[],ov49_02269B70[],ov49_02269B60[],ov49_02269C90[],ov49_02269C60[];
int ov49_0225FDCC(void*,void*,int);
int ov49_0225FDCC(void*owner,void*ui,int index){int*ctx=ov49_0225EF84(owner);switch(ov49_0225EF88(owner)){
case 0:ov49_0225EF40(owner,4);ov49_0225EF90(owner);break;
case 1:{int status;void*mon;int oldtile;void*extra,*data,*net,*obj;int ny,nx,dir,newtile,cur;void*map,*other;P old,newp;
ov49_0225A53C(ui,0);obj=ov49_0225A010(ui);net=ov49_02259FE8(ui);map=ov49_02259FF0(ui);data=ov49_0225A000(ui);extra=ov49_02259FF8(ui);other=ov49_02258DB0(map);
if(other){if(!ov45_0222A330(net)&&ov45_0222A3A0(net)==1&&!ov45_0222A2F8(net)){if(ov49_02258F3C(other)!=9)ov49_02258EEC(map, (void*)(other),9);}else if(ov49_02258F3C(other)!=0)ov49_02258EEC(map, (void*)(other),0);}
mon=ov49_02258DAC(map);status=ov49_02258E60(mon,5);dir=ov49_02258E60(mon,6);old=ov49_02258E34(mon);newp=ov42_02228270(old,dir);oldtile=ov49_022589C4(data,(u16)(old.x/16),(u16)(old.y/16));ny=newp.y/16;nx=newp.x/16;newtile=ov49_022589C4(data,(u16)nx,(u16)ny);cur=ov49_0225A4E0(ui);if(cur!=index&&ov49_0225A084(ui,(u8)cur))ov49_0225A4D0(ui);
if(status){if(ov49_0225A500(ui)==1&&(u32)(status-1)<=2){cur=ov49_0225A4E0(ui);if(cur!=index){other=ov49_02258D70(map,cur);if(other&&!ov49_02258F3C(other))ov49_02258EEC(map, (void*)(other),*ctx);}ov49_0225A4D0(ui);}break;}
ov49_0225A53C(ui,0);if(ov49_02258A30(oldtile)==1){ov49_0225F260(owner,ui,index,ov49_02269D20,oldtile);ov49_02258EEC(map, (void*)(mon),0);ov49_0225A03C(ui,(u8)oldtile);ov49_0225A53C(ui,1);return 0;}
if(ov49_0225A55C(ui)==1){ov49_02258EEC(map, (void*)(mon),0);ov49_0225EFC4(obj,index,ov49_02269B80,0);ov49_0225A53C(ui,1);return 0;}
if(ov45_0222B00C(net)){ov49_0225EFC4(obj,index,ov49_02269B70,0);ov49_02258EEC(map, (void*)(mon),0);ov49_0225A53C(ui,1);return 0;}
if(gSystem.newKeys&1){void*o=ov49_02258F40(map,mon);if(o){int s=ov49_02258E60(o,5);int p=ov49_02258E60(o,4);if(p!=254){int available=ov49_0225A064(ui,(u8)p);if(!s&&!available){ov49_0225EFC4(obj,index,ov49_02269B60,0);ov49_02258EEC(map, (void*)(mon),0);ov49_02258EEC(map, (void*)(o),0);ov49_0225A53C(ui,1);return 0;}}}
if(ov49_02258A90(newtile)==1&&!ov49_02258E60(mon,6)){ov49_0225E58C(extra,(u8)nx,(u8)ny);ov49_0225A03C(ui,(u8)newtile);ov49_0225A53C(ui,1);return 0;}
if(ov49_02258A70(newtile)==1){ov49_0225F260(owner,ui,index,ov49_02269C90,newtile);ov49_02258EEC(map, (void*)(mon),0);ov49_0225A03C(ui,(u8)newtile);ov49_0225A53C(ui,1);return 0;}}
if(ov49_0225F224(dir)==1){void*o;if(!dir&&ov49_02258A50(newtile)==1){ov49_0225F260(owner,ui,index,ov49_02269C60,newtile);ov49_02258EEC(map, (void*)(mon),0);ov49_0225A03C(ui,(u8)newtile);ov49_0225A53C(ui,1);return 0;}o=ov49_02258F40(map,mon);if(o){int p=ov49_02258E60(o,4);if(p!=254&&!ov49_0225A4F0(ui)&&!ov49_0225A084(ui,(u8)p)){ov49_0225A428(ui,p,1);*ctx=ov49_02258F3C(o);ov49_02258EEC(map, (void*)(o),0);return 0;}}}
if(gSystem.newKeys&1024){if(!ov49_0225A4F0(ui)){ov49_0225A428(ui,ov45_0222A53C(net),0);PlaySE(1500);}else{int current=ov49_0225A4E0(ui);if(current==index){ov49_0225A4D0(ui);PlaySE(1500);}}return 0;}break;}}
return 0;}
