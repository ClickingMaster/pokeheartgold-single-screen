#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 gap[0xdc];void*sys;void*man;};void SpriteSystem_LoadCharResObjFromOpenNarc(void*,void*,void*,int,int,int,int);void SpriteSystem_LoadCellResObjFromOpenNarc(void*,void*,void*,int,int,int);void SpriteSystem_LoadAnimResObjFromOpenNarc(void*,void*,void*,int,int,int);void ov57_02239114(struct S*,void*);
void ov57_02239114(struct S*s,void*n){SpriteSystem_LoadCharResObjFromOpenNarc(s->sys,s->man,n,14,1,2,45000);SpriteSystem_LoadCellResObjFromOpenNarc(s->sys,s->man,n,16,1,47000);SpriteSystem_LoadAnimResObjFromOpenNarc(s->sys,s->man,n,15,1,48000);}
