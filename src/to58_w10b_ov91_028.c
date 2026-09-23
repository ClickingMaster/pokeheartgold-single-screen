#include "global.h"
extern void ov91_022601C8(void *, void *);
void ov91_0225DBC0(u8 *, void *);
void ov91_0225DBC0(u8 *data, void *unused) {
    ov91_022601C8(data + 0x4C0, unused);
}
