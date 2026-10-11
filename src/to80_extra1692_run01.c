#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct UI {u8 p0[0x2c];void *grid;u8 p30[0x2c0];void *back;u8 p2f4[8];void *icons[9];void *cursor;u8 p324[0x127];u8 flag,choice;u8 p44d[0x3c47];u8 map[0x4838];int special;} UI;
typedef struct W {void *p0,*storage,*party;u8 pc[0x13];u8 box;u8 p20;u8 selected,state;u8 p23[2];u8 page;u8 p26[14];UI *ui;} W;
extern void ManagedSprite_GetPositionXY(void *,s16 *,s16 *);
extern void ManagedSprite_SetPositionXY(void *,s16,s16);
extern int Party_GetCount(void *);
extern void *Party_GetMonByIndex(void *,int);
extern u32 GetMonData(void *,int,void *);
extern int ItemIdIsMail(u16);
extern int PCBox_PartyHasOtherUsableMon(W *,int);
extern int PCStorage_CountEmptySpotsInBox(void *);
extern void PCBox_ClearMonInfo(W *);
extern void PCBox_SlideOutSummaryButton(void *);
extern void ov14_021E884C(UI *);
extern void PCBox_UpdateTrayCursor(W *);
extern void PCBox_DrawTrayBoxNameAndCount(W *);
extern int GridInputHandler_GetNextInput(void *);
extern int PCBox_ShowSlotMonInfo(W *,int);
extern void PCBox_AllocMoveWork(W *);
extern void ov14_021EA378(void *);
extern void PCBox_WaitForUpdateFunc(W *,void (*)(void *),int);
void ov14_021F15C8(W *,u32);
void ov14_021F15C8(W *w,u32 index) {
 struct {s16 y,x;} xy;
 w->ui->special=index==255;
 ManagedSprite_GetPositionXY(w->ui->cursor,&xy.x,&xy.y);
 ManagedSprite_SetPositionXY(w->ui->icons[w->ui->map[w->selected]],xy.x,xy.y+4);
 if(index<36) {
 if(index>=30) {
 int count=Party_GetCount(w->party);
 if(w->selected<30) {
 if(index-30>count){PCBox_ClearMonInfo(w);PCBox_SlideOutSummaryButton(w->ui->back);}else ov14_021E884C(w->ui);
 } else {
 if(index-30>=count){PCBox_ClearMonInfo(w);PCBox_SlideOutSummaryButton(w->ui->back);}else ov14_021E884C(w->ui);
 }
 } else ov14_021E884C(w->ui);
 } else if(index!=255) {
 u32 selected;
 w->page=(w->page/6)*6+index-37;
 PCBox_UpdateTrayCursor(w);PCBox_DrawTrayBoxNameAndCount(w);
 selected=w->selected;
 if(selected>=30) {
 void *mon;
 selected-=30;mon=Party_GetMonByIndex(w->party,selected);
 if(ItemIdIsMail((u16)GetMonData(mon,6,0))==1)ov14_021E884C(w->ui);
 else if(GetMonData(mon,162,0)!=0)ov14_021E884C(w->ui);
 else if(!PCBox_PartyHasOtherUsableMon(w,selected))ov14_021E884C(w->ui);
 else if(w->page==w->box){PCBox_ClearMonInfo(w);PCBox_SlideOutSummaryButton(w->ui->back);}
 else if(!PCStorage_CountEmptySpotsInBox(w->storage))ov14_021E884C(w->ui);
 else {PCBox_ClearMonInfo(w);PCBox_SlideOutSummaryButton(w->ui->back);}
 } else if(w->page==w->box){PCBox_ClearMonInfo(w);PCBox_SlideOutSummaryButton(w->ui->back);}
 else if(!PCStorage_CountEmptySpotsInBox(w->storage))ov14_021E884C(w->ui);
 else {PCBox_ClearMonInfo(w);PCBox_SlideOutSummaryButton(w->ui->back);}
 } else {
 u32 next=GridInputHandler_GetNextInput(w->ui->grid);
 if(next<36) {if(!PCBox_ShowSlotMonInfo(w,next))PCBox_SlideOutSummaryButton(w->ui->back);}
 else {PCBox_ClearMonInfo(w);PCBox_SlideOutSummaryButton(w->ui->back);}
 }
 w->ui->choice=(u8)index;w->ui->flag=0;PCBox_AllocMoveWork(w);w->state=2;PCBox_WaitForUpdateFunc(w,ov14_021EA378,43);
}
