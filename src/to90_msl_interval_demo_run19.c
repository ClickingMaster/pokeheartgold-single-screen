#pragma force_active on
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u8 _02111734;
extern const u8 _ZTVSt9bad_alloc[];
extern const u8 _02111788[];
void _ZNSt9bad_allocD1Ev(void);
void __throw(const void *, void *, void (*)(void));
void _ZSt17__throw_bad_allocv(void);

void _ZSt17__throw_bad_allocv(void) {
    const void *vtable;
    if (_02111734) {
        vtable = _ZTVSt9bad_alloc;
        __throw(_02111788, &vtable, _ZNSt9bad_allocD1Ev);
    }
}
