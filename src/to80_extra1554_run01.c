#pragma force_active on
#include "global.h"
#pragma force_active on
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
extern int ov80_02236DD4(int),ov81_02241D0C(void*);
extern void ov81_02241524(void*),ov81_02243228(void*,u32,int*,int*),ov81_02241CA0(void*,int*,int*),ov81_02241F50(void*),ov81_02241FEC(void*),ov81_022420B4(void*),ov81_0224218C(void*),ov81_022408C4(void*,void*,int,int,int),ov81_02242F48(void*),ov81_02242FB0(void*,int),ov81_02242F94(void*,int),ov81_022408A0(void*,int,int),Pokepic_SetAttr(void*,int,int),ov81_02241E68(void*,int,void*,int),ov81_02241C84(int,void*,int*,int*),ov81_02242D94(void*,int,int),ov81_02240658(void*,int),FillWindowPixelBuffer(void*,int),ov81_02240AD8(void*,void*,int,int,int,int,int,int,u16,u8);
extern void *ov81_02242D18(void*,int,int,int,int,int,int),*ov81_02242CBC(void*,int,int,int,int,int,int);
void ov81_0223E5B4(u8*);
void ov81_0223E5B4(u8*w){int x,y,selected,max=ov80_02236DD4(w[9]),i;u8 *q;
ov81_02240F38(w,0);ov81_02241524(w);ov81_02243228(PTR(w,0x464),U32(w,0x468),&x,&y);PTR(w,0x388)=ov81_02242D18(w+0x1c4,0,2,0,0,x,y);
ov81_02243228(PTR(w,0x464),6,&x,&y);PTR(w,0x38c)=ov81_02242D18(w+0x1c4,0,3,0,0,x,y);
ov81_02241CA0(w,&x,&y);PTR(w,0x390)=ov81_02242CBC(w+0x1c4,1,0,0,0,x,y);
ov81_02241F50(w);ov81_02241FEC(w);ov81_022420B4(w);ov81_0224218C(w);
PTR(w,0x394)=ov81_02242D18(w+0x1c4,4,0,0,0,80,12);PTR(w,0x398)=ov81_02242D18(w+0x1c4,5,0,0,0,80,12);
ov81_02242218(w,PTR(w,0x3c0),U32(w,0x468),6);ov81_02242300(w,U32(w,0x468),6);BgCellManager_DrawCellAt(PTR(w,0x474),0,7,0);ov81_022408C4(w,w+0x50,0,0,0);
{i=0;if(i<w[17]){q=w;do{ov81_02242F48(PTR(w+U16(q,0x3c8)*4,0x360));ov81_02242FB0(PTR(w+U16(q,0x3c8)*4,0x360),0);ov81_02242F94(PTR(w+U16(q,0x3c8)*4,0x360),1);i++;q+=2;}while(i<w[17]);}}
ov81_02243028(w+0xc0,Options_GetFrame(PTR(w,0x1b8)));ov81_022408A0(w,0,w[17]+1);w[16]=ov81_0224086C(w,0);
if(F(w).b1==1){selected=ov81_02241D0C(w);{i=0;if(i<w[17]){q=w;do{Pokepic_SetAttr(PTR(q,0x1ac),6,0);q+=4;i++;}while(i<w[17]);}}Pokepic_SetAttr(PTR(w+selected*4,0x1ac),6,0);ov81_02241E68(w,selected,PTR(w,0x468),0);ov81_02241FEC(w);ov81_02241C84(selected,PTR(w,0x47c),&x,&y);ov81_02242D94(PTR(w,0x390),x,y);ov81_02240658(w,255);ov81_02241450(w);
if(w[24]!=0){i=0;if(max>0){u8 *win=w,*q=w;win+=0x50;do{FillWindowPixelBuffer(win+(i+5)*16,0);if(i<w[24])ov81_02240AD8(w,win+(i+5)*16,0,0,15,2,0,0,U16(q,0x45a),(u8)U16(q,0x45e));ScheduleWindowCopyToVram(win+(i+5)*16);i++;q+=2;}while(i<max);}}}else ov81_022414E0(w);}
