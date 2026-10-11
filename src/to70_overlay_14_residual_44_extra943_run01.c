#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x34];void*ctx;}Work;extern void PCBox_SlideOutActionMenu(void*),ov14_021E8328(void*),ov14_021E884C(void*),PCBox_SetSelectedIconOutlineVisible(Work*,int),PCBox_WaitForUpdateFunc(Work*,void*,int);extern u8 ov14_021EA130[];

void ov14_021F27D4(Work*w);

void ov14_021F27D4(Work*w){PCBox_SlideOutActionMenu(*(void**)((u8*)w->ctx+0x2f0));ov14_021E8328(*(void**)((u8*)w->ctx+0x2f0));ov14_021E884C(w->ctx);PCBox_SetSelectedIconOutlineVisible(w,0);PCBox_WaitForUpdateFunc(w,ov14_021EA130,89);}
