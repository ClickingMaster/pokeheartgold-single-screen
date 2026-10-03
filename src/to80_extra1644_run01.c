#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Node{char pad[12];u32 value;}Node;typedef struct Result{Node*node;u8 flag;char pad[3];}Result;extern u8 _02254F58[];extern int ov45_02238450(void*,u32*,Node**,u8*,u8*);extern Node*ov45_02236CE0(void*,int,u8,u8,u32*);void ov45_02236BA8(Result*,void*,u32*);
void ov45_02236BA8(Result*r,void*p,u32*v){u8 second,first;Node*node;int result=ov45_02238450(p,v,&node,&second,&first);if(!node||node->value<*v){r->node=ov45_02236CE0(p,result,second,first,v);r->flag=_02254F58[2];}else{r->node=node;r->flag=_02254F58[0];}}
