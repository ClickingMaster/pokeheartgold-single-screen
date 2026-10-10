#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
typedef void (*Initializer)(void);
extern Initializer __sinit__[];
void __call_static_initializers(void);

void __call_static_initializers(void) {
    Initializer *it = __sinit__;
    while (it && *it) {
        (*it)();
        it++;
    }
}
