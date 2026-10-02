#pragma force_active on
#include "global.h"
typedef struct {u8 p[13];u8 flag;u8 q[6];u8 index;u8 r[0x3b];u8 windows[35][16];u8 s[0x2c4];void *obj,*unused;u8 *map;u8 t[0x70];void *msg;u16 id;u8 z[6];void *arg;} W;
extern void ScheduleWindowCopyToVram(void*);extern void ClearWindowTilemapAndScheduleTransfer(void*);extern void ov83_0224755C(void*,int);extern int ov83_02247768(int,int);extern void ov83_02247668(void*,void*,int,void*);
void ov83_02246938(W*,int);void ov83_02246988(W*);
void ov83_02246988(W*w) {int n=ov83_02247768(w->index,w->flag);if(w->map[n]==0)ov83_02247668(w->obj,w->msg,0,w->arg);else ov83_02247668(w->obj,w->msg,w->id,w->arg);}
