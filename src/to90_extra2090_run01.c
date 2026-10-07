#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 pad;void*mon;u8 pad8[12];u16 hp;u8 pad16[5];u8 low:7;u8 top:1;u8 pad1c[52];} Slot;
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
void ov08_0222057C(void*),ov08_022201C0(void*),ov08_0221DBCC(void*,u16,u8,int),ReadMsgDataIntoString(void*,int,void*);
int GetItemAttr(u16,int,int),BattleSystem_RecoverStatus(void*,void*,u8,int,u16),ov08_0221D5DC(void*,u8),GetMonData(void*,int,void*);
void *BattleSystem_GetPartyMon(void*,void*,u8);
int ov08_0221C1C8(void*);
int ov08_0221C1C8(void*w){void*c=P(w,0);
 if((B(c,0x11)==0 && W(c,0x18)) || (B(c,0x11)==1 && W(c,0x1c))){ov08_0222057C(w);ov08_022201C0(w);B(P(w,0),0x11)=6;B(w,0x2079)=25;return 17;}
 if(GetItemAttr(H(c,0x22),36,W(c,12)) && !GetItemAttr(H(c,0x22),37,W(c,12)) && !((Slot*)w)[B(c,0x11)].top){B(w,0x2079)=13;return 22;}
 if(BattleSystem_RecoverStatus(P(c,8),P(c,0x28),*(((u8*)c+B(c,0x11))+0x2c),0,H(c,0x22))==1){
 if(GetItemAttr(H(c,0x22),37,W(c,12)))B(w,0x2079)=13;
 else if(ov08_0221D5DC(w,B(c,0x11))==1 && !GetItemAttr(H(c,0x22),23,W(c,12))){ov08_0221DBCC(P(c,8),H(c,0x22),B(c,0x33),W(c,12));((Slot*)w)[B(c,0x11)].mon=BattleSystem_GetPartyMon(P(c,8),P(c,0x28),*(((u8*)c+B(c,0x11))+0x2c));H(c,0x20)=GetMonData(((Slot*)w)[B(c,0x11)].mon,163,0);H(c,0x20)-=((Slot*)w)[B(c,0x11)].hp;B(w,0x2079)=25;}
 else B(w,0x2079)=23;
 B(w,0x207c)=0;return 22;
 }
 ReadMsgDataIntoString(P(w,0x1fa8),81,P(w,0x1fb0));ov08_022201C0(w);B(P(w,0),0x11)=6;B(w,0x2079)=25;return 17;
}
