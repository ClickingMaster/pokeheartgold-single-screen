#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0xa4];s32 position;u8 p[4];u8 direction;u8 q[0x17];s32 width;} Context;
extern BOOL ov01_021F4728(s32,s32,s32);
u8 ov01_021F635C(s32 target,s32 ignored,Context *p);
#pragma force_active on
u8 ov01_021F635C(s32 target,s32 ignored,Context *p){
 if(p->position==target)return p->direction;
 GF_ASSERT(target!=-1);
 switch(p->direction){
 case 0:
 if(p->width==target-p->position)return p->direction+2;
 if(target-p->position==1&&ov01_021F4728(p->position,target,p->width))return p->direction+1;
 if(p->width+1==target-p->position&&!ov01_021F4728(p->position,target,p->width))return p->direction+3;
 return 4;
 case 1:
 if(p->width==target-p->position)return p->direction+2;
 if(p->position-target==1&&ov01_021F4728(p->position,target,p->width))return p->direction-1;
 if(p->width-1==target-p->position&&!ov01_021F4728(p->position,target,p->width))return p->direction+1;
 return 4;
 case 2:
 if(p->width==p->position-target)return p->direction-2;
 if(p->width-1==p->position-target&&!ov01_021F4728(p->position,target,p->width))return p->direction-1;
 if(target-p->position==1&&ov01_021F4728(p->position,target,p->width))return p->direction+1;
 return 4;
 case 3:
 if(p->width==p->position-target)return p->direction-2;
 if(p->width+1==p->position-target&&!ov01_021F4728(p->position,target,p->width))return p->direction-3;
 if(p->position-target==1&&ov01_021F4728(p->position,target,p->width))return p->direction-1;
 return 4;
 }
 return 4;
}
