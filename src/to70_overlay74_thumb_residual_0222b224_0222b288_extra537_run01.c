#pragma force_active on
#include "global.h"
typedef struct {void *sprite;u8 pad[72];} Item;
typedef struct {u8 p[0x94];Item a[80],b[80];void*c;u8 q[72];void*d;} W;
extern void Sprite_UpdateAnim(void*,int);extern void *ov74_0222B0C8(W*,int);extern void Sprite_TryChangeAnimSeq(void*,int);extern void Sprite_SetAnimationFrame(void*,int);
void ov74_0222B224(W*);void ov74_0222B288(W*);
void ov74_0222B224(W*w) {int i;W*p;i=0;p=w;for(;i<80;i++,p=(W*)((u8*)p+76)){if(p->a[0].sprite)Sprite_UpdateAnim(p->a[0].sprite,8192);if(p->b[0].sprite)Sprite_UpdateAnim(p->b[0].sprite,8192);}if(w->c)Sprite_UpdateAnim(w->c,8192);if(w->d)Sprite_UpdateAnim(w->d,8192);}
void ov74_0222B288(W*w) {int i;W*p;i=0;p=w;for(;i<80;i++,p=(W*)((u8*)p+76)){p->a[0].sprite=ov74_0222B0C8(w,1);Sprite_TryChangeAnimSeq(p->a[0].sprite,1);Sprite_SetAnimationFrame(p->a[0].sprite,0);}}
