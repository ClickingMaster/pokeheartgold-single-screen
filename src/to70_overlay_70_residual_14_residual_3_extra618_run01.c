#pragma force_active on
#include "global.h"
typedef struct {u32 a[7];} Template;
extern Template ov70_0224539C,ov70_02245380,ov70_02245364,ov70_02245348;
extern void InitBgFromTemplate(void*,int,const Template*,int),BgClearTilemapBufferAndCommit(void*,int),BG_ClearCharDataRange(int,int,int,int),Gts_InitSubScreenBgs(void*,int,int);
void ov70_02239EAC(void*);
void ov70_02239EAC(void*p){Template a,b,c,d;a=ov70_0224539C;InitBgFromTemplate(p,0,&a,0);BgClearTilemapBufferAndCommit(p,0);b=ov70_02245380;InitBgFromTemplate(p,1,&b,0);c=ov70_02245364;InitBgFromTemplate(p,2,&c,0);d=ov70_02245348;InitBgFromTemplate(p,3,&d,0);BgClearTilemapBufferAndCommit(p,3);BG_ClearCharDataRange(0,32,0,61);BG_ClearCharDataRange(3,32,0,61);Gts_InitSubScreenBgs(p,0,0);}
