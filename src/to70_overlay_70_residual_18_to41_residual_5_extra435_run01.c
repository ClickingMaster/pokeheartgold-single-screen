#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[7];} Template;
extern Template ov70_02245690,ov70_022456AC,ov70_02245674,ov70_02245658;
extern void InitBgFromTemplate(void *,int,Template *,int),BgClearTilemapBufferAndCommit(void *,int),BG_ClearCharDataRange(int,int,int,int),Gts_InitSubScreenBgs(void *,int,int);
void ov70_0223B7CC(void *);
void ov70_0223B7CC(void *bg){Template a=ov70_02245690;InitBgFromTemplate(bg,0,&a,0);BgClearTilemapBufferAndCommit(bg,0);{Template b=ov70_022456AC;InitBgFromTemplate(bg,1,&b,0);BgClearTilemapBufferAndCommit(bg,1);}{Template c=ov70_02245674;InitBgFromTemplate(bg,2,&c,0);BgClearTilemapBufferAndCommit(bg,2);}{Template d=ov70_02245658;InitBgFromTemplate(bg,3,&d,0);BgClearTilemapBufferAndCommit(bg,3);}Gts_InitSubScreenBgs(bg,0,0);BG_ClearCharDataRange(2,32,0,61);BG_ClearCharDataRange(0,32,0,61);BG_ClearCharDataRange(3,32,0,61);}
