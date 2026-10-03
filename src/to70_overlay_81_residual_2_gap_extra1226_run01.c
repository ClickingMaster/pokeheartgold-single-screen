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
int ov81_0223ECE4(u8*);
int ov81_0223ECE4(u8*w){u32 input;
switch(w[8]){
case 0:w[8]=1;F(w).b1=0;break;
case 1:input=GridInputHandler_HandleInput_NoHold(PTR(w,0x464));switch(input){
case 0:case 1:case 2:case 3:case 4:case 5:PlaySE(1500);U32(w,0x468)=input;ov81_02241C0C(w);ov81_02242218(w,PTR(w,0x3c0),U32(w,0x468),6);ov81_02242300(w,U32(w,0x468),6);ov81_02241450(w);break;
case 6:PlaySE(1500);ov81_022425C4(w,2,12,6);w[8]=2;break;
case 7:PlaySE(1500);ov81_022425C4(w,18,12,7);w[8]=2;break;
case 8:PlaySE(1500);ov81_022425C4(w,10,15,8);w[8]=2;break;
case -3:PlaySE(1500);ov81_02241C0C(w);ov81_02242218(w,PTR(w,0x3c0),GridInputHandler_GetNextInput(PTR(w,0x464)),6);ov81_02242300(w,GridInputHandler_GetNextInput(PTR(w,0x464)),6);break;
case -2:if(GridInputHandler_GetNextInput(PTR(w,0x464))>=6){PlaySE(1500);ov81_022414E0(w);return 1;}if(w[17]!=0){PlaySE(1500);ov81_0223EF5C(w);if(ov80_02237254(w[9])==1)ov81_02240FA4(w,8,0);return 1;}break;}break;
case 2:ov81_022425EC(w);break;
case 3:if(IsPaletteFadeFinished()==1){if(PaletteData_GetSelectedBuffersBitmask(PTR(w,0x1a0))!=0){PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(PTR(w,0x1a0));U32(w,0x478)=255;}else{ov81_02240E78(w);ov81_02240BB0(w);PTR(w,4)=OverlayManager_New(gOverlayTemplate_PokemonSummary,PTR(w,0x1c0),100);F(w).b1=1;return 1;}}break;
case 4:if(PTR(w,4)==0)return 1;break;
case 5:if(IsPaletteFadeFinished()==1)w[8]=1;break;
case 6:BeginNormalPaletteFade(0,0,0,0,6,1,100);F(w).b6=1;w[8]=3;break;
case 7:ov81_022414E0(w);if(ov81_02242F40(PTR(w,0x360+U32(w,0x468)*4))==0)ov81_0223F038(w);else ov81_0223F0BC(w);if(ov80_02237254(w[9])==1)ov81_02240FA4(w,8,0);return 1;
case 8:ov81_022414E0(w);return 1;}return 0;}
