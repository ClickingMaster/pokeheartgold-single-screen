#pragma force_active on
#include "global.h"
extern void ov74_0222FFFC(void*),ov74_02231070(int),ov74_022314A0(void),ov74_02231460(void*,u32,void*);extern int WM_SetIndCallback(void(*)(void*)),ov74_02231064(void);extern u8*ov74_02231054(void);
int ov74_02230A14(void);
void ov74_02230A34(void);
void ov74_02230A4C(void*a,u32 size,void*b);
int ov74_02230A14(void){if(WM_SetIndCallback(ov74_0222FFFC)!=0){ov74_02231070(4);return 0;}return 1;}
void ov74_02230A34(void){switch(ov74_02231064()){case 0:case 1:case 2:ov74_022314A0();break;}}
void ov74_02230A4C(void*a,u32 size,void*b){if(size<=*(u16*)(ov74_02231054()+0x34)){DC_FlushRange(a,size);ov74_02231460(a,size,b);}}
