#pragma force_active on
#include "global.h"
#pragma force_active on
void *ov102_021EA268(void *);
void ov102_021EA874(void *);
void FillWindowPixelBuffer(void *,u8);
int EasyChatApp_GetType(void *),EasyChatApp_GetWord(void *,int);
void ov102_021EAD70(void *,void *),ov102_021EAD98(void *,void *,void *,int);
int ov102_021EAB30(void *,void *);
void ov102_021EAC20(void *);
void CopyWindowPixelsToVram_TextMode(void *);
void ScheduleBgTilemapBufferTransfer(void *,int);
void ov102_021EAA3C(u8 *);
#define P(p,o) (*(void **)((p)+(o)))
void ov102_021EAA3C(u8 *w){
 int i;
 void *bg=ov102_021EA268(P(w,0));
 u8 *win,*a,*b;
 ov102_021EA874(w);
 for(i=0,win=w+12,a=w+0x84,b=w+0x88;i<2;i++,win+=16){
 FillWindowPixelBuffer(win,13);
 switch(EasyChatApp_GetType(P(w,4))){
 case 0:
 ov102_021EAD70(win,a);ov102_021EAD98(w,win,a,EasyChatApp_GetWord(P(w,4),0));break;
 case 1:
 ov102_021EAD70(win,a);ov102_021EAD70(win,b);
 ov102_021EAD98(w,win,a,EasyChatApp_GetWord(P(w,4),0));
 ov102_021EAD98(w,win,b,EasyChatApp_GetWord(P(w,4),1));break;
 case 2:
 *(int *)(w+0x8c)=ov102_021EAB30(w,win);ov102_021EAC20(w);break;
 }
 CopyWindowPixelsToVram_TextMode(win);
 }
 ScheduleBgTilemapBufferTransfer(bg,0);ScheduleBgTilemapBufferTransfer(bg,4);
}
