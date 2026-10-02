#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 p[0x294];u8 enabled[5];u8 mode;u8 p29A[0x5b8-0x29a];void*sprites[6];u8 p5D0[0x7bc-0x5d0];s8 state;};void Sprite_SetDrawFlag(void*,int);void sub_0208B9C8(struct S*);
void sub_0208B9C8(struct S*s){int i;if(s->state==0){if(s->mode){Sprite_SetDrawFlag(s->sprites[5],1);for(i=0;i<5;i++)Sprite_SetDrawFlag(s->sprites[i],0);}else{for(i=0;i<5;i++){if(s->enabled[i])Sprite_SetDrawFlag(s->sprites[i],1);else Sprite_SetDrawFlag(s->sprites[i],0);}Sprite_SetDrawFlag(s->sprites[5],0);}}else{for(i=0;i<6;i++)Sprite_SetDrawFlag(s->sprites[i],0);}}
