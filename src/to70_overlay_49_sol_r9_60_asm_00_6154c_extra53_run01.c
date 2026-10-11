#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov49_02269B48[],ov49_02269B50[],ov49_02269B58[],ov49_02269B80[];
extern void *WiFiPlazaRoom_GetLobby(void*),*WiFiPlazaRoom_GetObjManager(void*),*ov49_0225A010(void*),*ov49_02258D70(void*,int),*ov49_0225EF84(void*),*ov49_0225EF40(void*,int);
extern void ov49_02258DAC(void*),ov49_0225EF8C(void*,int),ov49_0225EF68(void*),ov49_02258EEC(void *, void *, int),ov49_0225A06C(void*,u8,int),ov49_0225A04C(void*,u8,int),ov49_0225EF98(void*,int,const void*,void*),ov49_02259130(void*,int),ov49_0225EFC4(void*,int,const void*,void*);
extern int ov49_0225EF88(void*),ov45_0222A25C(void*,int),ov45_0222A374(void*),ov45_0222ADA8(void*,int),ov45_0222AD58(void*,int);
extern void ov45_0222A2A0(void*,int);
extern void *WiFiPlazaWork_GetPlayer(void*,int);
extern int ov45_0222A920(void*),ov49_02261DBC(void*,void*,void*,void*,u8,int);
int ov49_02261720(void*,void*,int);
int ov49_02261720(void*a,void*ctx,int id){void *entry,*work,*table,*messages,*net;net=WiFiPlazaRoom_GetLobby(ctx);table=WiFiPlazaRoom_GetObjManager(ctx);messages=ov49_0225A010(ctx);entry=ov49_02258D70(table,id);ov49_02258DAC(table);work=ov49_0225EF84(a);switch(ov49_0225EF88(a)){case 0:work=ov49_0225EF40(a,8);ov49_0225EF8C(a,1);case 1:if(ov45_0222A25C(net,id)){ov49_0225EF68(a);ov49_02258EEC(table, (void*)(entry),0);ov49_0225A06C(ctx,(u8)id,1);ov49_0225A04C(ctx,(u8)id,1);ov49_0225EF98(messages,id,ov49_02269B48,0);break;}ov45_0222A2A0(net,id);if(ov45_0222A374(net)==1&&ov45_0222ADA8(net,id)!=-1){ov49_02258EEC(table, (void*)(entry),0);ov49_0225A06C(ctx,(u8)id,1);ov49_0225A04C(ctx,(u8)id,1);ov49_02259130(entry,0);ov49_0225EF68(a);ov49_0225EF98(messages,id,ov49_02269B50,work);break;}if(ov49_02261DBC(work,net,ctx,entry,(u8)ov45_0222A920(WiFiPlazaWork_GetPlayer(net,id)),0)){ov49_02258EEC(table, (void*)(entry),0);ov49_0225A06C(ctx,(u8)id,1);ov49_0225A04C(ctx,(u8)id,1);ov49_02259130(entry,0);ov49_0225EFC4(messages,id,ov49_02269B58,work);break;}if(ov45_0222AD58(net,id)==1){ov49_02258EEC(table, (void*)(entry),0);ov49_0225A04C(ctx,(u8)id,1);ov49_0225EFC4(messages,id,ov49_02269B80,0);}break;}return 0;}
