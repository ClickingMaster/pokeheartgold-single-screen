#include "global.h"
#include "sprite_system.h"
#include "font_sprite.h"
#pragma force_active on
typedef struct Choice {int enabled,reserved04,message,anim,action,reserved14;u32 selection;int reserved1c;const struct Choice *child;} Choice;
typedef struct SpriteRow {void *sprite;u8 gap04[16];void *text;u8 gap18[16];} SpriteRow;
typedef struct Scene {
 u8 gap0[8];int state;u8 gapc[16];void *manager;u8 gap20[8];void *palette;
 u8 gap2c[44];int color;u8 gap5c[0x4d8];SpriteRow left[5],right[5];
 void *path[5];int selected;u32 gap6dc;int count,choice;u8 gap6e8[8];void *cursor;
 u8 gap6f4[0x124];void *choices;u8 gap81c[0x44];void *animation;u8 remaining[0x4170-0x864];
} Scene;
extern void ov40_0222D3E8(u8*,u8*,int,u8*);
void ov40_0222CE7C(void*);
typedef char OwnerSize[(sizeof(Scene)==0x4170)?1:-1];
typedef char RowSize[(sizeof(SpriteRow)==40)?1:-1];
typedef char ChoiceSize[(sizeof(Choice)==36)?1:-1];
typedef char PathOffset[(offsetof(Scene,path)==0x6c4)?1:-1];
void ov40_0222CE7C(void *argument) {
 Scene *owner=argument;int i;
 for(i=0;i<5;i++) {
  ManagedSprite_SetDrawFlag(owner->right[i].sprite,((const Choice*)owner->choices)[i].enabled);
  if(((const Choice*)owner->choices)[i].enabled!=0) {
   ManagedSprite_SetAnim(owner->right[i].sprite,((const Choice*)owner->choices)[i].anim);
   ov40_0222D3E8((u8*)owner,(u8*)&owner->right[i],((const Choice*)owner->choices)[i].message,(u8*)&((const Choice*)owner->choices)[i]);
  }
 }
}
