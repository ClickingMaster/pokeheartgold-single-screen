#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 pad,b,pad8,a;} Reply;
extern int ov40_0223D5CC(void*),VsRecorder_StepBlend(void*,void*,int,int),GlobalTerminalNet_RequestBoxUpload(void*,u32,u32,u32,u32),GlobalTerminalNet_GetError(void*,Reply**);
extern void *ov40_0223D540(void*);
extern void ov40_02236FE0(void*),ov40_02237564(void*),VsRecorder_SetVBlankPaused(void*,int),ov40_02237410(void*),ov40_022371A0(void*),VsRecorder_StepFade(void*,int),VsRecorder_PlaceOrbitCursor(void*,void*,int,int),VsRecorder_ShowTopMessage(void*,int),VsRecorder_HideTopMessage(void*),VsRecorder_ShowMessageScreen(void*,int,u32,u32),VsRecorder_AddGameStatsScore(void*,int),ov40_0222DD8C(void*,int),VsRecorder_SetSubState(void*,int);
extern void VsRecorder_ApplyBlend(void*,void*,int,int,int,int,int),OrbitCursor_SetVisible(void*,int),OrbitCursor_SetRadius(void*,int,int),GfGfx_EngineBTogglePlanes(int,int),GfGfx_EngineATogglePlanes(int,int),PaletteData_BlendPalettes(void*,int,int,u8,u16),PlaySE(int),StopSE(int,int);
int ov40_02236A70(u8*);
#define WORD(p,o) (*(u32*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
int ov40_02236A70(u8 *s){u8 *work=PTR(s,0x860);Reply *reply;if(!ov40_0223D5CC(s))return 0;switch(WORD(s,8)){
case 0:ov40_02236FE0(s);OrbitCursor_SetVisible(PTR(s,0x6f4),0);GfGfx_EngineBTogglePlanes(4,0);WORD(s,8)++;
case 1:VsRecorder_ApplyBlend(work+0x1a4,work+0x1a8,8,18,8,18,1);ov40_02237564(s);VsRecorder_SetVBlankPaused(s,1);ov40_02237410(s);VsRecorder_SetVBlankPaused(s,0);ov40_022371A0(s);GfGfx_EngineATogglePlanes(4,0);WORD(s,8)++;break;
case 2:VsRecorder_StepFade(work+0x1ac,1);if(VsRecorder_StepBlend(work+0x1a4,work+0x1a8,1,0))WORD(s,8)++;PaletteData_BlendPalettes(PTR(s,0x28),3,12,(u8)WORD(work,0x1ac),(u16)WORD(s,0x58));break;
case 3:WORD(s,8)++;
case 4:VsRecorder_PlaceOrbitCursor(s,PTR(s,0x6f0),128,96);OrbitCursor_SetVisible(PTR(s,0x6f0),1);OrbitCursor_SetRadius(PTR(s,0x6f0),24,24);VsRecorder_ShowTopMessage(s,0x120);PlaySE(0x57d);WORD(s,8)++;break;
case 5:if(GlobalTerminalNet_RequestBoxUpload(ov40_0223D540(s),WORD(work,0x380),WORD(work,0x384),WORD(work,0),WORD(work,0x1a0))==1)WORD(s,8)++;break;
case 6:VsRecorder_HideTopMessage(s);if(GlobalTerminalNet_GetError(ov40_0223D540(s),&reply)==1){StopSE(0x57d,0);VsRecorder_ShowMessageScreen(s,2,reply->a,reply->b);WORD(s,8)++;}else{StopSE(0x57d,0);VsRecorder_AddGameStatsScore(s,37);ov40_0222DD8C(s,1);PlaySE(0x577);WORD(s,8)++;}break;
case 7:OrbitCursor_SetVisible(PTR(s,0x6f0),0);OrbitCursor_SetRadius(PTR(s,0x6f0),0,0);WORD(s,8)++;break;
default:VsRecorder_SetSubState(s,10);break;}return 0;}
