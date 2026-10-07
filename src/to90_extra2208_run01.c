#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 kind:6;u8 unused:1;u8 ready:1;} Bits;
typedef struct {u8 pad[0xd2];volatile Bits flags;u8 gap[0x39];u8 menu[1];} Info;
typedef struct {u8 pad[12];u16 *result;Info *info;int value;} State;
extern int MenuInputStateMgr_GetState(void *);
extern void ov27_0225A8E8(State *,int),ov27_0225B398(State *,int);
int ov27_0225A714(State *);
int ov27_0225A714(State *s){
 Info *info;u16 *result;int kind;
 GF_ASSERT(s->result!=0);info=s->info;result=s->result;kind=info->flags.kind;
 if(kind==4){if(MenuInputStateMgr_GetState(info->menu)==0)ov27_0225A8E8(s,1);else info->flags.kind=0;}
 else if(kind==3){
  if(MenuInputStateMgr_GetState(info->menu)==1){*result=1;info->flags.kind=0;info->flags.ready=1;}
  else{ov27_0225B398(s,s->value);info->flags.kind=1;}
 }
 return 1;
}
