#pragma force_active on
#include "global.h"
#include "heap.h"
#pragma force_active on
struct S{void*c;void*object;int a,args[6],first,second;int rest[3];};struct T{u8 pad[32];void*ptr;};void ov07_02221EEC(void*,void*,struct T*);void ov07_02221540(void*,struct T*);
void*ParticleSystem_GetEmitterParam(void*);enum HeapID ov07_0221BFD0(void*);void*ov07_0221C478(void*);int ov07_0221F9A8(void*,int*,int);int ov07_0221C468(void*);int ov07_0221C470(void*);void ov07_022208B4(void*,struct S*);void ov07_022215B4(void*);
void ov07_02221540(void*a,struct T*t){ov07_02221EEC(t->ptr,a,t);}
