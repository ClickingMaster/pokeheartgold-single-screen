#pragma force_active on
#include "global.h"
typedef struct {int active,count,a,b,id;} Entry;typedef struct {u8 pad[0xb54];Entry entries[5];} Work;
void ov85_021E7E88(Entry*,int);
void ov85_021E7E88(Entry*w,int id){int i;Entry *scan;w=(Entry*)((u8*)w+0xb54);scan=w;for(i=0;i<5;i++,scan++){if(scan->active==1&&scan->id==id){scan->count=0;return;}}for(i=0;i<5;i++,w++){if(w->active==0){w->active=1;w->id=id;return;}}GF_AssertFail();}
