#pragma force_active on
#include "global.h"
extern int sub_02091C38(void*,u16);extern int sub_02091C74(void*,u16);
int ov102_021E85A8(void*,int,u16);
int ov102_021E85A8(void*w,int flag,u16 id) {if(id==254 || id==255)return 0;if(flag==0){if(sub_02091C38(w,id))return 1;return 0;}if(sub_02091C74(w,id))return 1;return 0;}
