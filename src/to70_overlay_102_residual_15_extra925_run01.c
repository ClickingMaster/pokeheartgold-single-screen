#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x18];void *source;u8 p2[0x1c4];void *a,*b;u8 p3[0xc];u8 active;} Work;
extern void ov102_021EAF5C(void *,int),ov102_021EAF7C(void *),ov102_021EB524(void *,int),ov102_021EB530(void *,int),ov102_021E94A4(Work **);extern int EasyChatApp_GetMenuCursor(void *);void ov102_021E98FC(void *,Work **);
void ov102_021E98FC(void *unused,Work **owner){Work *w=*owner;w->active=1;ov102_021EAF5C(w->a,0);ov102_021EAF7C(w->a);ov102_021EB524(w->b,1);ov102_021EB530(w->b,EasyChatApp_GetMenuCursor(w->source));ov102_021E94A4(owner);}
