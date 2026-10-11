#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {u8 prefix[0x2fc];ManagedSprite *sprites[64];u8 beforeImages[0x5c];u8 images[30][512];u8 draw[30],palette[30],slots[64];} Icons;
typedef struct {u8 prefix[8];u32 mode;} Config;
typedef struct {Config *config;u8 gap[0x30];Icons *icons;} State;
void PCBox_SetIconImage(void *,void *,int,int);void PCBox_UpdateSlotIconItemTranslucency(void *,u16);
void PCBox_ScrollBoxIcons(void *,s32);
// @fn PCBox_ScrollBoxIcons
void PCBox_ScrollBoxIcons(void *parent,s32 amount){State *state=parent;
s16 wrap; s16 stop; u16 i; struct {s16 y,x;} position;
if(amount>=0){stop=176;wrap=-8;}else{stop=-8;wrap=176;}
 for(i=0;i<30;i++){
  int index=state->icons->slots[i];
  ManagedSprite_GetPositionXY(state->icons->sprites[index],&position.x,&position.y);position.x+=amount;
  if(position.x==stop){position.x=wrap;PCBox_SetIconImage(state->icons,state->icons->images[i],index,state->icons->palette[i]);ManagedSprite_SetDrawFlag(state->icons->sprites[index],state->icons->draw[i]);if(state->config->mode==3)PCBox_UpdateSlotIconItemTranslucency(parent,i);}
  ManagedSprite_SetPositionXY(state->icons->sprites[index],position.x,position.y);
 }
}
