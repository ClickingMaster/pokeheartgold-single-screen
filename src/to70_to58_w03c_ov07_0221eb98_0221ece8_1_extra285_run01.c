#pragma force_active on
#include "global.h"
#pragma force_active on
int ov07_0221FB04(void *,int),ov07_0221BFC0(void *);void SetBgPriority(u8,u8),SetBgControlParam(void *,u8,int,int),BgClearTilemapBufferAndCommit(void *,u8);
void ov07_0221EC7C(u8 *,int);
void ov07_0221EC7C(u8 *w,int bg){SetBgPriority((u8)bg,(u8)ov07_0221FB04(w,1));if(ov07_0221BFC0(w)==1){SetBgControlParam(*(void **)(w+196),(u8)bg,2,3);}else{SetBgControlParam(*(void **)(w+196),(u8)bg,0,0);SetBgControlParam(*(void **)(w+196),(u8)bg,2,3);}BgClearTilemapBufferAndCommit(*(void **)(w+196),(u8)bg);}
