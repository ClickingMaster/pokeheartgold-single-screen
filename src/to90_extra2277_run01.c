#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x310];void *sprites[6];} Context;
extern BOOL ManagedSprite_GetDrawFlag(void *);extern void ManagedSprite_OffsetPositionXY(void *,s16,s16);
void BattleBagMenu_OffsetButtonSprite(Context *,u32,u32);
#pragma force_active on
void BattleBagMenu_OffsetButtonSprite(Context *p,u32 type,u32 mode){void *sprite;u8 i;if(type>=6&&type<=11)sprite=((Context*)((u8*)p+((type-6)<<2)))->sprites[0];else if(type==4){for(i=0;i<6;i++){sprite=p->sprites[i];if(ManagedSprite_GetDrawFlag(sprite))break;}}else return;switch(mode){case 0:case 2:ManagedSprite_OffsetPositionXY(sprite,0,2);break;case 1:ManagedSprite_OffsetPositionXY(sprite,0,-4);break;}}
