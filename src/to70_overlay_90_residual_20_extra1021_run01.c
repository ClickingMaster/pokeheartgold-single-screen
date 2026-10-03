#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct {u8 a,mode;u8 pad[2];void *prompt,*bg;int layer,tile,palette;u8 x,y,z,flag;u8 t,enabled;u8 q[2];} W;
extern void *YesNoPrompt_Create(int);void ov90_0225B340(W*,void**,int,int);
void ov90_0225B340(W*w,void**bg,int mode,int heap) {memset(w,0,32);w->bg=*bg;w->layer=4;w->tile=139;w->palette=2;w->x=24;w->y=8;w->flag=0;w->prompt=YesNoPrompt_Create(heap);w->mode=mode;w->enabled=1;}
