#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Modes {u32 words[4];} Modes;
typedef struct Template {u32 words[7];} Template;
extern const Modes ov109_021E78B8;
extern const Template ov109_021E7944,ov109_021E797C,ov109_021E7998,ov109_021E790C,ov109_021E7928,ov109_021E7960,ov109_021E79B4;
void ov109_021E692C(void *);
void *BgConfig_Alloc(int);
void SetBothScreensModesAndDisable(const Modes *);
void InitBgFromTemplate(void *,int,const Template *,int);
void BgClearTilemapBufferAndCommit(void *,int);
void BG_ClearCharDataRange(int,int,int,int);
void BgSetPosTextAndCommit(void *,int,int,int);
void ov109_021E694C(u8 *);
#define P(p,o) (*(void **)((p)+(o)))
#define W(p,o) (*(int *)((p)+(o)))
void ov109_021E694C(u8 *w){
 Modes modes;
 Template t4,t7,t6,t0,t1,t2,t3;
 ov109_021E692C(w);P(w,20)=BgConfig_Alloc(W(w,0));
 modes=ov109_021E78B8;SetBothScreensModesAndDisable(&modes);
 *(volatile u16 *)0x04000304&=~0x8000;
 t4=ov109_021E7944;InitBgFromTemplate(P(w,20),4,&t4,0);BgClearTilemapBufferAndCommit(P(w,20),4);
 t7=ov109_021E797C;InitBgFromTemplate(P(w,20),7,&t7,2);BgClearTilemapBufferAndCommit(P(w,20),7);
 t6=ov109_021E7998;InitBgFromTemplate(P(w,20),6,&t6,0);BgClearTilemapBufferAndCommit(P(w,20),6);
 t0=ov109_021E790C;InitBgFromTemplate(P(w,20),0,&t0,0);BgClearTilemapBufferAndCommit(P(w,20),0);
 t1=ov109_021E7928;InitBgFromTemplate(P(w,20),1,&t1,0);BgClearTilemapBufferAndCommit(P(w,20),1);
 t2=ov109_021E7960;InitBgFromTemplate(P(w,20),2,&t2,2);BgClearTilemapBufferAndCommit(P(w,20),2);
 t3=ov109_021E79B4;InitBgFromTemplate(P(w,20),3,&t3,2);BgClearTilemapBufferAndCommit(P(w,20),3);
 BG_ClearCharDataRange(4,32,0,W(w,0));BG_ClearCharDataRange(6,32,0,W(w,0));BG_ClearCharDataRange(0,32,0,W(w,0));BG_ClearCharDataRange(3,64,0,W(w,0));
 BgSetPosTextAndCommit(P(w,20),7,0,-4);
}
