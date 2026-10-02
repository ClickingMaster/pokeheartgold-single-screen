#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x10];u32 size;void*data;}Char;
typedef struct{u16 width,height;u8 pad[8];u8 data[1];}Screen;
typedef struct{void*bg;u8 pad[0x30ec];Char*c;u8 pad2[0x2c0];Screen*s;}Work;
extern s32 FX_Inv(s32);extern void OS_WaitVBlankIntr(void),BG_LoadCharTilesData(void*,int,void*,u32,int),CopyToBgTilemapRect(void*,int,int,int,int,int,const void*,int,int,int,int),BgCommitTilemapBufferToVram(void*,int);
void ov51_021E6C00(void);void ov51_021E6C6C(Work*);
void ov51_021E6C00(void){MtxFx22 m;int a=FX_Inv(4096);int b=FX_Inv(4096);m._00=a;m._01=0;m._10=0;m._11=b;OS_WaitVBlankIntr();G2x_SetBGyAffine_(0x04001020,&m,128,96,0,0);G2x_SetBGyAffine_(0x04001030,&m,128,96,0,0);OS_WaitVBlankIntr();G2x_SetBGyAffine_(0x04000030,&m,128,0,0,0);}
void ov51_021E6C6C(Work*w){Screen*s;BG_LoadCharTilesData(w->bg,7,w->c->data,w->c->size,0);s=w->s;CopyToBgTilemapRect(w->bg,7,21,5,10,11,s->data,21,5,(u8)(s->width>>3),(u8)(s->height>>3));BgCommitTilemapBufferToVram(w->bg,7);}
