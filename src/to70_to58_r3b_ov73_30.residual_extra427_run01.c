#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct {u8 pad[0x1c];int state;u8 pad2[0x74];volatile int selected;u8 pad3[0xb54];void *msg;u8 pad4[0x1f8];u8 window[16];} Work;
extern void ov73_021EA0F0(void *,void *,int,int);extern void PlaySE(int);extern void ov73_021EA134(Work *,int,int);extern void ov73_021E83EC(Work *,int,int);
int ov73_021E97A8(Work *);
int ov73_021E97A8(Work *w){if(gSystem.newAndRepeatedKeys&64){w->selected++;if(w->selected>10)w->selected=1;ov73_021EA0F0(w->window,w->msg,w->selected,2);PlaySE(1500);}else if(gSystem.newAndRepeatedKeys&128){w->selected--;if(w->selected<1)w->selected=10;ov73_021EA0F0(w->window,w->msg,w->selected,2);PlaySE(1500);}else if(gSystem.newKeys&1){ov73_021EA134(w,0,0);ov73_021E83EC(w,11,32);PlaySE(1500);}else if(gSystem.newKeys&2){ov73_021EA134(w,0,0);w->state=23;PlaySE(1500);}return 3;}
