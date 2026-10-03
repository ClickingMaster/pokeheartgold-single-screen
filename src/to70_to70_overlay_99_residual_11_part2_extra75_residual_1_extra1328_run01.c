#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State{void *first;u8 pad4[12];void *ui;u8 pad14[0x98];s8 event;u8 padad[3];u32 mode;u8 padb4[8];u32 values[8];} State;
extern void *ov98_0221EEFC(void *);
extern void BufferPokeathlonEventName(void *,int,u8);
extern void ov98_0221EC08(void *,int,int,int,int);
extern void ov98_0221EBEC(void *,int,int,int,u8,int);
extern void BufferIntegerAsString(void *,int,u32,int,int,int);
extern u32 ov99_021E94FC(void *,u8,u32);
extern int ov99_021E8544(State *,u8);
extern u32 ov99_021E94CC(void *,int,u8,u32);
extern void ov99_021E88EC(State *,u32,int,int,int);
extern void ov98_0221EBD8(void *,int,int,int);
extern void ov99_021E9020(State *);
extern int ov99_021E9518(void *,u8,u8);
extern void ov98_0221EE9C(void *,int,int,int,int);
extern void ov98_0221EE84(void *,int);
extern int ov99_021E82E4(void *,u8);
extern void ov98_0221ECD0(void *,int,int,int,int,int);
extern int ov99_021E945C(void *),ov99_021E9508(void *);
extern void ov98_0221EDC4(void *,int,int,int,int);
extern int ov99_021E84EC(State *,int);
void ov99_021E89EC(State *);
void ov99_021E89EC(State *p){
 int i,selected;void *fmt;
 BufferPokeathlonEventName(ov98_0221EEFC(p->ui),0,(u8)p->event);
 ov98_0221EC08(p->ui,2,0x3e,1,1);
 ov98_0221EBEC(p->ui,5,p->event+0x41,0,1,0);
 fmt=ov98_0221EEFC(p->ui);
 BufferIntegerAsString(fmt,0,ov99_021E94FC(p->first,(u8)p->event,p->mode),7,1,1);
 ov98_0221EC08(p->ui,3,0x36,1,0);
 for(i=0;i<5;i++){
  if(i==0 && p->mode==0)selected=1;else selected=0;
  if(!ov99_021E8544(p,(u8)i)){
   u32 value=ov99_021E94CC(p->first,p->event,(u8)i,p->mode);
   ov99_021E88EC(p,value,i+7,p->event+0x56,selected);
  }else ov98_0221EBEC(p->ui,i+7,p->event+0x60,0,(u8)selected,0);
 }
 if(p->mode){
  ov98_0221EBD8(p->ui,6,0x4b,0);ov99_021E9020(p);
  for(i=0;i<5;i++)if(!ov99_021E8544(p,(u8)i)){
   int value=ov99_021E9518(p->first,(u8)p->event,(u8)i);
   ov98_0221EE9C(p->ui,i+12,value,0,1);
  }
  return;
 }
 if(p->values[p->event]){ov98_0221EE84(p->ui,17);ov98_0221EE84(p->ui,18);}
 else{
  int value=ov99_021E82E4(p->first,(u8)p->event);
  ov98_0221ECD0(p->ui,17,0x6b,value,3,0);ov98_0221EBD8(p->ui,18,0x6a,0);
 }
 if(p->event<0)ov98_0221EE84(p->ui,6);
 if(!ov99_021E945C(p->first)){
  if(p->values[p->event])ov98_0221EBD8(p->ui,6,0x6d,0);
  else ov98_0221EDC4(p->ui,6,0x6c,0,ov99_021E9508(p->first));
 }else if(ov99_021E84EC(p,p->event))ov98_0221EDC4(p->ui,6,0x6c,0,ov99_021E9508(p->first));
 else ov98_0221EBD8(p->ui,6,p->event+0x4c,0);
}
