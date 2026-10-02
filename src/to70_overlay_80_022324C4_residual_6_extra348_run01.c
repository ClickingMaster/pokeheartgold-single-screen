#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[0xa8];void*first;void*second;}W;typedef struct P{u8 pad[16];u8 mode;}P;typedef struct Map{void*bg;}Map;extern Map*FrontierSystem_GetFrontierMap(W*);extern int ov80_02237D8C(u8);extern void*AllocWindows(int,int);extern void AddWindowParameterized(void*,void*,int,u8,u8,u8,u8,u8,u16),ov80_022333D0(void*,void*),ov80_02233490(W*,P*);void ov80_022332D0(W*,P*);
void ov80_022332D0(W*w,P*p){Map*map=FrontierSystem_GetFrontierMap(w);if(w->first)GF_AssertFail();if(w->second)GF_AssertFail();if(ov80_02237D8C(p->mode)==0){w->first=AllocWindows(11,1);AddWindowParameterized(map->bg,w->first,1,1,1,10,4,14,595);ov80_022333D0(map->bg,w->first);}else{w->first=AllocWindows(11,1);w->second=AllocWindows(11,1);AddWindowParameterized(map->bg,w->first,1,1,1,10,4,14,595);AddWindowParameterized(map->bg,w->second,1,21,1,10,4,14,555);ov80_022333D0(map->bg,w->first);ov80_022333D0(map->bg,w->second);}ov80_02233490(w,p);}
