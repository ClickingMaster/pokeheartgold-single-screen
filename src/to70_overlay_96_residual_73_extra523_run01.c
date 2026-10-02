#pragma force_active on
#include "global.h"
typedef struct{void *obj;int index;} Entry;
extern void *ov96_021FBE44(void *,u8);
extern int ov96_021FBE9C(void *);
extern void ov96_021FBEDC(void *,const VecFx32 *),ov96_021FBE54(void *,int);
extern VecFx32 ov96_0221C5B8;
void *ov96_021FBF90(Entry *,int,int);
void *ov96_021FBF90(Entry *e,int n,int y){void *s;VecFx32 pos;s=ov96_021FBE44(e[n].obj,(u8)e[n].index);if(ov96_021FBE9C(s)){GF_AssertFail();return 0;}e[n].index=(e[n].index+1)%4;pos=ov96_0221C5B8;pos.y+=y<<12;switch(n){case 0:pos.x=3<<16;break;case 1:pos.x=8<<16;break;case 2:pos.x=13<<16;break;}ov96_021FBEDC(s,&pos);ov96_021FBE54(s,1);return s;}
