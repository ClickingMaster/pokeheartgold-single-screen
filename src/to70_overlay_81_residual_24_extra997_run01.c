#pragma force_active on
#include "global.h"
extern u8 ov81_022436F0[],ov81_02243808[],ov81_02243658[];
extern void *GridInputHandler_Create(void *,void *,void *,void *,int,u8,int);
extern void GridInputHandler_SetAllEnabled(void *),GridInputHandler_ClearEnabledFlag(void *,int);
void *ov81_02243240(void *,int);void ov81_02243270(void *,int);
void *ov81_02243240(void *arg,int flag){return GridInputHandler_Create(ov81_022436F0,ov81_02243808,ov81_02243658,arg,1,(u8)flag,100);}
void ov81_02243270(void *h,int enabled){if(enabled==1){GridInputHandler_SetAllEnabled(h);}else{GridInputHandler_ClearEnabledFlag(h,6);GridInputHandler_ClearEnabledFlag(h,7);GridInputHandler_ClearEnabledFlag(h,8);}}
