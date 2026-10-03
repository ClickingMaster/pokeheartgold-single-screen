#pragma force_active on
#include "global.h"
typedef struct {u8 p[12];void*s[12];} Work;typedef struct {u8 p[0x1c];void*manager;} Context;
extern void SpriteManager_UnloadPlttObjById(void*,int),SpriteManager_UnloadCellObjById(void*,int),SpriteManager_UnloadAnimObjById(void*,int),SpriteManager_UnloadCharObjById(void*,int),Sprite_DeleteAndFreeResources(void*),ov40_02230398(Work*,Context*);
void ov40_0223064C(Work*,Context*);
void ov40_0223064C(Work*w,Context*c){int i;Work*p;SpriteManager_UnloadPlttObjById(c->manager,100000);SpriteManager_UnloadCellObjById(c->manager,100000);SpriteManager_UnloadAnimObjById(c->manager,100000);i=0;p=w;do{if(p->s[0]){SpriteManager_UnloadCharObjById(c->manager,i+100000);Sprite_DeleteAndFreeResources(p->s[0]);p->s[0]=0;}i++;p=(Work*)((u8*)p+4);}while(i<12);ov40_02230398(w,c);}
