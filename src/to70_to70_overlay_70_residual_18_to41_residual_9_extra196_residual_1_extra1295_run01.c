#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x24];s32 choice,a,b;} Menu;
extern u8 gSystem[];extern void PlaySE(int),Sprite_SetAnimCtrlSeq(void *,int);
extern int ov70_0223C2EC(void *);extern void Gts_SetSpritePosition(void *,int,int);
typedef struct {u16 value,pad[2];} Triple;
extern Triple ov70_022464FE[],ov70_02246500[],ov70_02246502[];
void ov70_0223C304(u8 *);
void ov70_0223C304(u8 *w) {
 int keys=*(u32*)(gSystem+0x48);Menu *m;int a,b;
 if(keys&64) {
  m=*(Menu**)(w+0x11c4);
  if(m->choice==0) {if(m->a>0){m->a--;PlaySE(1500);}}
  else {if(m->b>0){PlaySE(1500);(*(Menu**)(w+0x11c4))->b--;}}
 } else if(keys&128) {
  m=*(Menu**)(w+0x11c4);
  if(m->choice==0) {if(m->a<3){m->a++;PlaySE(1500);}}
  else {if(m->b<2){PlaySE(1500);(*(Menu**)(w+0x11c4))->b++;}}
 } else if(keys&16) {
  if((*(Menu**)(w+0x11c4))->choice!=1)PlaySE(1500);
  (*(Menu**)(w+0x11c4))->choice=1;
 } else if(keys&32) {
  if((*(Menu**)(w+0x11c4))->choice!=0)PlaySE(1500);
  (*(Menu**)(w+0x11c4))->choice=0;
 }
 a=ov70_0223C2EC(w);b=ov70_0223C2EC(w);
 Gts_SetSpritePosition(*(void**)(w+0xdcc),ov70_022464FE[a].value,ov70_02246500[b].value);
 Sprite_SetAnimCtrlSeq(*(void**)(w+0xdcc),ov70_02246502[ov70_0223C2EC(w)].value);
}
