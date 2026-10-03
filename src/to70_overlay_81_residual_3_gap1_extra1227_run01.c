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
extern void ov81_02240F38(void*,int),ov81_02241D38(void*),sub_020196E8(void*,int,int,int),sub_0201980C(void*,int),ov81_0224174C(void*),ov81_02242E08(void*,int),ov81_02242D88(void*,int),ClearWindowTilemapAndScheduleTransfer(void*),ov81_022425D8(void*,int,int,int),ScheduleWindowCopyToVram(void*),ov81_022417B4(void*),ov81_02241840(void*),ov81_0223F684(void*);
int ov81_0223F38C(u8*);
int ov81_0223F38C(u8*w){u32 input;
switch(w[8]){
case 0:ov81_02240F38(w,0);w[8]=1;F(w).b1=0;break;
case 1:input=GridInputHandler_HandleInput_NoHold(PTR(w,0x464));switch(input){
case 0:case 1:case 2:PlaySE(1500);U32(w,0x468)=input;ov81_02242218(w,PTR(w,0x3c0),U32(w,0x468),4);ov81_02241D38(w);sub_020196E8(PTR(w,0x474),0,7,0);ov81_0224174C(w);ov81_02242E08(PTR(w,0x388),1);break;
case 4:PlaySE(1500);ov81_022425C4(w,2,12,7);w[8]=2;break;
case 5:PlaySE(1500);ov81_022425C4(w,18,12,8);w[8]=2;break;
case 6:PlaySE(1500);ov81_022425C4(w,10,15,9);w[8]=2;break;
case 3:PlaySE(1500);ov81_02242D88(PTR(w,0x394),0);ov81_02242D88(PTR(w,0x398),0);ClearWindowTilemapAndScheduleTransfer(w+0xd0);sub_0201980C(PTR(w,0x474),0);ov81_02241D38(w);ov81_022425D8(w,23,15,6);w[8]=2;break;
case -3:PlaySE(1500);{u32 next=GridInputHandler_GetNextInput(PTR(w,0x464));if(next==3){ov81_02242D88(PTR(w,0x394),0);ov81_02242D88(PTR(w,0x398),0);ClearWindowTilemapAndScheduleTransfer(w+0xd0);sub_0201980C(PTR(w,0x474),0);}else{ov81_02242218(w,PTR(w,0x3c0),next,3);ScheduleWindowCopyToVram(w+0xd0);sub_020196E8(PTR(w,0x474),0,7,0);}ov81_02241D38(w);}break;
case -2:if(GridInputHandler_GetNextInput(PTR(w,0x464))>=4){PlaySE(1500);ov81_022417B4(w);ov81_02242E08(PTR(w,0x388),0);}else{PlaySE(1500);ov81_02241840(w);ov81_02242E08(PTR(w,0x388),1);ov81_02240F38(w,1);return 1;}break;}break;
case 2:ov81_022425EC(w);break;
case 3:if(IsPaletteFadeFinished()==1){if(PaletteData_GetSelectedBuffersBitmask(PTR(w,0x1a0))!=0){PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(PTR(w,0x1a0));U32(w,0x478)=255;}else{ov81_02240E78(w);ov81_02240BB0(w);PTR(w,4)=OverlayManager_New(gOverlayTemplate_PokemonSummary,PTR(w,0x1c0),100);F(w).b1=1;return 1;}}break;
case 4:if(PTR(w,4)==0)return 1;break;
case 5:if(IsPaletteFadeFinished()==1)w[8]=1;break;
case 6:ov81_02241840(w);ov81_02240F38(w,1);return 1;
case 7:BeginNormalPaletteFade(0,0,0,0,6,1,100);F(w).b6=1;w[8]=3;break;
case 8:ov81_0223F684(w);ov81_022417B4(w);ov81_02241840(w);return 1;
case 9:ov81_022417B4(w);ov81_02242E08(PTR(w,0x388),0);w[8]=1;break;}return 0;}
