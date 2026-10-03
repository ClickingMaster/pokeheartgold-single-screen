#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x54];int y;} Position;
typedef struct{int state,index,ticks;Position*position;} Work;
extern const int _021EAA80[];
void ov85_021E8374(void*,Work*);
void ov85_021E8374(void*task,Work*w){switch(w->state){case 0:break;case 1:w->position->y=_021EAA80[w->index]<<12;w->index++;if(w->index>=16){w->index=0;w->state++;}break;case 2:w->ticks++;if(w->ticks>=15){w->ticks=0;w->state=1;}break;}}
