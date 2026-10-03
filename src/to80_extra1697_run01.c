#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct{u8 p[0x180];void*grid;u8 q[0x1ac-0x184];u32 limit;int unknown;int page,count,message;u8 r[8];int state,next;}Work;
extern u8 _021E6E7C[];
extern int TouchscreenHitbox_FindRectAtTouchNew(void*);extern u32 GridInputHandler_HandleInput_AllowHold(void*),GridInputHandler_GetNextInput(void*);extern void ov64_021E64F8(Work*),PlaySE(int),ov64_021E677C(Work*,u32),ov64_021E652C(Work*,int),ov64_021E62A8(Work*);extern int ov64_021E6B84(Work*);
int ov64_021E62C8(Work*);
static inline void previous(Work*w){w->message=15;w->next=2;w->state=1;ov64_021E64F8(w);PlaySE(1500);}
static inline void next(Work*w){w->message=16;w->next=3;w->state=1;ov64_021E64F8(w);PlaySE(1500);}
int ov64_021E62C8(Work*w){u32 input;switch(w->state){case 0:{int hit=TouchscreenHitbox_FindRectAtTouchNew(_021E6E7C);if(hit==0){if(w->page>0)previous(w);}else if(hit==1){if(w->count>=1 &&w->page!=w->count-1)next(w);}else{input=GridInputHandler_HandleInput_AllowHold(w->grid);switch(input){case 0:case 1:case 2:case 3:case 4:case 5:ov64_021E677C(w,input);if(!(gSystem.newKeys&1)&&input<w->limit)PlaySE(1500);break;case 6:case 0xfffffffe:PlaySE(1500);w->message=17;w->next=4;w->state++;break;case 0xfffffffd:ov64_021E677C(w,GridInputHandler_GetNextInput(w->grid));PlaySE(1500);break;case 0xffffffff:input=GridInputHandler_GetNextInput(w->grid);if(gSystem.newAndRepeatedKeys&16){if((input==2||input==5)&&w->count>=1 &&w->page!=w->count-1)next(w);}else if(gSystem.newAndRepeatedKeys&32){if((input==0||input==3)&&w->page>0)previous(w);}break;case 0xfffffffc:break;}}break;}case 1:if(!ov64_021E6B84(w))w->state=w->next;break;case 2:ov64_021E652C(w,-1);w->state=0;break;case 3:ov64_021E652C(w,1);w->state=0;break;case 4:ov64_021E62A8(w);PlaySE(1549);return 0;}return 1;}
