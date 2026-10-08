#pragma force_active on
#include "global.h"
typedef struct {u32 old,current,next;} Angle;
typedef struct {u8 pad[0x208];s16 x,y;int enabled,rate;u8 pad2[8];Angle angles[4];u16 count:15;u16 started:1;} Work;
extern void ov28_0225DD58(Work *),ov28_0225DEB0(Work *),ov28_0225E0F4(Work *),ov28_0225E060(Work *),ov28_0225DFBC(Work *),ov28_0225DF14(Work *);
extern void (*const ov28_0225EAAC[])(Angle *);
void ov28_0225E1A4(Work *);
// @fn ov28_0225E1A4
void ov28_0225E1A4(Work *w){
u32 i;s16 x,y;
ov28_0225DD58(w);
if(w->enabled==0){ov28_0225DEB0(w);w->rate=0;}else{x=abs(w->x);y=abs(w->y);if(x<=8&&y<=8){ov28_0225E0F4(w);w->rate=4;}else if(x<=12&&y<=12){ov28_0225E060(w);w->rate=3;}else if(x<=17&&y<=17){ov28_0225DFBC(w);w->rate=2;}else{ov28_0225DF14(w);w->rate=1;}}
if(!w->started){for(i=0;i<4;i++){w->angles[i].old=w->angles[i].current;w->angles[i].next=w->angles[i].current;}w->started=1;w->count=1;}
else{for(i=0;i<4;i++){w->angles[i].current%=360;w->angles[i].old%=360;if(abs((s32)(w->angles[i].current-w->angles[i].old))>=180){if(w->angles[i].current>w->angles[i].old)w->angles[i].old+=360;else if(w->angles[i].current<w->angles[i].old)w->angles[i].current+=360;}ov28_0225EAAC[w->rate](&w->angles[i]);}}
}
