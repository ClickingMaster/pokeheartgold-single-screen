#pragma force_active on
#include "global.h"
extern int FieldSystem_ShouldDrawStartMenuIcon(void *,int);
extern void CopyWindowToVram(void *);
extern void Sprite_SetDrawFlag(void *,int);
typedef struct {u8 p[16];void *field;u8 p2[0x3c8 - 20];void *sprite;u8 p3[0x3e0 - 0x3cc];u8 mainWindow[16];u8 windows[8][16];} Work;
void ov27_0225BC84(Work *w);
void ov27_0225BC84(Work *w){int draw[8],i,count=0;for(i=0;i<8;i++)draw[i]=FieldSystem_ShouldDrawStartMenuIcon(w->field,i);for(i=0;i<8;i++){if(draw[i]){CopyWindowToVram(w->windows[i]);count++;}}if(count){Sprite_SetDrawFlag(w->sprite,1);CopyWindowToVram(w->mainWindow);}}
