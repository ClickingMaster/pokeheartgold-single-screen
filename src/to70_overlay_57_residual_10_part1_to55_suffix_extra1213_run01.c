#pragma force_active on
#include "global.h"
#pragma force_active on
extern void PlaySE(u16);
extern void ov57_02238AC0(void *);
extern void ov57_022388E4(void *,int);
extern void ov57_02238958(void *);
extern void ov57_02238A00(void *);
extern void ov57_022399F8(void *);
extern void ov57_0223B858(void *,int);
extern void ov57_0223B90C(void *,int);
extern void ov57_0223B948(void *,int);
extern int ov57_02238028(void *);
extern void ov57_02239728(void *,int,int,int);
extern u32 ov57_02237FAC(void *,u8);
extern int sub_0209106C(u32);
extern void ov57_02239B2C(void *,int);
extern int SealCaseInventory_GetSealQuantity(void *,int);
extern int GiveOrTakeSeal(void *,int,int);
extern void ov57_02239A8C(void *,u32);
extern void ov57_022382F8(void *,int,int);
typedef struct Item {u8 seal;u8 pad[15];} Item;
typedef struct Args {u8 pad[32];void *sealcase;} Args;
typedef struct Work {Args *args;u8 p0[0x60];void *inventory;u8 p1[0x6c];u32 selected;int enabled;u8 p2[0x10];u8 message[0x174];int v260,v264;u8 p3[0xe8];Item items[8];u8 p4[0x2c];int mode;u8 p5[0x34];void *button8,*button9,*button10,*button11,*button12;int page,count;u8 available[8];} Work;
void ov57_0223AEA8(int,int,Work *);
void ov57_0223AEA8(int action,int down,Work *w) {
 if(!w->enabled)return;
 switch(action){
 case 8:
 if(down==0){if(w->page>0)w->page--;else w->page=w->count-1;ov57_02238AC0(w);ov57_022388E4(w,w->page);ov57_02238958(w);ov57_02238A00(w);ov57_022399F8(w);PlaySE(0x6c4);}ov57_0223B858(w->button8,down);break;
 case 9:
 if(down==0){w->page++;w->page%=w->count;ov57_02238AC0(w);ov57_022388E4(w,w->page);ov57_02238958(w);ov57_02238A00(w);ov57_022399F8(w);PlaySE(0x6c4);}ov57_0223B858(w->button9,down);break;
 case 10:
 if(down==0){if(w->mode!=5){w->mode=5;PlaySE(0x5dd);}ov57_0223B90C(w->button10,0);}ov57_0223B858(w->button10,down);break;
 case 11:
 if(down==0){if(w->mode!=6){w->mode=6;ov57_0223B948(w,0);PlaySE(0x5e2);}ov57_0223B90C(w->button11,w->v260);}ov57_0223B858(w->button11,down);break;
 case 12:
 if(down==0){if(w->mode!=7){w->mode=7;ov57_0223B948(w,0);PlaySE(0x5dd);}ov57_0223B90C(w->button12,w->v264);}ov57_0223B858(w->button12,down);break;
 case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:
 if(down==0){
 if(!ov57_02238028(w)){PlaySE(0x5f2);ov57_02239728(w->message,0,15,0);return;}
 if(w->available[action]!=0&&SealCaseInventory_GetSealQuantity(w->inventory,w->available[action]-1)!=0){
 w->selected=ov57_02237FAC(w,(u8)action);ov57_02239B2C(w->message,sub_0209106C(w->available[action]));GiveOrTakeSeal(w->args->sealcase,w->available[action],-1);ov57_02239A8C(w,action);PlaySE(0x5eb);
 }else if(w->available[action]!=0){PlaySE(0x5f2);ov57_02239728(w->message,0,16,0);}}
 break;
 case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:
 if(down==0){int index=action-13;ov57_022382F8(w,down,(u8)index);ov57_02239B2C(w->message,sub_0209106C(w->items[index].seal));PlaySE(0x5eb);}break;
 default:GF_AssertFail();break;
 }
}
