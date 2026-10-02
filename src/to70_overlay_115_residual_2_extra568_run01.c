#pragma force_active on
#include "global.h"
typedef struct {s32 x,y,z;} Vec;typedef struct {u8 raw[20];} Anim;
typedef struct {s16 timer,count;void*sprites[4];Anim anims[4];} Work;
extern int ov01_021EFE80(Anim*);extern void ov01_021F074C(Vec*,s32,s32,s32),Sprite_SetAffineScale(void*,Vec*),Sprite_SetDrawFlag(void*,int);
int ov115_0225F0B4(Work*);
int ov115_0225F0B4(Work*work){Work*w=work;int done=1;Vec v;int i,status;Anim*p;Work*q;Work*spr;if(w->count<4){done=0;w->timer--;if(w->timer<=0){w->timer=3;w->count++;}}i=0;if(w->count>0){p=w->anims;q=w;spr=q;do{status=ov01_021EFE80(p);ov01_021F074C(&v,*(s32*)q->anims,*(s32*)q->anims,*(s32*)q->anims);Sprite_SetAffineScale(spr->sprites[0],&v);Sprite_SetDrawFlag(spr->sprites[0],1);if(status==0)done=0;else if(i!=3)Sprite_SetDrawFlag(spr->sprites[0],0);i++;p++;q=(Work*)((u8*)q+20);spr=(Work*)((u8*)spr+4);}while(i<w->count);}return done;}
