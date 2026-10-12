#pragma force_active on
#include "global.h"
typedef struct{u16 state;u8 pad[0xa];void*field;u8 pad2[0x14];void*mon;}Work;
extern int FieldSystem_FirstAliveMonHasType(void*,int);extern void FieldSystem_UnkSub108_AddMonMood(void*,int),ov02_022507B4(void*,int);extern u32 GetMonData(void*,int,void*);extern void PlayCry(u16,u8);
int ov01_021F26CC(Work*);int ov01_021F2700(Work*);
int ov01_021F26CC(Work*w){int mode;if(FieldSystem_FirstAliveMonHasType(w->field,0)){FieldSystem_UnkSub108_AddMonMood(*(void**)((u8*)w->field+0x108),1);mode=2;}else mode=1;ov02_022507B4(w->field,mode);w->state++;return 0;}
int ov01_021F2700(Work*w){int species=GetMonData(w->mon,5,NULL);int form=GetMonData(w->mon,112,NULL);PlayCry((u16)species,(u8)form);w->state++;return 0;}
