#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
void _ZNSt9type_infoD2Ev(void *);
void *_ZN10__cxxabiv117__class_type_infoD2Ev(void *);

void *_ZN10__cxxabiv117__class_type_infoD2Ev(void *p) {
    _ZNSt9type_infoD2Ev(p);
    return p;
}
