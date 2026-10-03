#pragma force_active on
#include "global.h"
typedef struct{int x,y,flip;} Point;
typedef struct{Point p[4];} Table;
typedef struct{s16 x,y,z,rot;int priority,palette,vram;u32 res[6];int transfer,extra;} Template;
extern Table ov57_0223BE18;
extern void *SpriteSystem_NewSprite(void *,void *,Template *);extern void ManagedSprite_SetFlipMode(void *,int);
typedef struct{Table table;Template t;} Local;
void ov57_02239184(u32 *);
void ov57_02239184(u32 *w){Local data;data.table=ov57_0223BE18;int i;for(i=0;i<4;i++){data.t.x=data.table.p[i].x;data.t.y=data.table.p[i].y;data.t.z=0;data.t.rot=0;data.t.priority=40;data.t.palette=2;data.t.vram=2;data.t.transfer=1;data.t.extra=0;data.t.res[0]=0xafc8;data.t.res[1]=0x6594;data.t.res[2]=0xb798;data.t.res[3]=0xbb80;data.t.res[4]=-1;data.t.res[5]=-1;w[0xcf+i]=(u32)SpriteSystem_NewSprite((void *)w[0x37],(void *)w[0x38],&data.t);ManagedSprite_SetFlipMode((void *)w[0xcf+i],data.table.p[i].flip);}}
