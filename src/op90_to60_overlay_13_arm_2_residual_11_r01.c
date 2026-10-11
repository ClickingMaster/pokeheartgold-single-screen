#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct Object {char pad[0x10];void *handle;} Object;
typedef struct Work {int unk0;Object *object;} Work;
extern Work ov13_0224F5AC;
extern void WifiSetupOamBlock_GetPosition(void *,int,int *,int *);
extern void WifiSetupOamBlock_SetPosition(void *,int,int,int);
extern void WifiSetupTask_SetFunc(void *,void *);
extern void ov13_0223B178(void);
void ov13_0223B0D8(void *p);
#pragma push
#pragma opt_propagation off
// @fn ov13_0223B0D8
void ov13_0223B0D8(void *p){
 int value,other;
 WifiSetupOamBlock_GetPosition(ov13_0224F5AC.object->handle,0,&value,&other);
 value+=8;
 if(value<0xb0||value>0x100){
  WifiSetupOamBlock_SetPosition(ov13_0224F5AC.object->handle,-1,value,0x21);
  return;
 }
 {Work *w=&ov13_0224F5AC; Object *o=w->object; value=0xb0;
 WifiSetupOamBlock_SetPosition(o->handle,-1,value,0x21);}
 WifiSetupTask_SetFunc(p,ov13_0223B178);
}

#pragma pop
