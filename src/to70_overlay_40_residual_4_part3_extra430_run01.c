#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 mode;u8 pad[16];void *manager,*sys,*narc;} State;
extern void SpriteSystem_LoadCharResObjFromOpenNarc(void *,void *,void *,int,int,int,int),SpriteSystem_LoadCellResObjFromOpenNarc(void *,void *,void *,int,int,int),SpriteSystem_LoadAnimResObjFromOpenNarc(void *,void *,void *,int,int,int);
void ov40_0222CAD8(State *);
void ov40_0222CAD8(State *s){void *manager=s->manager,*sys=s->sys,*narc=s->narc;int chars,cells,anims;switch(s->mode){case 0:chars=63;cells=24;anims=25;break;case 2:chars=17;cells=18;anims=19;break;case 5:case 6:chars=8;cells=9;anims=10;break;case 4:chars=11;cells=12;anims=13;break;case 3:chars=14;cells=15;anims=16;break;default:chars=48;cells=22;anims=23;break;}SpriteSystem_LoadCharResObjFromOpenNarc(sys,narc,manager,chars,0,1,10001);SpriteSystem_LoadCharResObjFromOpenNarc(sys,narc,manager,chars,0,2,10002);SpriteSystem_LoadCellResObjFromOpenNarc(sys,narc,manager,cells,0,10001);SpriteSystem_LoadCellResObjFromOpenNarc(sys,narc,manager,cells,0,10002);SpriteSystem_LoadAnimResObjFromOpenNarc(sys,narc,manager,anims,0,10001);SpriteSystem_LoadAnimResObjFromOpenNarc(sys,narc,manager,anims,0,10002);}
