#pragma force_active on
#include "global.h"
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
#define U8(p,o) (*(u8*)((u8*)(p)+(o)))
typedef struct {u8 b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1;} Flags;
#define F(p) (*(Flags*)((u8*)(p)+19))
extern int GridInputHandler_HandleInput_NoHold(void*);extern u32 GridInputHandler_GetNextInput(void*);
extern void PlaySE(int),ov81_02241C0C(void*),ov81_02242218(void*,void*,u32,int),ov81_02242300(void*,u32,int),ov81_02241450(void*),ov81_022425C4(void*,int,int,int),ov81_022414E0(void*),ov81_0223EF5C(void*),ov81_02240FA4(void*,int,int),ov81_022425EC(void*),PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(void*),ov81_02240E78(void*),ov81_02240BB0(void*),BeginNormalPaletteFade(int,int,int,int,int,int,int),ov81_0223F038(void*),ov81_0223F0BC(void*);
extern int ov80_02237254(int),IsPaletteFadeFinished(void),PaletteData_GetSelectedBuffersBitmask(void*),ov81_02242F40(void*);
extern u8 gOverlayTemplate_PokemonSummary[];
extern void *OverlayManager_New(const void*,void*,int);
extern int ov81_0223E318(u8*),ov81_0223EC88(void*),ov81_0223E520(void*),ov81_02241144(void*),ov81_0223ECE4(void*),ov81_02240F28(void*),ov81_0223F1A4(void*),ov81_0223F314(void*),ov81_0223F38C(void*),ov81_0223F6A8(void*),ov81_0223F770(void*),ov81_0223FBAC(void*),ov81_0223FC74(void*),ov81_02240008(void*),ov81_02240048(void*),ov81_02240088(void*),ov81_022400D0(void*);
extern void *OverlayManager_GetData(void*);
extern void ov81_022404AC(void*,int*,int),ov81_02242514(void*),ov81_02240F38(void*,int),ov81_0223E8BC(void*),ov81_0223EA98(void*),SpriteList_RenderAndAnimateSprites(void*),ov81_02242C48(void*);
extern int ov81_02240F08(void*,int),ov81_02240F18(int);
int ov81_0223DEA8(void*,int*);
int ov81_0223DEA8(void*manager,int*state){u8*w=OverlayManager_GetData(manager);if(*(u16*)(w+0x458)==1){switch(*state){case 6:case 7:case 8:case 9:if(F(w).b6==0)ov81_022404AC(w,state,11);break;}}ov81_02242514(w);switch(*state){
case 0:if(ov81_0223E318(w)==1)ov81_022404AC(w,state,1);break;
case 2:if(ov81_0223EC88(w)==1)ov81_022404AC(w,state,1);else return 0;
case 1:if(ov81_0223E520(w)==1){if(ov81_02240F08(w,0)==1)ov81_022404AC(w,state,3);else ov81_022404AC(w,state,6);}break;
case 3:ov81_02241144(w);if(ov81_0223ECE4(w)==1){if(F(w).b1==1)ov81_022404AC(w,state,2);else if(w[17]==ov81_02240F18(w[9]))ov81_022404AC(w,state,4);else if(ov81_02240F28(w)==1)ov81_022404AC(w,state,13);else ov81_022404AC(w,state,3);}break;
case 4:ov81_02241144(w);if(ov81_0223F1A4(w)==1){if(w[17]==ov81_02240F18(w[9]))ov81_022404AC(w,state,13);else ov81_022404AC(w,state,5);}break;
case 5:if(ov81_0223F314(w)==1)ov81_022404AC(w,state,3);break;
case 6:if(ov81_0223F38C(w)==1){if(F(w).b1==1)ov81_022404AC(w,state,2);else if(ov81_02240F28(w)==1){ov81_02240F38(w,0);ov81_022404AC(w,state,7);}else ov81_022404AC(w,state,10);}break;
case 7:if(ov81_0223F6A8(w)==1){if(ov81_02240F28(w)==1){ov81_02240F38(w,0);ov81_022404AC(w,state,6);}else if(ov80_02237254(w[9])==1){F(w).b3=0;ov81_022404AC(w,state,11);}else ov81_022404AC(w,state,13);}break;
case 8:if(ov81_0223F770(w)==1){if(ov81_02240F28(w)==1){ov81_02240F38(w,0);ov81_022404AC(w,state,9);}else if(w[17]==0)ov81_022404AC(w,state,10);else if(ov80_02237254(w[9])==1)ov81_022404AC(w,state,11);else ov81_022404AC(w,state,13);}break;
case 9:if(ov81_0223FBAC(w)==1){if(ov81_02240F28(w)==1){ov81_02240F38(w,0);ov81_022404AC(w,state,8);}else if(ov80_02237254(w[9])==1){F(w).b3=0;ov81_022404AC(w,state,11);}else ov81_022404AC(w,state,13);}break;
case 10:if(ov81_0223FC74(w)==1){if(w[17]==0){ov81_0223E8BC(w);ov81_022404AC(w,state,6);}else{ov81_0223EA98(w);ov81_022404AC(w,state,8);}}break;
case 11:if(ov81_02240008(w)==1){if(*(u16*)(w+0x458)==1)ov81_022404AC(w,state,14);else ov81_022404AC(w,state,12);}break;
case 12:if(ov81_02240048(w)==1)ov81_022404AC(w,state,13);break;
case 13:if(ov81_02240088(w)==1)return 1;break;
case 14:if(ov81_022400D0(w)==1)ov81_022404AC(w,state,12);break;}
SpriteList_RenderAndAnimateSprites(PTR(w,0x1c4));ov81_02242C48(PTR(w,0x1a8));return 0;}
