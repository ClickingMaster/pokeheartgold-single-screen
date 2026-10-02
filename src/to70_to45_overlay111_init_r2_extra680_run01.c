#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 a,b,mode,c;} Args;
typedef struct {int heap;Args *args;void *bg,*d,*e,*f,*prompt,*g,*h,*resource;s8 flag;u8 pad[3];void *mode;u32 tail;} State;
extern Args *OverlayManager_GetArgs(void *);extern State *OverlayManager_CreateAndGetData(void *,int,int);extern void Heap_Create(int,int,u32);extern void *ov111_021E5C94(u8),*YesNoPrompt_Create(int),*ov111_021E67C4(int),*ov111_021E66DC(int,void *,void *,void *,void *,s8);
extern void ov111_021E5CD4(void),ov111_021E5CB4(void),ov111_021E5F50(State *),ov111_021E6000(State *),ov111_021E60D4(State *),ov111_021E6180(State *),ov111_021E67EC(void *,void *,int,u32),ResetVisibleHardwareWindows(int),TextFlags_SetCanABSpeedUpPrint(int),TextFlags_SetCanTouchSpeedUpPrint(int),Main_SetVBlankIntrCB(void (*)(void *),void *),ov111_021E5DF0(void *);
void ov111_021E5E34(void *);
void ov111_021E5E34(void *manager){Args *args=OverlayManager_GetArgs(manager);State *s;GF_ASSERT(args);Heap_Create(3,148,3<<16);s=OverlayManager_CreateAndGetData(manager,52,148);MI_CpuFill8(s,0,52);s->heap=148;s->args=args;s->mode=ov111_021E5C94(args->mode);ov111_021E5CD4();*(volatile u16*)0x04000304 &=~0x8000;ov111_021E5CB4();ov111_021E5F50(s);ov111_021E6000(s);ov111_021E60D4(s);ov111_021E6180(s);s->prompt=YesNoPrompt_Create(s->heap);s->resource=ov111_021E67C4(s->heap);ov111_021E67EC(s->resource,s->bg,1,args->c);s->h=ov111_021E66DC(s->heap,s->bg,s->resource,s->d,s->e,s->flag);ResetVisibleHardwareWindows(0);ResetVisibleHardwareWindows(1);TextFlags_SetCanABSpeedUpPrint(1);TextFlags_SetCanTouchSpeedUpPrint(1);Main_SetVBlankIntrCB(ov111_021E5DF0,s);}
