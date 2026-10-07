#pragma force_active on
#include "global.h"
typedef struct {u8 pad[12];int index;u32 pad2;void *data;u8 pad3[60];int y;} Entry;typedef struct {u8 pad[12];int speed;Entry *entry;void *w;} W;extern void ov85_021E73D4(void *,void *,void*,int);int ov85_021E7C2C(W *);
#pragma force_active on
int ov85_021E7C2C(W *a){Entry *e=a->entry;if(a->speed>4096){a->speed-=4096;if(a->speed<4096)a->speed=4096;}{int y=e->y;e->y=(s32)((u32)y-(u32)((volatile W*)a)->speed);}if(e->y<=0){e->y=0;{int index=e->index;void *work=a->w;ov85_021E73D4(work,*(void**)((u8*)work+(u32)index*4+0x98),e->data,index);}return 2;}return 0;}
