#pragma force_active on
#include "global.h"
#include "constants/sndseq.h"
#pragma force_active on
typedef struct{int heap;void*field;u16 state,time;u8 p[0xcc-12];s32 pitch;}W31715;
extern W31715*TaskManager_GetEnvironment(void*);extern void ov02_02245D18(W31715*),ov02_02245DE0(W31715*),ov02_02245DB0(W31715*),AreaLightManager_Free(void*);extern void*AreaLightManager_New(void*,int);extern void BeginNormalPaletteFade(int,int,int,u16,int,int,int),Heap_Free(void*);extern int IsPaletteFadeFinished(void),PlaySE(u16);extern void PlaySE_SetPitch(u16,s16);
int ov02_02245BC8(void*);
int ov02_02245BC8(void*task){W31715*w=TaskManager_GetEnvironment(task);switch(w->state){case 0:ov02_02245D18(w);w->time=0;w->pitch=0;w->state++;break;case 1:if(w->pitch!=255){if(w->pitch==0||w->pitch==30||w->pitch==50||w->pitch==60||w->pitch==70||w->pitch==80){PlaySE(SEQ_SE_DP_FW411);if(w->pitch==50)PlaySE(SEQ_SE_DP_F207);if(w->pitch>=60)PlaySE_SetPitch(SEQ_SE_DP_FW411,(s16)((u32)(w->pitch/10-5)*64u));}w->pitch=(s32)((u32)w->pitch+1);}ov02_02245DE0(w);if(w->time++>=90){BeginNormalPaletteFade(3,0,0,32767,24,2,w->heap);w->state++;}break;case 2:ov02_02245DE0(w);if(IsPaletteFadeFinished()){w->time=0;w->state++;}break;case 3:if(w->time++>=60){ov02_02245DB0(w);AreaLightManager_Free((u8*)w->field+0x50);*(void**)((u8*)w->field+0x50)=AreaLightManager_New(*(void**)((u8*)w->field+0x48),4);BeginNormalPaletteFade(3,1,1,32767,18,1,w->heap);w->state++;}break;case 4:if(IsPaletteFadeFinished()){Heap_Free(w);return 1;}break;default:break;}return 0;}
