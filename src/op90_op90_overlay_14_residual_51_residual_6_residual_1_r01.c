#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {u8 prefix[0x2fc];ManagedSprite *sprites[64];u8 beforeImages[0x5c];u8 images[30][512];u8 draw[30],palette[30],slots[64];} Icons;
typedef struct {u8 prefix[8];u32 mode;} Config;
typedef struct {Config *config;u8 gap[0x30];Icons *icons;} State;
void ov14_021F2E78(void *,void *,int,int);void ov14_021F34DC(void *,u16);
void ov14_021F3210(void *,s32);
// @fn ov14_021F3210
void ov14_021F3210(void *parent,s32 amount){State *state=parent;
s16 wrap; s16 stop; u16 i; struct {s16 y,x;} position;
if(amount>=0){stop=176;wrap=-8;}else{stop=-8;wrap=176;}
 for(i=0;i<30;i++){
  int index=state->icons->slots[i];
  ManagedSprite_GetPositionXY(state->icons->sprites[index],&position.x,&position.y);position.x+=amount;
  if(position.x==stop){position.x=wrap;ov14_021F2E78(state->icons,state->icons->images[i],index,state->icons->palette[i]);ManagedSprite_SetDrawFlag(state->icons->sprites[index],state->icons->draw[i]);if(state->config->mode==3)ov14_021F34DC(parent,i);}
  ManagedSprite_SetPositionXY(state->icons->sprites[index],position.x,position.y);
 }
}
