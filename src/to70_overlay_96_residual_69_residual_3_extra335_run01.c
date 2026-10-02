#pragma force_active on
#include "global.h"
#include "sprite.h"
#pragma force_active on
extern const VecFx32 ov96_0221C3E0;
void sub_02013728(void *);
void ov96_021F8C88(int,int,Sprite **),ov96_021F8CFC(int,int,Sprite **);
void ov96_021F8C88(int x,int y,Sprite **s){VecFx32 p=ov96_0221C3E0;p.y+=y<<12;p.x=(x+128)<<12;Sprite_SetMatrix(s[0],&p);p.x=(x+16)<<12;Sprite_SetMatrix(s[1],&p);p.x=(x+48)<<12;Sprite_SetMatrix(s[2],&p);p.y-=16384;Sprite_SetMatrix(s[3],&p);sub_02013728(s[4]);sub_02013728(s[8]);}
void ov96_021F8CFC(int x,int y,Sprite **s){VecFx32 p=*Sprite_GetMatrixPtr(s[0]);p.x+=x<<12;p.y+=y<<12;Sprite_SetMatrix(s[0],&p);p=*Sprite_GetMatrixPtr(s[1]);p.x+=x<<12;p.y+=y<<12;Sprite_SetMatrix(s[1],&p);p=*Sprite_GetMatrixPtr(s[3]);p.x+=x<<12;p.y+=y<<12;Sprite_SetMatrix(s[3],&p);Sprite_SetMatrix(s[2],&p);sub_02013728(s[4]);sub_02013728(s[8]);}
