#pragma force_active on
#include "global.h"
typedef struct{int active,flag;void *sprite,*task;} Entry;
typedef struct{u8 p[0x284];void *bg;u8 p2[0xaa8];u32 count;Entry entries[3];} Work;
extern void *ov96_021EA7A4(int,int,int,void *),*SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);
extern void ov96_021E81D8(void *,void *);
void *ov96_021E64F8(Work *,int,int,int,int);
void *ov96_021E64F8(Work *w,int flag,int a,int b,int c){Entry *e;if(w->count>=3){GF_AssertFail();return 0;}e=&w->entries[(u8)w->count];e->active=1;e->flag=flag;e->sprite=ov96_021EA7A4(a,b,c,w->bg);e->task=SysTask_CreateOnMainQueue(ov96_021E81D8,e,1);w->count++;return e->sprite;}
