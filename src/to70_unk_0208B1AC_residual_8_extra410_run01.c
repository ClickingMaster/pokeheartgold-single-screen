#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[634];u8 value;u8 pad27b;u16 mask;u8 gap[2];u32 bits0:28;u32 bit28:1;u32 bit29:1;u32 bit30:2;u8 tail[384];void*sprites[54];u8 gap2[740];u8 state,a,b,c;}W;extern void Sprite_SetDrawFlag(void*,int),Sprite_SetAnimationFrame(void*,int),Sprite_SetAnimCtrlSeq(void*,int),thunk_Sprite_SetDrawFlag(void*,int);void sub_0208BCD4(W*),sub_0208BD38(W*);
void sub_0208BCD4(W*w){u32 i;w->state=0;w->c=0;w->b=0;if(w->value==0)w->a=0;else if(w->value==255)w->a=12;else w->a=(12*w->value)>>8;for(i=29;i<=40;i++){Sprite_SetDrawFlag(w->sprites[i],0);Sprite_SetAnimationFrame(w->sprites[i],0);}}
void sub_0208BD38(W*w){u32 i;for(i=0;i<6;i++){if(w->mask&(1<<i))Sprite_SetAnimCtrlSeq(w->sprites[23+i],1);else Sprite_SetAnimCtrlSeq(w->sprites[23+i],0);}if(w->bit29==1&&w->bit28==0)thunk_Sprite_SetDrawFlag(w->sprites[52],1);else thunk_Sprite_SetDrawFlag(w->sprites[52],0);if(w->bit30==2)thunk_Sprite_SetDrawFlag(w->sprites[53],1);else thunk_Sprite_SetDrawFlag(w->sprites[53],0);}
