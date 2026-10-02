#pragma force_active on
#include "global.h"
extern void *ov12_0223A934(void *);
extern void *Heap_Alloc(int,int);
extern u8 BattleSystem_GetTextFrameDelay(void *);
extern u32 BattleSystem_PrintBattleMessage(void *,void *,void *,u8);
extern void ov12_022605D0(void *,void *);
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);
typedef struct {void *system;u8 id,value,printer,pad;} Work;
typedef struct {u8 command,tag;u16 id;int params[6];int digits,battler;} Message;
typedef struct {u8 a,b;u16 id;} Packet;
void ov12_0225A85C(void *system,void *ctx,Packet *p);
void ov12_0225A85C(void *system,void *ctx,Packet *p){void *messages=ov12_0223A934(system);Work *w=Heap_Alloc(5,8);Message msg;u32 a,b;w->system=system;w->id=p->a;w->value=((u8*)ctx)[0x194];msg.id=p->id*3;msg.tag=2;a=p->b;b=((u8*)ctx)[0x194];msg.params[0]=b|(a<<8);w->printer=BattleSystem_PrintBattleMessage(system,messages,&msg,BattleSystem_GetTextFrameDelay(system));SysTask_CreateOnMainQueue(ov12_022605D0,w,0);}
