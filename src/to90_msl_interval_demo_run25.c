#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void(*Initializer)(void);extern Initializer __sinit__[];void __call_static_initializers(void);
void __call_static_initializers(void){Initializer*it=__sinit__;while(it&&*it){(*it)();it++;}}
