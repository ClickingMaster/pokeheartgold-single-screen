#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_common_subs off
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
typedef struct{u16 id,other;}Item;
extern void FillWindowPixelBuffer(void *,u8),CopyWindowPixelsToVram_TextMode(void *);
extern void ov18_021EF1E4(void *,int),ov18_021EF220(void *,int),ov18_021EF25C(void *,int),ov18_021EF298(void *,int),ov18_021EF2D4(void *,int),ov18_021EF310(void *,int),ov18_021EF34C(void *,int),ov18_021EFBE8(void *,int),ov18_021EFC3C(void *,int),ov18_021EFC9C(void *,int,int,int),ov18_021EFD00(void *,u16,int),ov18_021EFDB4(void *,u16,int),ov18_021EFE70(void *,int);
void ov18_021EFEC4(u8 *);
void ov18_021EFEC4(u8 *p){u32 i;FillWindowPixelBuffer(p+28,0);FillWindowPixelBuffer(p+44,0);FillWindowPixelBuffer(p+60,0);FillWindowPixelBuffer(p+76,0);FillWindowPixelBuffer(p+92,0);FillWindowPixelBuffer(p+108,0);FillWindowPixelBuffer(p+124,0);ov18_021EF1E4(p,1);ov18_021EF220(p,2);ov18_021EF25C(p,3);ov18_021EF298(p,4);ov18_021EF2D4(p,5);ov18_021EF310(p,6);ov18_021EF34C(p,7);ov18_021EFBE8(p,8);ov18_021EFC3C(p,9);ov18_021EFC9C(p,W(p,0x1870),10,29);ov18_021EFC9C(p,W(p,0x1874),11,29);ov18_021EFD00(p,((Item *)W(p,0x1850))[W(p,0x1878)].id,12);ov18_021EFD00(p,((Item *)W(p,0x1850))[W(p,0x187c)].id,13);ov18_021EFDB4(p,((Item *)W(p,0x1850))[W(p,0x1880)].other,14);ov18_021EFDB4(p,((Item *)W(p,0x1850))[W(p,0x1884)].other,15);ov18_021EFE70(p,16);i=1;p+=28;for(;i<=16;i++,p+=16)CopyWindowPixelsToVram_TextMode(p);}
