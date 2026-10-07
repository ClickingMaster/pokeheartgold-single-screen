#pragma force_active on
#include "global.h"
typedef struct{u8 pad[10];u8 printer;u8 a[2];u8 selected;u8 b[6];u8 mode;u8 c[0x2b8-0x15];void*options;u8 d[0x55c-0x2bc];void*party;} Work;
extern int Options_GetFrame(void*);extern void ov83_02247944(void*,int);extern int ov83_02247768(u8,int);extern void*Party_GetMonByIndex(void*,int);extern void*Mon_GetBoxMon(void*);extern void ov83_02244AB0(Work*,int,void*);extern u8 ov83_022448AC(Work*,int,int);extern void ov83_02245270(Work*,int),ov83_02245C80(Work*),ov83_02245D48(Work*),ov83_02246114(Work*,int),PlaySE(int);

void ov83_02245288(void *, int);

void ov83_02245288(void *parent,int selected){Work *w=parent;ov83_02247944((u8*)w+0xc0,Options_GetFrame(w->options));ov83_02244AB0(w,0,Mon_GetBoxMon(Party_GetMonByIndex(w->party,ov83_02247768(w->mode,selected))));w->printer=ov83_022448AC(w,47,1);ov83_02245270(w,selected);ov83_02245C80(w);if(w->selected==selected){ov83_02245D48(w);ov83_02246114(w,0);}PlaySE(0x623);}
