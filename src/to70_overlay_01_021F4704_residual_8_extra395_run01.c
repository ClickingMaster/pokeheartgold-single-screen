#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct E{u8 pad[4];int a,b;u8 gap[16];int x,y;u8 c,d,e,pad27;int f;int active;}E;typedef struct S{int a,b;u8 gap[4];u8 id;u8 gapd[3];int x,y;u8 c,d;}S;typedef struct W{E entries[2];u8 gap[4];S source;u8 pad[33];u8 selected;}W;extern int ov01_021F5B88(void*);int ov01_021F5BBC(void*,E*,u8*);void ov01_021F5BF0(W*);
int ov01_021F5BBC(void*obj,E*e,u8*out){int i;for(i=0;i<2;i++,e++){if(e->active==1&&*((u8*)e+48)==ov01_021F5B88(obj)){*out=i;return 1;}}return 0;}
