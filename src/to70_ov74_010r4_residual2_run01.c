#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x10];void *save,*profile,*options;int frame;void *bg;u8 gap[0xe88c-0x24];void *prompt;u8 gap2[0x12608-0xe890];void *s1,*s2;}Work;
extern void Heap_Create(int,int,u32);
extern Work *OverlayManager_CreateAndGetData(void*,u32,int);
extern void *OverlayManager_GetArgs(void*),*BgConfig_Alloc(int),*YesNoPrompt_Create(int),*Save_PlayerData_GetProfile(void*),*Save_PlayerData_GetOptionsAddr(void*),*String_New(int,int);
extern void SetMasterBrightnessByColor(int,int),Sound_SetSceneAndPlayBGM(int,int,int),OS_InitTick(void),ov74_02236074(void);
extern int Options_GetFrame(void*),OS_IsTickAvailable(void);
extern void *ov74_0223D338;

int ov74_02233230(void*m,void *state);

int ov74_02233230(void*m,void *state){Work*w;Heap_Create(3,76,14<<14);w=OverlayManager_CreateAndGetData(m,0x12610,76);memset(w,0,0x12610);w->bg=BgConfig_Alloc(76);w->prompt=YesNoPrompt_Create(76);SetMasterBrightnessByColor(0,0);SetMasterBrightnessByColor(1,0);w->save=((void**)OverlayManager_GetArgs(m))[2];w->profile=Save_PlayerData_GetProfile(w->save);w->options=Save_PlayerData_GetOptionsAddr(w->save);w->frame=Options_GetFrame(w->options);w->s1=String_New(384,76);w->s2=String_New(384,76);Sound_SetSceneAndPlayBGM(9,0x47e,1);if(OS_IsTickAvailable()==0)OS_InitTick();ov74_02236074();ov74_0223D338=(u8*)w+0xe89c;return 1;}
