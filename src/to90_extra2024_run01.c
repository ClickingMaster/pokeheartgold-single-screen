#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern u8*_022577C0;extern int ov45_02231514(void),ov45_022321E0(void),ov45_0222E96C(void);extern void ov45_022302E4(void*),ov45_022320C4(void*);int ov45_0222E6C8(void);
int ov45_0222E6C8(void){if(!_022577C0)return 0;switch(ov45_02231514()){case 0:B(_022577C0,0x12c)=0;break;case 1:case 2:case 3:case 4:case 5:case 6:case 7:B(_022577C0,0x12c)=1;break;case 8:B(_022577C0,0x12c)=2;break;case 9:B(_022577C0,0x12c)=3;break;case 10:B(_022577C0,0x12c)=4;break;}if(B(_022577C0,0x12c)==2){ov45_022302E4(_022577C0);ov45_022320C4(_022577C0+0x19c);switch(ov45_022321E0()){case 0:B(_022577C0,0x12e)=0;B(_022577C0,0x12d)=4;break;case 1:B(_022577C0,0x12e)=1;break;case 2:if(B(_022577C0,0x12e)!=3)B(_022577C0,0x12e)=2;break;case 3:B(_022577C0,0x12e)=4;break;}}return ov45_0222E96C();}
