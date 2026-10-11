#pragma force_active on
#include "global.h"
typedef struct {u8 pad[8];void *party;u8 pad2[0x13];u8 byte1f,pad20,choice,pad22[3],byte25;u8 pad3[0xa];u32 state;} W;
extern int PCBox_PartyHasOtherUsableMon(W *,int),ItemIdIsMail(u16);
extern void PlaySE(int),PCBox_PrintReleaseMessage(W *,int,int),PCBox_PrintMonActionMessage(W *,int,int,int),PCBox_SetIconsTranslucent(W *,int,int),PCBox_HideSelectedIconOutline(W *);
extern void *Party_GetMonByIndex(void *,int);
extern u32 GetMonData(void *,int,void *);
extern int PCBox_WaitForUpdateFunc(W *,void *,int);
extern u8 ov14_021E96C8[];
int PCBox_State_TryDeposit(W *);
int PCBox_State_TryDeposit(W *w){int slot=w->choice-30;void *mon;if(PCBox_PartyHasOtherUsableMon(w,slot)==0){PlaySE(1523);PCBox_PrintReleaseMessage(w,6,37);w->state=93;return 6;}mon=Party_GetMonByIndex(w->party,slot);if(ItemIdIsMail((u16)GetMonData(mon,6,0))==1){PlaySE(1523);PCBox_PrintMonActionMessage(w,0,6,37);w->state=93;return 6;}if(GetMonData(mon,162,0)!=0){PlaySE(1523);PCBox_PrintMonActionMessage(w,0,5,37);w->state=93;return 6;}w->byte25=w->byte1f;PCBox_SetIconsTranslucent(w,2,1);PCBox_HideSelectedIconOutline(w);return PCBox_WaitForUpdateFunc(w,ov14_021E96C8,94);}
