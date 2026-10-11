#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x248];void *sys,*man;}Work;
extern int GetItemIndexMapping(int,int);extern void SpriteSystem_ReplaceCharResObj(void*,void*,int,int,int,int),SpriteSystem_ReplacePlttResObj(void*,void*,int,int,int,int);void BagApp_SetItemIconSprite(Work*,int,int);
void BagApp_SetItemIconSprite(Work*w,int base,int item){SpriteSystem_ReplaceCharResObj(w->sys,w->man,18,GetItemIndexMapping(item,1),0,base+0xc0fc);SpriteSystem_ReplacePlttResObj(w->sys,w->man,18,GetItemIndexMapping(item,2),0,base+0xc0fb);}
