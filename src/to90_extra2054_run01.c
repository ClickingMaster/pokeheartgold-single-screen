#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 part[3][0x4c];int angle,step,ticks;u8 frame,done,delay,counter,index;} State;
extern const u8 ov93_02262CC4[];
extern const u8 ov93_02262CC5[];
extern void ov93_02261528(void *,int);
extern void ov93_02261538(void *,State *,void *);
extern void ov93_02262540(void *,void *,void *);
void ov93_02261354(void *,State *);
void ov93_02261354(void *ctx,State *s){
 int sum=0,i;
 if(s->done==1)return;
 s->ticks++;
 if(s->delay==0){
  s->angle+=s->step;s->counter++;
  if(s->counter>=ov93_02262CC4[s->index*4]){
   s->counter=0;s->frame++;s->angle=s->frame*0x1e000;s->delay=ov93_02262CC5[s->index*4];
  }
 }else {
  s->delay--;
  if(s->delay==0&&s->frame>=12){
   s->frame=0;s->index++;if(s->index>=5)s->index=4;
   s->step=0x1e000/ov93_02262CC4[s->index*4];
  }
 }
 if(s->index==1){
  { const u8 *p=ov93_02262CC4;for(i=0;i<2;i++,p+=4)sum+=(p[0]+p[1])*12; }
  if(sum-15==s->ticks)ov93_02261528(s->part[1],1);
 }else if(s->index==2){
  { const u8 *p=ov93_02262CC4;for(i=0;i<3;i++,p+=4)sum+=(p[0]+p[1])*12; }
  if(sum-15==s->ticks)ov93_02261528(s->part[2],1);
 }
 for(i=0;i<3;i++){
  ov93_02261538(ctx,s,s->part[i]);ov93_02262540(ctx,s->part[i],s->part[i]+0x30);
 }
}
