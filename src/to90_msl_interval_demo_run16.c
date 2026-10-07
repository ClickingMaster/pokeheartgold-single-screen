#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{void*object;u32 unused;void(*destroy)(void*,int);} Catch;
void __end__catch(Catch*);
void __end__catch(Catch*c){if(c->object&&c->destroy)c->destroy(c->object,-1);}
