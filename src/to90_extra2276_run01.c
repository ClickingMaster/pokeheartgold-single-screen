#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x208f];u8 mode;}Work;BOOL ov08_0221DB04(void*);int ov08_0221DB24(void*,u32);
int ov08_0221DB24(void*opaque,u32 bits){Work*w=opaque;if(ov08_0221DB04(w)==1){if(w->mode==2){if(bits&1)return 1;}else{if(!(bits&1))return 1;}}return 0;}
