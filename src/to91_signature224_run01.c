#pragma force_active on
#include "global.h"
#include "touch_sampling_internal.h"
#include "touchscreen.h"
#include "sound.h"
#pragma force_active on
typedef struct {u8 p[0x30c];u32 mode;u8 q[0x431a-0x310];u8 selected,unused,x[8],y[8];u8 lo:4,hi:4;u8 trailing[0x5cb0-0x432d];} W40292State;
extern const TouchscreenHitbox ov52_021E94BA[],ov52_021E94B2[];
extern void ov52_021E927C(void *,int),ov52_021E8CDC(void *,int),ov52_021E8CBC(void *,int),ov52_021E9364(void *);
extern BOOL PlaySE(u16);
void ov52_021E8BDC(void *);
void ov52_021E8BDC(void *data){W40292State *s=data;TouchOutput touches;int i;u8 *sample;int hit=TouchscreenHitbox_FindRectAtTouchNew(ov52_021E94BA);if(hit!=-1){if(hit==0){if(s->mode==1){ov52_021E927C(s,10);s->mode=2;ov52_021E8CDC((u8*)s+0x250,1);PlaySE(1501);}}else{s->selected=hit;ov52_021E8CBC((u8*)s+0x250,hit);}}if(TouchscreenHitbox_FindRectAtTouchHeld(ov52_021E94B2)!=-1)ov52_021E9364(s);if(sub_02021280(&touches,4,1)==1){i=0;if(i<touches.count){sample=(u8*)&touches;do{s->x[i]=*(u16*)(sample+2);s->y[i]=*(u16*)(sample+4);i++;sample+=8;}while(i<touches.count);}s->hi=(u8)touches.count;s->lo=s->selected;}}
