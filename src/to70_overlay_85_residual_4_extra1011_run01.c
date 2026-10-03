#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int state;u8 p[0x2c];int current;u8 p2[0x9c];void *ui;} Work;
extern int ov85_021E8720(Work *),ov85_021E8834(Work *);extern void sub_02096D4C(void *,int,int,int);int ov85_021E5D3C(Work *);
int ov85_021E5D3C(Work *w){if(w->current==ov85_021E8720(w)){if(ov85_021E8834(w)==1){sub_02096D4C(w->ui,16,0,0);w->state=46;}else{sub_02096D4C(w->ui,17,0,0);w->state=15;}}return 0;}
