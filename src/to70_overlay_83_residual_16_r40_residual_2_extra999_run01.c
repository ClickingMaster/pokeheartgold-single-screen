#pragma force_active on
#include "global.h"
typedef struct {u8 pad[15];u8 ticks;u16 x;u8 value,y,pad2,extra;u8 pad3[0x7e7];u8 remote;} Work;
extern int Comm_GetLocalNetId(void);

void ov83_02241474(int id,int unused,u16*data,Work*w);

void ov83_02241474(int id,int unused,u16*data,Work*w){w->ticks++;if(id!=Comm_GetLocalNetId()){w->remote=data[1];if(Comm_GetLocalNetId()==0){if(w->value!=255){w->remote=0;return;}w->value=w->remote+w->extra;w->x=data[4];w->y=data[5];}else{w->value=data[2];w->x=data[4];w->y=data[5];}}}
