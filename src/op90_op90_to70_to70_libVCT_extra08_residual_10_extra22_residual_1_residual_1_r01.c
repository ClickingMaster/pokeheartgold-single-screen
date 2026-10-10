#pragma force_active on
#include "global.h"

#include "to90_vct_ssp_private.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Header {
    u32 magic;
    u8 kind, size, type, pad, id, target, notify, peer;
    u32 mask;
} Header;

int vct_build_ssp_header(Session *, Header *, u32);

int vct_build_ssp_header(Session *s, Header *h, u32 kind) {
    h->magic = 0x5f564354;
    h->size = 16;
    h->type = kind;
    h->id = __vct.aid;
    h->target = s->aid;
    h->notify = 0;
    h->peer = __vct.aid;
    h->mask = 0;
    return 0;
}
