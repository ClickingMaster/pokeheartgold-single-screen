#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *ov49_02259FE8(void *),*ov49_02259FF0(void *),*ov49_0225EF84(void *),*ov49_0225A010(void *),*ov49_02258D70(void *,int);
extern int ov49_0225EF88(void *),ov45_0222A230(void *,int),ov45_0222A550(void *,int),ov45_0222AADC(void),ov49_02258F38(void *);
extern void ov49_0225EF40(void *,int),ov49_0225EF8C(void *,int),ov49_022591C0(void *,int),ov49_02258D54(void *);
extern void *WiFiPlazaWork_GetPlayer(void *,int);
extern int WiFiPlazaPlayer_GetAvatar(void *);
extern void *ov49_02258CB8(void *,int,int);
extern u32 ov45_0222A920(void *);
extern int ov49_02261DBC(void *,void *,void *,void *,u8,int);
extern void ov49_02258EEC(void *, void *, int),ov49_0225A06C(void *,u8,int),ov49_0225A04C(void *,u8,int),ov49_02259130(void *,int);
extern const u8 ov49_02269B58[],ov49_02269B40[];
extern void ov49_0225EFC4(void *,int,const void *,void *),ov49_0225EF90(void *),ov49_0225EF68(void *),ov49_0225EF98(void *,int,const void *,int);
int ov49_0226154C(void *,void *,int);
int ov49_0226154C(void *p,void *ctx,int idx){
 void *a=ov49_02259FE8(ctx),*b=ov49_02259FF0(ctx),*slot=ov49_02258D70(b,idx),*d=ov49_0225EF84(p),*e=ov49_0225A010(ctx);
 switch(ov49_0225EF88(p)){
 case 0:ov49_0225EF40(p,8);ov49_0225EF8C(p,1);
 case 1:{int x=ov45_0222A230(a,idx);int y=ov45_0222A550(a,idx);if(x==1 || y==1)ov49_0225EF8C(p,2);break;}
 case 2:{void *x=WiFiPlazaWork_GetPlayer(a,idx);if(!x){ov49_0225EF8C(p,1);break;}
  if(ov45_0222AADC()==1){void *y=ov49_02258CB8(b,idx,WiFiPlazaPlayer_GetAvatar(x));if(y){ov49_022591C0(y,0);ov49_0225EF8C(p,3);}}break;}
 case 3:{void *x=WiFiPlazaWork_GetPlayer(a,idx);if(!x){if(slot)ov49_02258D54(slot);ov49_0225EF8C(p,1);break;}
  if(ov49_02261DBC(d,a,ctx,slot,(u8)ov45_0222A920(x),1)){
   ov49_0225EF8C(p,5);ov49_02258EEC(b, (void*)(slot),0);ov49_0225A06C(ctx,(u8)idx,1);ov49_0225A04C(ctx,(u8)idx,1);ov49_02259130(slot,0);ov49_0225EFC4(e,idx,ov49_02269B58,d);
  }else{ov49_02258EEC(b, (void*)(slot),4);ov49_0225EF8C(p,4);ov49_0225A04C(ctx,(u8)idx,1);ov49_0225A06C(ctx,(u8)idx,1);ov49_022591C0(slot,1);}break;}
 case 4:if(ov49_02258F38(slot)==1)ov49_0225EF90(p);break;
 case 5:ov49_0225EF68(p);ov49_02258EEC(b, (void*)(slot),2);ov49_0225EF98(e,idx,ov49_02269B40,0);ov49_0225A04C(ctx,(u8)idx,0);ov49_0225A06C(ctx,(u8)idx,0);break;
 }
 return 0;
}
