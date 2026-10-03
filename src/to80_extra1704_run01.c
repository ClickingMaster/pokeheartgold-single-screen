#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
#define S(p,o) (*(s8 *)((u8 *)(p)+(o)))
typedef struct{u8 p[0x18];void*sprite;u8 q[0xac-0x1c];s8 selected,previous;u8 pad[6];u32 chosen,changed;}Work;
extern const u8 ov99_021EA3C4[];extern int TouchscreenHitbox_FindHitboxAtTouchNew(const void*);extern void ManagedSprite_SetAnim(void*,int);extern void ManagedSprite_SetAnimateFlag(void*,int);extern void ManagedSprite_SetPositionXY(void*,s16,s16);extern int ManagedSprite_GetActiveAnim(void*);extern void PlaySE(int);extern void ov99_021E8FEC(Work*,int);int ov99_021E90F4(Work*);
int ov99_021E90F4(Work*w){int old=w->selected;if(gSystem.touchNew){int hit=TouchscreenHitbox_FindHitboxAtTouchNew(ov99_021EA3C4);if(hit!=-1){w->selected=hit-1;ManagedSprite_SetAnim(w->sprite,11);PlaySE(1500);if(w->selected==-1){w->chosen=1;ManagedSprite_SetAnim(w->sprite,13);PlaySE(1500);}ManagedSprite_SetAnimateFlag(w->sprite,1);}}else{if(gSystem.newKeys&1){if(old==10)return 1;if(old==-1){w->chosen=1;PlaySE(1500);}}if(w->selected==-1){if(gSystem.newAndRepeatedKeysRaw&0x80)w->selected=w->previous;}else if(w->selected==10){if(gSystem.newAndRepeatedKeysRaw&0x40)w->selected=w->previous+5;}else{if(gSystem.newAndRepeatedKeysRaw&0x10)w->selected++;else if(gSystem.newAndRepeatedKeysRaw&0x20)w->selected--;else if(gSystem.newAndRepeatedKeysRaw&0x80)w->selected+=5;else if(gSystem.newAndRepeatedKeysRaw&0x40){int d=w->selected==10?1:5;w->selected-=d;}{int s=w->selected;if(s>10)s=10;else if(s<-1)s=-1;w->selected=s;}if(w->selected!=-1&&w->selected!=10)w->previous=w->selected%5;}}if(old!=w->selected){PlaySE(1500);if(w->selected==-1){ManagedSprite_SetPositionXY(w->sprite,224,16);ManagedSprite_SetAnim(w->sprite,12);}else if(w->selected==10){ManagedSprite_SetPositionXY(w->sprite,224,176);ManagedSprite_SetAnim(w->sprite,12);}else{int row=w->selected>=5?1:0;s8 col=w->selected;if(row)col-=5;ManagedSprite_SetPositionXY(w->sprite,(s16)(col*48+32),(s16)(row*64+72));if(ManagedSprite_GetActiveAnim(w->sprite)!=11)ManagedSprite_SetAnim(w->sprite,10);if(old!=10||w->selected!=9){ov99_021E8FEC(w,0);w->changed=1;}}}return 0;}
