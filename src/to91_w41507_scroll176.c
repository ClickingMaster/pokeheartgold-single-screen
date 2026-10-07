#include "global.h"
#pragma force_active on
#include "bg_window.h"
#include "sprite_system.h"
typedef struct{void*x;ManagedSprite*sprites[2];Window windows[6];void *active;s16 target,pad1,current;u16 tail;} Work;

void ov111_021E65D4(void *argument);

void ov111_021E65D4(void *argument){Work*w=argument;BgConfig*bg;Window*window;u8 id;int i;int step;struct {s16 y,x;} pos;if(w==0)GF_AssertFail();window=&w->windows[0];bg=GetWindowBgConfig(window);id=GetWindowBgId(window);{int current=w->current,target=w->target;int d=current-target;if(d<0)d=-d;d=(s16)d;step=d>>2;if(d<=1 || step==0)w->active=NULL;else{if(current>target)step=(s16)(~0u * (u32)step);w->current+=step;if(w->current<232){ScheduleSetBgPosText(bg,id,BG_POS_OP_SUB_Y,step);for(i=0;i<2;i++){ManagedSprite_GetPositionXYWithSubscreenOffset(w->sprites[i],&pos.x,&pos.y,0x20c000);pos.y+=step;ManagedSprite_SetPositionXYWithSubscreenOffset(w->sprites[i],pos.x,pos.y,0x20c000);}}}}BgCommitTilemapBufferToVram(bg,id);}
