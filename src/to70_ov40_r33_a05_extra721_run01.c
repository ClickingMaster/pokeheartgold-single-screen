#pragma force_active on
#include "global.h"
extern void ClearWindowTilemapAndCopyToVram(void*),RemoveWindow(void*);
typedef struct{u8 p[20];u8 windows1[8][16],windows2[8][16];u8 q[0x1f18];int count2,count1;}Data;
typedef struct{u8 p[0x860];Data*data;}Work;
void ov40_0223B538(Work*);void ov40_0223B574(Work*);
void ov40_0223B538(Work*w){Data*d=w->data;int i;u8*win;for(i=0;i<d->count1;i++){win=d->windows1[i];ClearWindowTilemapAndCopyToVram(win);RemoveWindow(win);}d->count1=0;}
void ov40_0223B574(Work*w){Data*d=w->data;int i;u8*win;for(i=0;i<d->count2;i++){win=d->windows2[i];ClearWindowTilemapAndCopyToVram(win);RemoveWindow(win);}d->count2=0;}
