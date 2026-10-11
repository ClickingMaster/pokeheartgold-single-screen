#pragma force_active on
#include "global.h"
#pragma force_active on

#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern void VsRecorder_ApplyBlend(void*,void*,int,int,int,int,int);extern int VsRecorder_StepBlend(void*,void*,int,int);extern void ov40_02230964(void*,int),ov40_02237564(void*),ov40_02237410(void*),ov40_022371A0(void*),ov40_02236578(void*,void*,void*,void*),ov40_0223655C(void*),ov40_02237644(void*),ov40_02237548(void*,int),ov40_022373E4(void*,int),ov40_022371E4(void*,u32),ov40_02237144(void*),ov40_02237030(void*,int),GfGfx_EngineATogglePlanes(int,int),VsRecorder_SetSubState(void*,int);
int ov40_022367B8(void*);
int ov40_022367B8(void*w){u8*s=P(w,0x860);switch(W(w,8)){case 0:VsRecorder_ApplyBlend(s+0x1a4,s+0x1a8,8,18,8,18,1);ov40_02230964(w,1);ov40_02237564(w);ov40_02237410(w);ov40_022371A0(w);ov40_02230964(w,0);W(w,8)++;case 1:if(VsRecorder_StepBlend(s+0x1a4,s+0x1a8,1,1)){ov40_02236578(P(w,0x830),P(s,0),P(s,0x1a0),s+4);ov40_0223655C(s);W(w,8)++;}break;case 2:ov40_02230964(w,1);ov40_02237644(w);ov40_02237548(w,1);ov40_022373E4(w,0);ov40_02230964(w,0);W(w,8)++;break;case 3:if(VsRecorder_StepBlend(s+0x1a4,s+0x1a8,0,1)){ov40_022371E4(w,W(s,0x1b0));ov40_02237144(w);ov40_02237030(w,270);ov40_02237548(w,0);ov40_022373E4(w,1);GfGfx_EngineATogglePlanes(1,1);*(volatile u16*)0x04000050=0;VsRecorder_SetSubState(w,3);}break;}return 0;}
