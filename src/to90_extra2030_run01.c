#pragma force_active on
#include "global.h"
#include "pokemon.h"
#define PTR(p,o) (*(void **)((p)+(o)))
#define WORD(p,o) (*(s32 *)((p)+(o)))
extern void ov70_0223AE98(void *),ov70_0223ABF4(void *),ov70_0223ACF4(void *),ov70_0223AC98(void *),ov70_0223B258(void *);
extern void ov70_0223AB3C(void *,s32),Gts_DrawMonSummary(void *,void *,void *,void *,void *,void *),Gts_DrawTrainerInfo(void *,void *,void *,void *,void *),Gts_LoadMonPicture(void *),ov70_0223B3BC(void *,void *,s32),ov70_0223B3EC(void *,void *,s32),ov70_02241330(void *,s32,s32);
extern void ov70_0223B4D4(void);

extern void BeginNormalPaletteFade(s32,s32,s32,s32,s32,s32,s32);
s32 ov70_0223A8BC(u8 *p);
#pragma force_active on
s32 ov70_0223A8BC(u8 *p){
 s32 offset;
 ov70_0223AE98(p);ov70_0223AB3C(PTR(p,4),-32-WORD(p,0xf14));ov70_0223ABF4(p);ov70_0223ACF4(p);ov70_0223AC98(p);
 offset=WORD(p,0x12c)*0x124;
 Gts_DrawMonSummary(PTR(p,0xba0),PTR(p,0xba4),PTR(p,0xb9c),p+0x1058,Mon_GetBoxMon((Pokemon*)(p+0x260+offset)),p+0x34c+offset);
 offset=WORD(p,0x12c)*0x124;
 Gts_DrawTrainerInfo(PTR(p,0xba0),p+0x10c8,p+0x36c+offset,p+0x260+offset,p+0x1118);
 Gts_LoadMonPicture(p+0x260+WORD(p,0x12c)*0x124);
 ov70_0223B3BC(PTR(p,0xba0),p+0x1138,0x4d);ov70_0223B3BC(PTR(p,0xba0),p+0x10e8,0x51);ov70_0223B3EC(PTR(p,0xba0),p+0xf58,0x58);ov70_0223B3EC(PTR(p,0xba0),p+0xf68,0x6d);
 ov70_0223B258(p);ov70_02241330(p,WORD(p,0x12c),-WORD(p,0xf14));
 *(void (**)(void))(p+0x1208)=ov70_0223B4D4;*(volatile u16*)0x04000304|=0x8000;
 if(WORD(p,0x24)==17){if(GXx_GetMasterBrightness_((volatile u16*)0x400006c)==-16&&GXx_GetMasterBrightness_((volatile u16*)0x400106c)!=-16)BeginNormalPaletteFade(3,1,1,0,6,1,61);
 else if(GXx_GetMasterBrightness_((volatile u16*)0x400006c)!=-16&&GXx_GetMasterBrightness_((volatile u16*)0x400106c)==-16)BeginNormalPaletteFade(4,1,1,0,6,1,61);
 else BeginNormalPaletteFade(0,1,1,0,6,1,61);}
 WORD(p,0x2c)=0;return 2;
}
