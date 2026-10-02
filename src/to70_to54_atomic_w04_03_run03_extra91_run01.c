#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work {void *bs,*owner;u8 first,id,state,flag;int backpic,value;int extra[2];} Work;
typedef struct Trainer {u8 a,kind;} Trainer;
extern void *Heap_Alloc(int,u32);
extern Trainer *BattleSystem_GetTrainer(void*,int);
extern int ov12_02261EF0(void*,int,int),TrainerClassToBackpicID(int,int),BattleSystem_GetBattleType(void*),ov12_0223C140(void*,int);
extern void ov12_0225D644(void*,void*),ov12_0225D138(void*,void*);
extern void *SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov12_0225A2A0(void*,u8*,u8*);
void ov12_0225A2A0(void*bs,u8*owner,u8*buf){Work*w=Heap_Alloc(5,28);w->state=0;w->bs=bs;w->first=buf[0];w->value=buf[1];w->id=owner[0x194];w->owner=owner;int backpic=0;if(owner[0x195]&1){w->flag=2;}else{Trainer*t=BattleSystem_GetTrainer(bs,owner[0x194]);w->flag=0;backpic=TrainerClassToBackpicID(ov12_02261EF0(bs,owner[0x194],t->kind),0);}w->backpic=backpic;BattleSystem_GetBattleType(bs);if(ov12_0223C140(bs,owner[0x194])!=255)SysTask_CreateOnMainQueue(ov12_0225D644,w,0);else SysTask_CreateOnMainQueue(ov12_0225D138,w,0);}
