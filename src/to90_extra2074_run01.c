#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 state;s16 timer,layer;u8 value;} State;
extern void ov91_022614FC(void *,s16,u8);
void ov91_022613C8(State *,void *);
void ov91_022613C8(State *s,void *bg){
 int layer,older,prev,alpha;
 switch(s->state){
 case 1:ov91_022614FC(bg,(s16)(s->layer-1),s->value);s->state++;s->timer=0;
 case 2:
  if(s->timer>=16){s->state=3;s->timer=0;ov91_022614FC(bg,s->layer,s->value);G2x_SetBlendAlpha_(0x04001050,1<<s->layer,8,0,16);return;}
  s->timer++;layer=s->layer;older=layer-2;prev=layer-1;
  if(layer<0)layer+=3;if(older<0)older+=3;if(prev<0)prev+=3;
  alpha=16-(s->timer*16)/16;
  G2x_SetBlendAlpha_(0x04001050,1<<layer,8|((1<<older)|(1<<prev)),alpha,16-alpha);return;
 case 3:
  if(s->timer>=8){s->state=0;*(volatile u16 *)0x04001050=0;return;}
  s->timer++;alpha=(s->timer*16)/8;
  G2x_SetBlendAlpha_(0x04001050,1<<s->layer,8,alpha,16-alpha);break;
 case 0:default:break;
 }
}
