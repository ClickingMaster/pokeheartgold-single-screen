#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 state;u8 active;s8 frame;} State;
void ov91_02261A00(State *,void *,void *),ov91_02261200(void *,int),ov91_022611D0(void *),ov91_02260364(void *,int),GF_SndHandleSetTempoRatio(int,int),PlaySE(u16),StartBrightnessTransition(int,int,int,int,int);
int IsBrightnessTransitionActive(int);
void ov91_02261A00(State *s,void *a,void *b){if(!s->active)return;switch(s->state){case 0:s->frame++;ov91_02261200(a,s->frame*102400/58+4096);GF_SndHandleSetTempoRatio(7,s->frame*150/58+256);if(s->frame==12)PlaySE(1415);if(s->frame>=58)s->state++;break;case 1:StartBrightnessTransition(4,16,0,63,1);s->state++;break;case 2:if(IsBrightnessTransitionActive(1)==1){ov91_022611D0(a);ov91_02261200(a,8192);ov91_02260364(b,1);s->state++;}break;case 3:StartBrightnessTransition(14,0,16,63,1);s->state++;break;case 4:if(IsBrightnessTransitionActive(1)==1)s->state++;break;case 5:s->active=0;s->state=0;break;}}
