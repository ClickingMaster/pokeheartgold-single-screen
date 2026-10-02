#pragma force_active on
#include "global.h"
typedef struct {u8 pad[8];void *party;u8 pad2[0x13];u8 byte1f,pad20,choice,pad22[3],byte25;u8 pad3[0xa];u32 state;} W;
extern int ov14_021E6480(W *,int),ItemIdIsMail(u16);
extern void PlaySE(int),ov14_021F67B0(W *,int,int),ov14_021F685C(W *,int,int,int),ov14_021F3488(W *,int,int),ov14_021F40DC(W *);
extern void *Party_GetMonByIndex(void *,int);
extern u32 GetMonData(void *,int,void *);
extern int ov14_021F0234(W *,void *,int);
extern u8 ov14_021E96C8[];
int ov14_021EE26C(W *);
int ov14_021EE26C(W *w){int slot=w->choice-30;void *mon;if(ov14_021E6480(w,slot)==0){PlaySE(1523);ov14_021F67B0(w,6,37);w->state=93;return 6;}mon=Party_GetMonByIndex(w->party,slot);if(ItemIdIsMail((u16)GetMonData(mon,6,0))==1){PlaySE(1523);ov14_021F685C(w,0,6,37);w->state=93;return 6;}if(GetMonData(mon,162,0)!=0){PlaySE(1523);ov14_021F685C(w,0,5,37);w->state=93;return 6;}w->byte25=w->byte1f;ov14_021F3488(w,2,1);ov14_021F40DC(w);return ov14_021F0234(w,ov14_021E96C8,94);}
