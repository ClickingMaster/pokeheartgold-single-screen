#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x10];u16 state;u8 pad2[0x2f0-0x12];void*gfx;u8 pad3[0x40b8-0x2f4];int x,y;u8 pad4[0x88c8-0x40c0];u16 flag;} Data;
typedef struct{u8 pad[0x21];u8 selected;u8 pad2[0x34-0x22];Data*data;} State;
extern u8 ov14_021F7C08[];
extern void BgCellManager_UpdateCellMove(void*,int),ov14_021F391C(Data*,int),ov14_021F29E4(Data*,int,int),ov14_021F2A18(Data*,int,int),ov14_021F3B3C(Data*),ov14_021F3B5C(Data*),ov14_021F395C(Data*,s16,s16),ov14_021E8620(void*),ov14_021F40E8(State*,int),ov14_021E7AD4(State*,int,int,int);
extern int ov14_021F2A04(Data*,int),System_GetTouchHeldCoords(int*,int*),ov14_021E6AA0(State*,int,int),ov14_021E7B8C(State*);
extern int ov14_021E7960(s16,s16),ov14_021E79AC(s16,s16,void*);
extern void PlaySE(int);
int ov14_021EA778(State*);
int ov14_021EA778(State*s){Data*d=s->data;int x,y;u16 selected,raw;BgCellManager_UpdateCellMove(d->gfx,14);BgCellManager_UpdateCellMove(s->data->gfx,15);switch(d->state){
case 0:ov14_021F391C(s->data,1);ov14_021F29E4(s->data,11,1);ov14_021F2A18(s->data,11,1);d->state=1;break;
case 1:if(ov14_021F2A04(s->data,11)==0){ov14_021F391C(s->data,0);ov14_021F3B3C(s->data);d->state=2;}break;
case 2:if(!System_GetTouchHeldCoords(&x,&y)){selected=ov14_021E7960((s16)s->data->x,(s16)s->data->y);if(selected==255)selected=ov14_021E79AC((s16)s->data->x,(s16)s->data->y,ov14_021F7C08);raw=selected;if(selected==255)selected=s->selected;else if(!ov14_021E6AA0(s,s->selected,selected))selected=s->selected;ov14_021E7AD4(s,raw,selected,2);d->state=3;}else{ov14_021F395C(s->data,(s16)x,(s16)y);ov14_021F3B5C(s->data);s->data->x=x;s->data->y=y;}break;
case 3:if(!ov14_021E7B8C(s)){PlaySE(1514);ov14_021E8620(s->data->gfx);d->state=4;}break;
case 4:ov14_021F391C(s->data,1);ov14_021F29E4(s->data,11,2);ov14_021F40E8(s,0);d->state=5;break;
case 5:if(!ov14_021F2A04(s->data,11)){ov14_021F2A18(s->data,11,0);ov14_021F391C(s->data,0);d->state=6;}break;
case 6:d->state=0;return 0;
}return 1;}
