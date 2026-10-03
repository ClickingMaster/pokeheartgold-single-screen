#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct {u16 species;u8 p;u8 form:4;u8 language:4;}Record;
typedef struct {u32 p;u32 flagA:1;u32 flagB:1;u32 q:30;}Work;
extern const u8 gGameLanguage;
void ov48_022593F4(Work*,Record**);
void ov48_022593F4(Work*w,Record**records){int i,offset;w->flagB=0;w->flagA=0;if(gGameLanguage==1)w->flagB=1;else w->flagB=0;if(w->flagB==1){for(i=0,offset=0;i<50;i++,offset+=4){if(((u32)(*(u8*)((u32)*records+offset+3)<<24)>>28)==1&&*(u16*)((u8*)*records+offset)!=103)w->flagA=1;}}else w->flagA=1;}
