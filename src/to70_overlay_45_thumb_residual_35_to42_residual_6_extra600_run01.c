#pragma force_active on
#include "global.h"
extern u32 ov00_021E7144(void);
extern void ov45_02232BB0(void *);
extern u32 ov45_0223023C(void *,int);
typedef struct {u8 p[8];u32 state;u8 p2[0x38];volatile u16 active,count;u32 tail;} Entry;
typedef struct {u8 p[0x5b8];Entry entry[3];u16 selected;u8 enabled;} Work;
extern Work *_022577C0;
void ov45_022302E4(void);
void ov45_022302E4(void){int i;if(_022577C0->enabled==1){u32 state=ov00_021E7144();Entry *e;if(state>4)GF_AssertFail();e=&_022577C0->entry[_022577C0->selected];if(e->state!=state){e->state=state;ov45_02232BB0(e);}}for(i=0;i<3;i++){u8 *e=(u8*)_022577C0+i*0x4c;if(*(volatile u16*)(e+0x5fc)==1){u32 count;if(*(volatile u16*)(e+0x5fe))(*(volatile u16*)(e+0x5fe))--;count=ov45_0223023C(_022577C0,i);e=(u8*)_022577C0+i*0x4c;if(count<*(volatile u16*)(e+0x5fe))*(volatile u16*)(e+0x5fe)=count;}}}
