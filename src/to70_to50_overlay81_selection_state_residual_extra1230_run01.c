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
extern void ov81_02240F38(void*,int),ov81_02241D38(void*),BgCellManager_DrawCellAt(void*,int,int,int),BgCellManager_EraseCell(void*,int),ov81_0224174C(void*),ov81_02242E08(void*,int),ov81_02242D88(void*,int),ClearWindowTilemapAndScheduleTransfer(void*),ov81_022425D8(void*,int,int,int),ScheduleWindowCopyToVram(void*),ov81_022417B4(void*),ov81_02241840(void*),ov81_0223F684(void*);
extern int Options_GetFrame(void*),ov81_0224086C(void*,int),YesNoPrompt_HandleInput(void*);
extern void ov81_02243028(void*,int),ov81_02241D94(void*),ov81_022424AC(void*,u32),ov81_02241980(void*),Pokepic_SetAttr(void*,int,int),ov81_022419E0(void*),ov81_02241A7C(void*),YesNoPrompt_Reset(void*),ov81_02242694(void*,int),ov81_0223FB64(void*),ov81_0223FB88(void*),ov81_02241A38(void*),ov81_0223FB3C(void*),ov81_02241BD0(void*,void*);
#define U16(p,o) (*(u16*)((u8*)(p)+(o)))
int ov81_0223F770(u8*);
int ov81_0223F770(u8*w){u32 input;
switch(w[8]){
case 0:ov81_02243028(w+0xc0,Options_GetFrame(PTR(w,0x1b8)));w[16]=ov81_0224086C(w,13);w[8]=1;break;
case 1:input=GridInputHandler_HandleInput_NoHold(PTR(w,0x464));switch(input){
case 0:case 1:case 2:case 3:PlaySE(1500);U32(w,0x468)=input;ov81_02242218(w,PTR(w,0x3c4),U32(w,0x468),4);BgCellManager_DrawCellAt(PTR(w,0x474),0,7,0);ov81_02241D94(w);ov81_022424AC(w,U32(w,0x468));ov81_02241980(w);ov81_02242E08(PTR(w,0x388),1);break;
case 4:PlaySE(1500);ov81_02242D88(PTR(w,0x394),0);ov81_02242D88(PTR(w,0x398),0);Pokepic_SetAttr(PTR(w+U16(w,0x3c8)*4,0x1ac),6,1);ClearWindowTilemapAndScheduleTransfer(w+0xd0);ClearWindowTilemapAndScheduleTransfer(w+0x50+(U16(w,0x3c8)+2)*16);BgCellManager_EraseCell(PTR(w,0x474),0);ov81_022425D8(w,2,15,5);w[8]=4;break;
case 5:PlaySE(1500);ov81_02242D88(PTR(w,0x394),0);ov81_02242D88(PTR(w,0x398),0);Pokepic_SetAttr(PTR(w+U16(w,0x3c8)*4,0x1ac),6,1);ClearWindowTilemapAndScheduleTransfer(w+0xd0);ClearWindowTilemapAndScheduleTransfer(w+0x50+(U16(w,0x3c8)+2)*16);BgCellManager_EraseCell(PTR(w,0x474),0);ov81_022425D8(w,23,15,6);w[8]=4;break;
case 6:PlaySE(1500);ov81_022425C4(w,2,15,7);w[8]=4;break;
case 7:PlaySE(1500);ov81_022425C4(w,18,15,8);w[8]=4;break;
case -3:PlaySE(1500);input=GridInputHandler_GetNextInput(PTR(w,0x464));if((u32)(input-4)<=1){ov81_02242D88(PTR(w,0x394),0);ov81_02242D88(PTR(w,0x398),0);Pokepic_SetAttr(PTR(w+U16(w,0x3c8)*4,0x1ac),6,1);ClearWindowTilemapAndScheduleTransfer(w+0xd0);ClearWindowTilemapAndScheduleTransfer(w+0x50+(U16(w,0x3c8)+2)*16);BgCellManager_EraseCell(PTR(w,0x474),0);}else{switch(input){case 6:case 7:break;default:ov81_02242218(w,PTR(w,0x3c4),input,4);ScheduleWindowCopyToVram(w+0xd0);BgCellManager_DrawCellAt(PTR(w,0x474),0,7,0);ov81_02241D94(w);ov81_022424AC(w,input);break;}}break;
case -2:if(GridInputHandler_GetNextInput(PTR(w,0x464))>=6){PlaySE(1500);ov81_022419E0(w);ov81_02242E08(PTR(w,0x388),0);}else{PlaySE(1500);ov81_02241A7C(w);ov81_02242E08(PTR(w,0x388),1);ov81_02240F38(w,1);return 1;}break;}break;
case 2:switch(YesNoPrompt_HandleInput(PTR(w,0x46c))){case 1:YesNoPrompt_Reset(PTR(w,0x46c));ov81_02242694(w,0);ov81_0223FB64(w);if(ov80_02237254(w[9])==1)w[16]=ov81_0224086C(w,2);w[8]++;break;case 2:YesNoPrompt_Reset(PTR(w,0x46c));ov81_02242694(w,0);ov81_0223FB88(w);ov81_02241A38(w);ov81_02242E08(PTR(w,0x388),0);w[8]=1;break;}break;
case 3:return 1;
case 4:ov81_022425EC(w);break;
case 5:ov81_02241A7C(w);ov81_0223FB3C(w);return 1;
case 6:ov81_02241A7C(w);ov81_02240F38(w,1);return 1;
case 7:ov81_022419E0(w);ov81_02241A7C(w);ov81_02241BD0(PTR(w,0x46c),PTR(w,0x4c));ov81_02242694(w,1);w[16]=ov81_0224086C(w,14);w[8]=2;break;
case 8:ov81_022419E0(w);ov81_02242E08(PTR(w,0x388),0);w[8]=1;break;}return 0;}
