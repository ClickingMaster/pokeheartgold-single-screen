#pragma force_active on
#include "global.h"
extern u8 sBagSpriteTemplates[];
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *,s16,s16,int),ManagedSprite_SetDrawFlag(void *,int),BagApp_SetItemIconSprite(void *,int,int),BagApp_UpdatePageArrows(void *);
typedef struct {u8 pad[0x254];void *sprites[6];u8 pad2[0x6a4-0x26c];u16 values[1];} State;
typedef struct {u8 pad[6];s16 index;} Param;

void BagApp_DrawItemIcons(State *s,Param *p,int count,int update);
void BagApp_ShowMovingItemIcon(State *s,Param *p,int value);

void BagApp_DrawItemIcons(State *s,Param *p,int count,int update){int i;State *cursor;u8 *pos=sBagSpriteTemplates;for(i=0,cursor=s;i<6;i++,pos+=0x34,cursor=(State *)((u8 *)cursor+4)){ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[0],*(s16 *)(pos+0x34),*(s16 *)(pos+0x36),0x100000);if(i<count){if(update)BagApp_SetItemIconSprite(s,i,s->values[p->index+i]);ManagedSprite_SetDrawFlag(cursor->sprites[0],1);}else ManagedSprite_SetDrawFlag(cursor->sprites[0],0);}BagApp_UpdatePageArrows(s);}
void BagApp_ShowMovingItemIcon(State *s,Param *p,int value){int selected=-1;int i;u8 *pos;State *cursor;if(p->index==(value/6)*6)selected=value%6;pos=sBagSpriteTemplates;for(i=0,cursor=s;i<6;i++,pos+=0x34,cursor=(State *)((u8 *)cursor+4)){ManagedSprite_SetPositionXYWithSubscreenOffset(cursor->sprites[0],*(s16 *)(pos+0x34),*(s16 *)(pos+0x36),0x100000);if(i==selected)ManagedSprite_SetDrawFlag(cursor->sprites[0],1);else ManagedSprite_SetDrawFlag(cursor->sprites[0],0);}BagApp_UpdatePageArrows(s);}
