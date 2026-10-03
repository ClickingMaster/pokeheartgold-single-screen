#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 x[4];}Display;
typedef struct{u32 x[7];}Bg;
typedef struct{Bg bg[5];}Bank;
extern Display ov112_021FF2F4;
extern Bank ov112_021FF434;
extern u8 ov112_021FF4C0[];
extern void SetBothScreensModesAndDisable(void*),InitBgFromTemplate(void*,u8,void*,u8),BgClearTilemapBufferAndCommit(void*,u8),BG_ClearCharDataRange(u8,u32,u32,int),BG_FillCharDataRange(void*,u8,int,int,int);
extern void*BgConfig_Alloc(int);
typedef struct{int a,heap;u8 p[12];void*bg;}Work;
void ov112_021F179C(Work*);
void ov112_021F179C(Work*w){Display modes;Bank templates;int i;u8*id;Bg*t;w->bg=BgConfig_Alloc(w->heap);modes=ov112_021FF2F4;SetBothScreensModesAndDisable(&modes);templates=ov112_021FF434;id=ov112_021FF4C0;i=0;t=templates.bg;for(;i<5;t++,id++,i++){InitBgFromTemplate(w->bg,*id,t,0);BgClearTilemapBufferAndCommit(w->bg,*id);BG_FillCharDataRange(w->bg,*id,0,1,0);}}
