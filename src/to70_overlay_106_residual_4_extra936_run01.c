#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8*inner;}Work;
extern u8 ov106_021E6EFC[],ov106_021E6EC4[],ov106_021E6EE4[];
extern void ov106_021E5900(Work*),ov106_021E59FC(Work*,void*),ov106_021E5A44(void*),ov106_021E5A74(void*),ov106_021E5AA8(Work*,int,void*,int),ov106_021E5954(Work*,int,int,int);

int ov106_021E6280(Work*w);

int ov106_021E6280(Work*w){ov106_021E5900(w);ov106_021E59FC(w,ov106_021E6EFC);ov106_021E5A44(w->inner+4);ov106_021E5A74(ov106_021E6EC4);ov106_021E5AA8(w,246,ov106_021E6EE4,2);ov106_021E5954(w,6,1,3);return 2;}
