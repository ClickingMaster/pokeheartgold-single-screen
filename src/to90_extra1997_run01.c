#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x10];u16 state;u8 pad2[0x2f0-0x12];void*gfx;u8 pad3[0x40b8-0x2f4];int x,y;u8 pad4[0x88c8-0x40c0];u16 flag;} Data;
typedef struct{u8 pad[0x21];u8 selected;u8 pad2[0x34-0x22];Data*data;} State;
extern u8 ov14_021F7C08[];
extern void BgCellManager_UpdateCellMove(void*,int),ov14_021F391C(Data*,int),PCBox_SetSpriteAnim(Data*,int,int),PCBox_SetSpriteVisible(Data*,int,int),PCBox_ShowHeldItemIconOutline(Data*),PCBox_UpdateHeldItemIconOutline(Data*),PCBox_SetHeldItemIconPosition(Data*,s16,s16),PCBox_SlideInSummaryButton(void*),PCBox_SetSelectedIconOutlineVisible(State*,int),ov14_021E7AD4(State*,int,int,int);
extern int PCBox_IsSpriteAnimated(Data*,int),System_GetTouchHeldCoords(int*,int*),PCBox_CanSwapHeldItems(State*,int,int),ov14_021E7B8C(State*);
extern int PCBox_GetTouchedBoxSlot(s16,s16),PCBox_GetTouchedPartySlot(s16,s16,void*);
extern void PlaySE(int);
int ov14_021EA778(State*);
int ov14_021EA778(State*s){Data*d=s->data;int x,y;u16 selected,raw;BgCellManager_UpdateCellMove(d->gfx,14);BgCellManager_UpdateCellMove(s->data->gfx,15);switch(d->state){
case 0:ov14_021F391C(s->data,1);PCBox_SetSpriteAnim(s->data,11,1);PCBox_SetSpriteVisible(s->data,11,1);d->state=1;break;
case 1:if(PCBox_IsSpriteAnimated(s->data,11)==0){ov14_021F391C(s->data,0);PCBox_ShowHeldItemIconOutline(s->data);d->state=2;}break;
case 2:if(!System_GetTouchHeldCoords(&x,&y)){selected=PCBox_GetTouchedBoxSlot((s16)s->data->x,(s16)s->data->y);if(selected==255)selected=PCBox_GetTouchedPartySlot((s16)s->data->x,(s16)s->data->y,ov14_021F7C08);raw=selected;if(selected==255)selected=s->selected;else if(!PCBox_CanSwapHeldItems(s,s->selected,selected))selected=s->selected;ov14_021E7AD4(s,raw,selected,2);d->state=3;}else{PCBox_SetHeldItemIconPosition(s->data,(s16)x,(s16)y);PCBox_UpdateHeldItemIconOutline(s->data);s->data->x=x;s->data->y=y;}break;
case 3:if(!ov14_021E7B8C(s)){PlaySE(1514);PCBox_SlideInSummaryButton(s->data->gfx);d->state=4;}break;
case 4:ov14_021F391C(s->data,1);PCBox_SetSpriteAnim(s->data,11,2);PCBox_SetSelectedIconOutlineVisible(s,0);d->state=5;break;
case 5:if(!PCBox_IsSpriteAnimated(s->data,11)){PCBox_SetSpriteVisible(s->data,11,0);ov14_021F391C(s->data,0);d->state=6;}break;
case 6:d->state=0;return 0;
}return 1;}
