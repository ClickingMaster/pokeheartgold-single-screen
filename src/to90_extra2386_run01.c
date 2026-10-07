#pragma force_active on
#include "global.h"
#pragma force_active on
extern int ov72_02237B54(void),ov72_02237B74(void);
extern void ov73_021EA180(void*),ov73_021E83EC(void*,int,int),sub_020399EC(void);
#define I(o) (*(u32*)(w+(o)))
int ov73_021E8FE4(u8*);
int ov73_021E8FE4(u8*w){if(ov72_02237B54()){int result=ov72_02237B74();I(0xf94)=0;switch(result){case 0:ov73_021EA180(w);switch(I(0xf88)){case 0:switch(I(0xf8c)){case 0:switch(*(u32*)((u8*)*(void**)w+0x18)){case 0:ov73_021E83EC(w,11,13);break;case 1:I(0x1c)=25;break;case 2:I(0x1c)=29;break;}break;case 1:case 2:case 3:I(0xf1c)=result;I(0x1c)=53;ov73_021EA180(w);break;}break;case 1:case 2:I(0xf1c)=result;I(0x1c)=53;ov73_021EA180(w);break;default:ov73_021EA180(w);sub_020399EC();break;}break;
case 1:I(0xf1c)=result;I(0x1c)=53;ov73_021EA180(w);break;
case -1:case 2:I(0xf1c)=result;I(0x1c)=53;ov73_021EA180(w);break;
case -7:case -4:I(0xf1c)=result;I(0x1c)=53;ov73_021EA180(w);break;
case -6:case -2:I(0xf1c)=result;I(0x1c)=53;ov73_021EA180(w);break;
default:ov73_021EA180(w);sub_020399EC();break;}}
else{I(0xf94)++;if(I(0xf94)==3600)sub_020399EC();}return 3;}
