#pragma force_active on
#include "global.h"
#pragma force_active on
extern u32 ov74_0223D0C0;
extern void *OverlayManager_GetData(void*);
extern u32 ListMenu_ProcessInput(void*);
extern void PlaySE(int);
void ov74_0222F404(void*,u32*,u32(*)(void*));
void ov74_0222F404(void*manager,u32*out,u32(*cancel)(void*)){u8*w=OverlayManager_GetData(manager);u32 result=ListMenu_ProcessInput(*(void**)(w+0x2bbc));switch(result){case -2:PlaySE(1500);if(cancel){u32 value=cancel(manager);if(value!=-1)*out=value;}break;case -1:break;default:PlaySE(1500);if(result==0)break;if(result<31){*out=result;break;}ov74_0223D0C0=result;u32 value=((u32(*)(void*))result)(manager);if(value!=-1)*out=value;break;}}
