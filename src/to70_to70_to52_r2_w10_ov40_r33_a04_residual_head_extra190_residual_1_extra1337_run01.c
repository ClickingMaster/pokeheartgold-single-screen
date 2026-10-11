#pragma force_active on
#include "global.h"
typedef struct {u32 v[4];} Table;
extern Table ov40_022454C0,ov40_022454D0;
extern void ov40_0222D78C(void*,int),ov40_0222D5AC(void*,void*,int),ov40_0222D66C(void*,void*,u32),ov40_0223B29C(void*,void*,int),ManagedSprite_SetPositionXY(void*,s16,s16),ManagedSprite_SetAnim(void*,u32),TextOBJ_SetPosition(void*,int,int),TextOBJ_SetSpritesDrawFlag(void*,int);
extern void *ov40_0222D800(void*,int);
void ov40_0223B374(u8*);
void ov40_0223B374(u8 *w){u8 *p=*(u8**)(w+2144),*item;Table text=ov40_022454C0,anims=ov40_022454D0;struct {volatile int y;u32 *t;void *volatile ctx;} vars;u32 *a;int i;
ov40_0222D78C(w,2);item=p+276;vars.t=text.v;vars.y=105;i=0;vars.ctx=w;a=anims.v;vars.ctx=w+20;for(;i<4;i++){*(void**)(p+280)=ov40_0222D800(w,2);ov40_0222D5AC(item,vars.ctx,2);ov40_0222D66C(item,vars.ctx,*vars.t);if(i!=3)ov40_0223B29C(item,w,i+1);ManagedSprite_SetPositionXY(*(void**)(p+280),50,vars.y);ManagedSprite_SetAnim(*(void**)(p+280),*a);TextOBJ_SetPosition(*(void**)(p+284),36,-8);TextOBJ_SetSpritesDrawFlag(*(void**)(p+284),1);vars.t++;{int next=vars.y+36;p+=28;item+=28;a++;vars.y=next;}}}
