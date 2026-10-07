#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
void ov00_021F2EE4(int,int);int ov00_021F84C4(int);
static inline void fail(int level){register int code=level;if(code!=0)ov00_021F2EE4(code,-86009);}
#pragma force_active on
#pragma opt_propagation off
int ov00_021F84C4(int a){if(a==0)return 0;switch(a){case 1:return 1;case 2:return 2;default:fail(6);return a;}}
