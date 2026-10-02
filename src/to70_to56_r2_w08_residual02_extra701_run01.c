#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x4c];u16 id;u8 unused:2;u8 isCard:1;u8 bit3:1;u8 special:1;u8 rest:3;u8 pad2;}Header;
typedef struct{u16 type,pad;u32 id;u8 pad2[0x340];u8 a;u8 pad3[7];u8 b;u8 pad4[3];u32 day;}Card;
typedef struct{u8 pad[0x8c];Header header;Card card;u8 pad2[0x25b4];void(*callback)(void);int state;}Work;
extern Work*OverlayManager_GetData(void*);extern void*OverlayManager_GetArgs(void*),*Save_MysteryGift_Get(void*);extern void SaveMysteryGift_ReceivedFlagSet(void*,u16),SaveMysteryGift_TrySetSpecialCard(void*,Card*),SaveMysteryGift_TryInsertGift(void*,Card*,int),SaveMysteryGift_TryInsertCard(void*,Card*),ov74_022360A0(void*),ov74_0222A130(void);
extern void GF_RTC_CopyDate(RTCDate*);
void ov74_0222A174(void*);
void ov74_0222A174(void*man){Work*w=OverlayManager_GetData(man);Header*h=&w->header;Card*c=&w->card;void*save=*(void**)((u8*)OverlayManager_GetArgs(man)+8);int isCard;void*gift;RTCDate date;if(h->isCard==0)isCard=0;else{c->b=0;isCard=1;if(h->special==0)c->a=0;GF_RTC_CopyDate(&date);c->day=RTC_ConvertDateToDay(&date);}gift=Save_MysteryGift_Get(save);SaveMysteryGift_ReceivedFlagSet(gift,h->id);if(h->isCard==1&&c->type==3&&c->id==533)SaveMysteryGift_TrySetSpecialCard(gift,c);else if(isCard==0)SaveMysteryGift_TryInsertGift(gift,c,3);else SaveMysteryGift_TryInsertCard(gift,c);ov74_022360A0(save);w->state=0;w->callback=ov74_0222A130;}
