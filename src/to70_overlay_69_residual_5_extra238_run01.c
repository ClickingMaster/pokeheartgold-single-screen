#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct MenuTemplate{void *items;void (*cursor)(void);void *print;void *window;u16 count,max;u32 tail[3];} MenuTemplate;
typedef struct Entry{int msg,value;}Entry;
void AddWindow(void *,void *,void *);void *ListMenuItems_New(int,int);
void ListMenuItems_AppendFromMsgData(void *,void *,int,int);
void *ListMenuInit(MenuTemplate *,int,int,int);
void DrawFrameAndWindow1(void *,int,int,int);void CopyWindowToVram(void *);
void ov69_021E68D8(void);void ov69_021E68EC(u8 *,void *,void *,const MenuTemplate *,const Entry *);
#define P(o) (*(void **)(w+(o)))
#define W(o) (*(int *)(w+(o)))
void ov69_021E68EC(u8 *w,void *win,void *wt,const MenuTemplate *orig,const Entry *e){
 MenuTemplate t;int i;
 AddWindow(P(0xc010),win,wt);P(0xc068)=ListMenuItems_New(orig->count,W(0));i=0;
 if((int)orig->count>0){const Entry *p=e;do{ListMenuItems_AppendFromMsgData(P(0xc068),P(0xc070),p->msg,p->value);i++;p++;}while(i<orig->count);}
 t=*orig;t.items=P(0xc068);t.window=win;t.cursor=ov69_021E68D8;
 P(0xc064)=ListMenuInit(&t,0,0,(u8)W(0));DrawFrameAndWindow1(t.window,1,473,7);CopyWindowToVram(win);
}
