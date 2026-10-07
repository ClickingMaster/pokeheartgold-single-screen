#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct{u8 pad[16];u16 state;u8 gap[2];BgConfig*bg;u8 rest[0x2f0-0x18];void*tiles;}Data;typedef struct{u8 gap[0x34];Data*data;}Work;
extern void*sub_02019B08(void*,int);int ov14_021E96C8(Work*);
int ov14_021E96C8(Work*w){Data*d=w->data;void*tiles=sub_02019B08(d->tiles,0);switch(d->state){case 0:CopyToBgTilemapRect(w->data->bg,0,0,9,32,1,tiles,0,3,32,7);break;case 1:CopyToBgTilemapRect(w->data->bg,0,0,8,32,1,tiles,0,0,32,7);CopyToBgTilemapRect(w->data->bg,0,0,10,32,1,tiles,0,6,32,7);break;case 2:CopyToBgTilemapRect(w->data->bg,0,0,7,32,2,tiles,0,0,32,7);CopyToBgTilemapRect(w->data->bg,0,0,10,32,2,tiles,0,5,32,7);break;case 10:FillBgTilemapRect(w->data->bg,0,0,0,4,32,11,16);case 3:CopyToBgTilemapRect(w->data->bg,0,0,6,32,7,tiles,0,0,32,7);break;case 4:CopyToBgTilemapRect(w->data->bg,0,0,5,32,2,tiles,0,0,32,7);CopyToBgTilemapRect(w->data->bg,0,0,12,32,2,tiles,0,5,32,7);break;case 5:CopyToBgTilemapRect(w->data->bg,0,0,4,32,2,tiles,0,0,32,7);CopyToBgTilemapRect(w->data->bg,0,0,13,32,2,tiles,0,5,32,7);break;}ScheduleBgTilemapBufferTransfer(w->data->bg,0);if(d->state==10){d->state=0;return 0;}d->state++;return 1;}
