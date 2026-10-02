#pragma force_active on
#include "global.h"
typedef struct{u8 p[8];u16 mode,page;u8 q[0x20];u8 *tiles[4];} Work;typedef struct{void *bg;} UI;extern u8 ov49_022696EC[];extern s8 ov49_022696F0[];extern void CopyToBgTilemapRect(void *,int,int,int,int,int,void *,int,int,int,int),ScheduleBgTilemapBufferTransfer(void *,int),ov49_0225BB14(UI *,void *,int,int,int,int),ScheduleSetBgPosText(void *,int,int,int);void ov49_0225CAD4(Work *,UI *,int,void *,int);
void ov49_0225CAD4(Work *w,UI *ui,int index,void *a,int b){CopyToBgTilemapRect(ui->bg,5,0,3,32,21,w->tiles[index]+12,0,3,32,32);ScheduleBgTilemapBufferTransfer(ui->bg,5);if(w->mode==1)ov49_0225BB14(ui,a,ov49_022696EC[index]+w->page*2+96,6,320,b);ScheduleSetBgPosText(ui->bg,6,3,ov49_022696F0[index]);}
