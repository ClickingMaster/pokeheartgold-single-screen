#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov08_022247E4(void*,u8,int,int);extern int ov08_02223CD4(void*,u32);
void ov08_02224A50(void*,int);
void ov08_02224A50(void*s,int mode){u32 i;switch(mode){case 0:ov08_022247E4(s,0,0,mode);ov08_022247E4(s,1,0,mode);ov08_022247E4(s,2,0,mode);ov08_022247E4(s,3,0,mode);if(*(u16*)((u8*)*(void**)s+0x20)==0)ov08_022247E4(s,4,3,mode);else ov08_022247E4(s,4,0,mode);ov08_022247E4(s,5,0,mode);break;case 1:for(i=0;i<6;i++){if(ov08_02223CD4(s,i)==0)ov08_022247E4(s,(u8)(i+6),3,mode);else ov08_022247E4(s,(u8)(i+6),0,mode);}if(*((u8*)s+0x1154+*((u8*)s+0x114d))==0){ov08_022247E4(s,12,3,mode);ov08_022247E4(s,13,3,mode);}else{ov08_022247E4(s,12,0,mode);ov08_022247E4(s,13,0,mode);}ov08_022247E4(s,14,0,mode);break;case 2:ov08_022247E4(s,15,0,mode);ov08_022247E4(s,16,0,mode);break;}}
