#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct {u8 pad[0x1c];int state;u8 pad2[0x78];int count;volatile int selected;u8 pad3[0xb48];void *msg;u8 pad4[0x20c];u8 window[16];} Work;
extern void ov73_021EA0F0(void *,void *,int,int);extern void PlaySE(int);extern void ov73_021EA134(Work *,int,int);
int ov73_021E98FC(Work *);
int ov73_021E98FC(Work *w){if(gSystem.newAndRepeatedKeys&64){w->selected++;if(w->selected==w->count+1)w->selected=1;ov73_021EA0F0(w->window,w->msg,w->selected,3);PlaySE(1500);}else if(gSystem.newAndRepeatedKeys&128){w->selected--;if(w->selected<1)w->selected=w->count;ov73_021EA0F0(w->window,w->msg,w->selected,3);PlaySE(1500);}else if(gSystem.newKeys&1){ov73_021EA134(w,0,0);w->state=35;PlaySE(1500);}else if(gSystem.newKeys&2){ov73_021EA134(w,0,0);w->state=29;PlaySE(1500);}return 3;}
