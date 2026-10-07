#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int value;u8 gap4[4];int state;u8 gapc[0x20];u8 task[0x16];u16 time;u8 tail[8];} Entry;
typedef union{struct{u8 pad[0x5bc];Entry slots[3];} entries;struct{u8 pad[0x69c];u16 selected;} header;} Work;
extern Work *_022577C0;
extern int ov45_0223021C(Work *,int);
extern void ov00_021E5CEC(void *,int,int,int),ov00_021E5CD0(void (*)(void *),void *),ov00_021E5CA0(void (*)(void *),void *),ov45_02230390(void *),ov45_02230394(void *);
int ov45_0222F350(int);
int ov45_0222F350(int index){Work *w;Entry *e;if(!_022577C0)GF_AssertFail();if(!ov45_0223021C(_022577C0,index))return 0;if(_022577C0->header.selected!=3)return 0;e=&_022577C0->entries.slots[index];if(e->state==1)return 0;_022577C0->header.selected=index;w=_022577C0;ov00_021E5CEC(w->entries.slots[index].task,w->entries.slots[index].value,0,w->entries.slots[index].time/30);ov00_021E5CD0(ov45_02230390,_022577C0);ov00_021E5CA0(ov45_02230394,_022577C0);return 1;}
