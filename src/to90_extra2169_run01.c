#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
typedef struct {u32 unknown;void *mem;u32 other,count,state;} Global;
extern Global ov74_0223D0A8;extern u8 gSystem[];
void ov74_0222CEC0(void),sub_0203A930(int),sub_02034D8C(void),WirelessStatusIcon_Create(void);int ov74_022368D4(void),sub_02034DB8(void),ov74_022365FC(void);u32 ov74_02236980(void);void*Heap_Alloc(int,u32);void ov74_0222CE6C(void*);void ov74_02236680(void*,void(*)(void*),void*);
int WM_GetLinkLevel(void);
void ov74_0222CEE0(void*);
void ov74_0222CEE0(void*p){u32 state;if(ov74_0223D0A8.state==44){ov74_0222CEC0();if(ov74_022368D4())ov74_0223D0A8.state=45;else ov74_0223D0A8.state=46;}state=ov74_0223D0A8.state;if(state==37||state==38||state==39||state==40||state==41||state==43)sub_0203A930(3-WM_GetLinkLevel());switch(ov74_0223D0A8.state){case 37:sub_02034D8C();ov74_0223D0A8.state=38;break;case 38:if(sub_02034DB8()==1){void *mem;ov74_0223D0A8.count=0;mem=Heap_Alloc(0x54,ov74_02236980());ov74_0223D0A8.mem=mem;ov74_02236680((u8*)p+0x5d4,ov74_0222CE6C,mem);ov74_0223D0A8.state=39;WirelessStatusIcon_Create();}break;case 41:ov74_0223D0A8.count++;break;case 47:return;case 39:case 40:case 42:case 43:case 44:case 45:case 46:case 48:break;}if(W(gSystem,0x48)&2){switch(ov74_0223D0A8.state){case 40:case 41:case 48:if(ov74_022365FC())ov74_0223D0A8.state=43;break;case 42:case 43:case 44:case 45:case 46:case 47:break;}}}
