#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define S(p,o) (*(s16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern u8 gSystem[];
extern void *OverlayManager_GetData(void*);
extern int Pokedex_IsEnabled(void*),SaveMysteryGift_FindAvailable(void*),TextPrinterCheckActive(u8),ov74_02228F14(void*),ov74_0222962C(void*);
extern void ov74_02228D64(void*),ov74_02228F8C(void*),ov74_0223539C(int,int,int*,int),ov74_02235390(int),ov74_02228E98(void*),ov74_0222EC08(void*,void*,int),ov74_02229190(void*),ov74_022353FC(int*),ov74_022358BC(void),GfGfx_EngineBTogglePlanes(int,int);
extern int ov74_02235568(void*,void*,int,int,int);
int ov74_02229294(void*,int*);
#pragma force_active on
int ov74_02229294(void *manager,int *state){u8 *w=OverlayManager_GetData(manager);
 switch(*state){
 case 0:ov74_02228D64(w);if(!Pokedex_IsEnabled(P(w,12)))ov74_0223539C(1,1,state,13);else *state=2;break;
 case 1:if(!ov74_02228F14(w)){ov74_02235390(1);ov74_0223539C(0,12,state,13);}break;
 case 2:ov74_02228F8C(w);if(!SaveMysteryGift_FindAvailable(P(w,0x3174))){W(w,0x98)=1;W(w,0x317c)=ov74_02235568(P(w,4),w+0x48,2,19,22);ov74_0223539C(1,3,state,13);}else{W(w,0x14c)=7;ov74_0223539C(1,4,state,13);}break;
 case 3:if(!TextPrinterCheckActive((u8)W(w,0x317c))){if(U(gSystem,0x48)&1){ov74_02235390(1);ov74_0223539C(0,12,state,13);}}break;
 case 4:W(w,0x150)=ov74_0222962C(w);if(W(w,0x150)==5){ov74_02235390(1);ov74_0223539C(0,12,state,13);}else if(W(w,0x150)==4){ov74_02235390(1);ov74_0223539C(0,12,state,13);}break;
 case 5:ov74_02228E98(w);GfGfx_EngineBTogglePlanes(1,1);GfGfx_EngineBTogglePlanes(2,0);ov74_0222EC08(P(w,4),w+0x3180,83);ov74_0223539C(1,6,state,13);B(w,0x32d2)&=~4;break;
 case 6:if(U(gSystem,0x48))ov74_0223539C(0,11,state,13);break;
 case 11:OS_ResetSystem(0);break;case 12:ov74_02229190(manager);return 1;case 13:ov74_022353FC(state);break;
 }ov74_022358BC();return 0;
}
