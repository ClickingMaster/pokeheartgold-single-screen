#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void*context;u8 total,current,pad,saved;u32 origin;VecFx32 delta;u8 gap[16];BOOL active;u16 position,reverse;} State;
extern u32 ov01_02209B60;
void ov01_021EAE50(void*,void*,void*,u8,u8);void ov01_021EAEA4(void*,VecFx32*,u8,u8);void ov01_021EAD8C(State*);
void ov01_021EAD8C(State*s){if(s->active){if(!s->reverse){s->current++;ov01_021EAE50(s->context,&s->position,&s->origin,s->current,s->total);ov01_021EAEA4(s->context,&s->delta,s->current,s->total);if(s->current>=s->total)s->active=0;}else{VecFx32 v=s->delta;v.x*= -1;v.y*= -1;v.z*= -1;ov01_021EAE50(s->context,&s->origin,&s->position,(u8)(s->total-s->current),s->total);ov01_021EAEA4(s->context,&v,s->current,s->total);s->current--;if(s->current==0){if(ov01_02209B60)ov01_02209B60=0;else ov01_02209B60=s->saved;s->reverse=0;s->active=0;}}}}
