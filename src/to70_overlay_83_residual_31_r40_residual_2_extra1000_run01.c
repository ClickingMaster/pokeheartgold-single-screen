#pragma force_active on
#include "global.h"
typedef struct {u8 pad[17];u8 value,x,y,pad2,extra,pad3,ticks;u8 pad4[0x59d];u8 remote;} Work;
extern int sub_0203769C(void);

void ov83_022451B8(int id,int unused,u16*data,Work*w);

void ov83_022451B8(int id,int unused,u16*data,Work*w){w->ticks++;if(id!=sub_0203769C()){w->remote=data[1];if(sub_0203769C()==0){if(w->value!=255){w->remote=0;return;}w->value=w->remote+w->extra;w->x=data[4];w->y=data[5];}else{w->value=data[2];w->x=data[4];w->y=data[5];}}}
