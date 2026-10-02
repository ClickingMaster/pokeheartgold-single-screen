#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Entry{u8 pad0[16];void*object;u8 pad14[36];}Entry;typedef struct Work{u8 pad0[16];u8 count;u8 pad11[0x1f3];Entry entries[4];u8 context[32];}Work;
extern int ov90_02259F7C(Entry*);extern void ov90_0225A134(void*,Entry*);int ov90_02259B68(Work*);
int ov90_02259B68(Work*w){int i;int all;all=1;i=0;if(i<w->count){Entry*n=w->entries;Work*walk=w;Entry*p=n;do{if(walk->entries[0].object){int ready=ov90_02259F7C(n);ov90_0225A134(w->context,p);if(!ready)all=0;}else all=0;walk=(Work*)((u8*)walk+56);n++;p++;i++;}while(i<w->count);}return all;}
