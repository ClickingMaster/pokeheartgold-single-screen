#pragma force_active on
#include "global.h"
typedef struct {u32 words[17];} Entry;
typedef struct {Entry entry;u16 active,value;u32 id;} Row;
typedef struct {u8 pad[0x5b8];Row rows[3];} Work;
extern int ov45_0223021C(Work *,int);extern u16 ov45_0223023C(Work *,int);
void ov45_02230164(Work *,int,u32,Entry *);
void ov45_02230164(Work *w,int n,u32 id,Entry *entry){GF_ASSERT(n<3);if(ov45_0223021C(w,n)==1&&id!=w->rows[n].id)return;w->rows[n].entry=*entry;if(w->rows[n].active==0){w->rows[n].active=1;w->rows[n].id=id;w->rows[n].value=ov45_0223023C(w,n);}}
