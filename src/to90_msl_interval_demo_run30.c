#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern void _ZNSt9type_infoD2Ev(void *);
extern void _ZdlPv(void *);
void *_ZN10__cxxabiv117__class_type_infoD0Ev(void *);

void *_ZN10__cxxabiv117__class_type_infoD0Ev(void *p) {
    _ZNSt9type_infoD2Ev(p);
    _ZdlPv(p);
    return p;
}
