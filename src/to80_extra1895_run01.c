#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct{u32 x[4];}Display;
typedef struct{u32 x[7];}Bg;
typedef struct{Bg bg[6];}Bank;
extern Display ov96_0221C2B4;
extern Bank ov96_0221C338;
extern u8 ov96_0221C290[];
extern void SetBothScreensModesAndDisable(void*),InitBgFromTemplate(void*,u8,void*,u8),BgClearTilemapBufferAndCommit(void*,u8),BG_ClearCharDataRange(u8,u32,u32,int),BG_FillCharDataRange(void*,u8,int,int,int);
extern u8 ov96_0221C298[];
void ov96_021F7D30(void*,int);
void ov96_021F7D30(void*w,int heap){Display modes=ov96_0221C2B4;Bank templates;volatile int i;u8*type,*id;Bg*t;int next;SetBothScreensModesAndDisable(&modes);templates=ov96_0221C338;type=ov96_0221C298;id=ov96_0221C290;i=0;t=templates.bg;do{InitBgFromTemplate(w,*id,t,*type);BgClearTilemapBufferAndCommit(w,*id);BG_ClearCharDataRange(*id,64,0,heap);next=i+1;type++;t++;id++;i=next;}while(next<6);}
