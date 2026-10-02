#pragma force_active on
#include "global.h"
typedef struct {u32 v[7];} Template;
extern Template ov28_0225EAC0,ov28_0225EADC,ov28_0225EAF8;
extern void InitBgFromTemplate(void*,int,Template*,int),BG_ClearCharDataRange(int,int,int,int),BgClearTilemapBufferAndCommit(void*,int);
void ov28_0225D650(void*bg);
void ov28_0225D650(void*bg){Template a,b,c;GXS_SetGraphicsMode(GX_BGMODE_0);a=ov28_0225EAC0;InitBgFromTemplate(bg,4,&a,0);BG_ClearCharDataRange(4,32,0,8);BgClearTilemapBufferAndCommit(bg,4);b=ov28_0225EADC;InitBgFromTemplate(bg,5,&b,0);c=ov28_0225EAF8;InitBgFromTemplate(bg,6,&c,0);}
