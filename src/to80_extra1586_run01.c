#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work {void *actor;u32 state,phase;u8 timer,tick,dir,pad;int x,y,top,bottom;void *sprite0,*sprite1,*sprite2;u8 p0[8];u8 mode;u8 p1[3];u16 time1,time2,c3c,c3e;} Work;
extern s8 ov96_0221C55C[][9],ov96_0221C589[][9],ov96_0221DC28[];
extern u8 ov96_0221DC2C[];
extern void ov96_021FB8FC(Work *,int);
extern void ov96_021FB8B4(Work *,int);
extern void PokeathlonCourseMonObject_SetMonPosition(void *,int,int);
extern void PokeathlonCourseMonObject_SetPosition(void *,int,int,int);
extern void PokeathlonCourseMonObject_SetMonDrawFlag(void *,int);
extern void PlaySEOnPlayer(int,int);
extern void SetPlayerPan(int,int);
extern void PokeathlonCourseMonObject_SetAnimState(void *,int);
extern void PokeathlonCourseMonObject_Move(void *,int,int,int,int);
extern void *PokeathlonCourseSpritePoolSprite_GetSprite(void *);
extern const VecFx32 *Sprite_GetMatrixPtr(void *);
extern void Sprite_SetMatrix(void *,const VecFx32 *);
extern void PokeathlonCourseMonObject_GetSpritePosition(void *,int *,int *);
void ov96_021FA83C(int,Work *);
void ov96_021FA83C(int index,Work *w){
 switch(w->state){
 case 1:
 w->timer++;
 switch(w->phase){
 case 1:{int offset=ov96_0221C55C[w->mode][w->timer-1];int y=w->y-offset;
 if(w->mode==0){ov96_021FB8FC(w,0);PokeathlonCourseMonObject_SetMonPosition(w->actor,w->x,y);}else{ov96_021FB8B4(w,offset);PokeathlonCourseMonObject_SetPosition(w->actor,w->x,y,0);}
 if(w->timer>=w->time1){w->phase=2;w->timer=0;if(w->mode==0)PokeathlonCourseMonObject_SetMonDrawFlag(w->actor,0);}break;}
 case 2:
 if(w->timer>=w->time2){w->phase=3;w->timer=0;if(w->mode==0){PokeathlonCourseMonObject_SetMonDrawFlag(w->actor,1);PlaySEOnPlayer(0x8aa,ov96_0221DC2C[index]);SetPlayerPan(ov96_0221DC2C[index],ov96_0221DC28[index]);}}break;
 case 3:{int offset=ov96_0221C589[w->mode][w->timer-1];int y=w->y-offset;
 if(w->mode==0){ov96_021FB8FC(w,1);PokeathlonCourseMonObject_SetMonPosition(w->actor,w->x,y);}else{ov96_021FB8B4(w,offset);PokeathlonCourseMonObject_SetPosition(w->actor,w->x,y,0);}
 if(w->timer>=w->time1){w->state=0;w->phase=0;w->timer=0;PokeathlonCourseMonObject_SetPosition(w->actor,w->x,w->y,0);if(w->mode!=0){PlaySEOnPlayer(0x8a8,ov96_0221DC2C[index]);SetPlayerPan(ov96_0221DC2C[index],ov96_0221DC28[index]);}ov96_021FB8B4(w,0);w->c3c=0;w->c3e=0;}
 else if(w->timer<w->time1-5)w->c3c=0;break;}
 default:GF_AssertFail();break;
 }break;
 case 2:
 w->timer++;
 if(w->tick==0){switch(w->dir){case 1:w->dir=3;break;case 3:w->dir=2;break;case 2:w->dir=4;break;case 4:w->dir=1;break;default:break;}PokeathlonCourseMonObject_SetAnimState(w->actor,w->dir);}
 w->tick=(w->tick+1)%4;
 if(w->timer>30){w->timer=0;w->state=0;w->phase=0;PokeathlonCourseMonObject_SetAnimState(w->actor,1);}break;
 case 3:{BOOL changed=FALSE;int delta;
 if(w->top<120){delta=1;if(w->top+1>120)delta=w->top-120;changed=TRUE;}
 else if(w->top>120){delta=-1;if(w->top-1<120)delta=120-w->top;changed=TRUE;}
 if(changed){w->top+=delta;w->bottom+=delta;PokeathlonCourseMonObject_Move(w->actor,0,delta,1,1);}break;}
 case 0:w->c3c=0;w->c3e=0;break;
 }
 {void *sprite=PokeathlonCourseSpritePoolSprite_GetSprite(w->sprite0);VecFx32 matrix=*Sprite_GetMatrixPtr(sprite);matrix.y=w->bottom<<12;Sprite_SetMatrix(sprite,&matrix);}
 {int x,y;void *sprite;VecFx32 matrix;PokeathlonCourseMonObject_GetSpritePosition(w->actor,&x,&y);sprite=PokeathlonCourseSpritePoolSprite_GetSprite(w->sprite2);matrix=*Sprite_GetMatrixPtr(sprite);matrix.y=y<<12;Sprite_SetMatrix(sprite,&matrix);}
}
