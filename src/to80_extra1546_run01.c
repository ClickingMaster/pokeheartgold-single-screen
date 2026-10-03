#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int x,y,z;}V;typedef struct{void*pad,*sprite;u8 pad2[0x2c];int y;}S;extern const u16 ov72_0223B4C0[],ov72_0223B4C2[];extern void Sprite_SetMatrix(void*,V*);
void ov72_0223B2FC(S*s,int i);
void ov72_0223B2FC(S*s,int i){V v;v.x=ov72_0223B4C0[i*2]<<12;v.y=(s->y+ov72_0223B4C2[i*2])<<12;v.z=0;Sprite_SetMatrix(s->sprite,&v);}
