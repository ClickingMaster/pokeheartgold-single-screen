#pragma force_active on
#include "global.h"
#include "system.h"
#include "touchscreen.h"
#pragma force_active on
typedef struct Slot{u8 unused0[13],state,unusedE,selected;u8 unused10[8];}Slot;
typedef struct Users{Slot slots[10];u8 unusedF0[0x3cf];u8 selected;}Users;
typedef struct Record{u8 unused0[12];u32 trainer;u8 unused10[12];}Record;
typedef struct Records{Record entries[30];int count;void*lookup;}Records;
typedef struct Profile{u32 trainer;}Profile;
typedef struct Peer{u8 unused0[0x50];Profile profile;}Peer;
typedef struct State{int state;Users*users;u32 unused8;void*resource;u8 unused10[0x1b4];int choice;u8 unused1c8[0xa8];Records*records;u8 unused274[16];u16 count,unused286,scroll;}State;
extern const TouchscreenHitbox ov34_0225E730[];
extern BOOL ov34_0225E5D4(State*),ov01_021F6B10(void*);extern void ov34_0225E5EC(State*,int),PlaySE(u16);extern int ov34_0225DC0C(void*,int);extern Peer*sub_02035754(int);extern void*sub_02035784(void);extern u32 PlayerProfile_GetTrainerID(void*);
int ov34_0225DE94(State*);
int ov34_0225DE94(State*s){int hit=TouchscreenHitbox_FindRectAtTouchHeld(ov34_0225E730);BOOL feedback=ov34_0225E5D4(s);
 if(hit!=-1){switch(hit){case 0:ov34_0225E5EC(s,hit);if(feedback==1){if(s->scroll){PlaySE(1509);s->scroll--;}s->choice=hit-2;}break;
 case 1:ov34_0225E5EC(s,hit);if(feedback==1){if((int)s->scroll<(int)s->count-3){PlaySE(1509);s->scroll++;}s->choice=hit-2;}break;
 case 5:s->choice=hit-2;break;
 case 6:if(gSystem.touchNew&&ov01_021F6B10(s->resource)==1){PlaySE(1532);s->state=3;}break;
 default:if(gSystem.touchNew){if(s->records->count>=hit-1){int index=ov34_0225DC0C(s->records->lookup,s->scroll+hit-2);int i=0,offset=0;for(;i<10;i++,offset+=24){Peer*peer=sub_02035754(i);if(peer){register Profile*profile=&peer->profile;if(((Slot*)((u8*)s->users+offset))->state==2&&profile->trainer==s->records->entries[index].trainer){PlaySE(1509);s->users->slots[i].selected=1;break;}}}if(PlayerProfile_GetTrainerID(sub_02035784())==s->records->entries[index].trainer){PlaySE(1509);s->users->selected=1;}}s->choice=hit-2;}break;}
 }return hit;
}
