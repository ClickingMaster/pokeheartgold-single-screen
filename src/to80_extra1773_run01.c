#pragma force_active on
#include "global.h"
typedef struct{u8 p[52];u8 *data;}Host;void ov14_021E5EFC(Host*,int);int ov14_021F0254(Host*,int);
int ov14_021F0254(Host*h,int state){if(state==1)ov14_021E5EFC(h,1);else ov14_021E5EFC(h,0);*(u16*)(h->data+0x438)=state;return 7;}
