#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct Object {char pad[0x10];void *handle;} Object;
typedef struct Work {int unk0;Object *object;} Work;
extern Work ov13_0224F5AC;
extern void ov13_02240068(void *,int,int *,int *);
extern void ov13_0223FF18(void *,int,int,int);
extern void ov13_022425C8(void *,void *);
extern void ov13_0223B178(void);
void ov13_0223B0D8(void *p);
#pragma push
#pragma opt_propagation off
// @fn ov13_0223B0D8
void ov13_0223B0D8(void *p){
 int value,other;
 ov13_02240068(ov13_0224F5AC.object->handle,0,&value,&other);
 value+=8;
 if(value<0xb0||value>0x100){
  ov13_0223FF18(ov13_0224F5AC.object->handle,-1,value,0x21);
  return;
 }
 {Work *w=&ov13_0224F5AC; Object *o=w->object; value=0xb0;
 ov13_0223FF18(o->handle,-1,value,0x21);}
 ov13_022425C8(p,ov13_0223B178);
}

#pragma pop
