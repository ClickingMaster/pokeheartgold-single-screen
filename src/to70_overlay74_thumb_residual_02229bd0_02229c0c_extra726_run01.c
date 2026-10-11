#pragma force_active on
#include "global.h"
extern u16 Comm_GetLocalNetId(void);
extern int Comm_IsNetIdConnected(u16);
extern void sub_02034434(void),ov74_02229BC0(void(*)(void),int);
extern void*sub_02034818(int);
typedef struct{u8 p[0x28];void*profiles[16];u8 q[0x359];u8 flags[16];}Work;
extern Work*volatile ov74_0223D0A4;
void ov74_02229BD0(void);void ov74_02229C0C(void);
void ov74_02229BD0(void){int i;if(Comm_IsNetIdConnected(Comm_GetLocalNetId())){for(i=0;i<16;i++)ov74_0223D0A4->flags[i]=0;sub_02034434();ov74_02229BC0(ov74_02229C0C,0);}}
void ov74_02229C0C(void){int i;for(i=0;i<16;i++){if(ov74_0223D0A4->profiles[i]==0&&Comm_IsNetIdConnected((u16)i)==1)ov74_0223D0A4->profiles[i]=sub_02034818(i);}}
