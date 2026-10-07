#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0xa],flagA,p2[2],choice,p3;u8 bit0:2,flags:1,bit3:5;u8 p4[4];u8 kind;u8 p5[0xab];u8 window[0x1f8];void*options;u8 p6[0x2a0];void*party;u8 p7[0x98];void*buttons;u8 p8[8];u8 stuff[4];} Work;
extern int Options_GetFrame(void*);
extern void ov83_02247944(void*,int);
extern int ov83_02247768(int,int);
extern void* Party_GetMonByIndex(void*,int);
extern void* Mon_GetBoxMon(void*);
extern void ov83_02244AB0(Work*,int,void*);
extern int ov83_022448AC(Work*,int,int);
extern void ov83_02245300(Work*,int);
extern void ov83_02245C80(Work*);
extern void ov83_02245D48(Work*);
extern void ov83_02246114(Work*,int);
extern void PlaySE(int);
extern void TouchscreenListMenu_DestroyButtons(void*);
extern void ov83_02246C70(Work*);
extern void ov83_022478B4(void*);
void ov83_02245318(void *, int);
void ov83_02245390(void *);
void ov83_02245318(void *parent,int choice){Work *w=parent;ov83_02247944(w->window,Options_GetFrame(w->options));ov83_02244AB0(w,0,Mon_GetBoxMon(Party_GetMonByIndex(w->party,ov83_02247768(w->kind,choice))));w->flagA=ov83_022448AC(w,0x53,1);ov83_02245300(w,choice);ov83_02245C80(w);if(w->choice==choice){ov83_02245D48(w);ov83_02246114(w,0);}PlaySE(0x623);}
void ov83_02245390(void *parent){Work *w=parent;if(w->flags==1){TouchscreenListMenu_DestroyButtons(w->buttons);ov83_02246C70(w);}ov83_022478B4(w->stuff);}
