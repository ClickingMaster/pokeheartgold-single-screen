#pragma force_active on
#include "global.h"
#pragma force_active on
struct Mon{u32 species,pad;u16 form,gender;};struct Slot{u32 tile,palette;void*sprite;u8 data[512];};struct Img{u8 pad[20];void*data;};int GetMonIconNaixEx(u32,int,int);int GetMonIconPaletteEx(u32,int,int);void sub_02094C6C(int,struct Img**,int,int);void Sprite_SetPaletteOverride(void*,int);void sub_02094C08(struct Mon*,struct Slot*,int,void*,int,int,int);
void sub_02094C08(struct Mon*m,struct Slot*slots,int index,void*sprite,int a,int b,int tile){struct Slot*s=&slots[index];if(sprite){struct Img*img;sub_02094C6C(GetMonIconNaixEx(m->species,m->form,m->gender),&img,a,b);MIi_CpuCopyFast((const u32*)img->data,(u32*)s->data,512);s->tile=(tile+(index<<4))<<5;s->sprite=sprite;s->palette=GetMonIconPaletteEx(m->species,m->gender,m->form)+8;Sprite_SetPaletteOverride(sprite,s->palette);}else s->sprite=0;}
