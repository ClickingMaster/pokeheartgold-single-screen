#pragma force_active on
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
typedef struct Chain Chain;

struct Chain {
    Chain *next;
    void (*destructor)(void *, int);
    void *object;
};

extern Chain *__global_destructor_chain;
void __destroy_global_chain(void);

void __destroy_global_chain(void) {
    Chain *node = __global_destructor_chain;
    if (node) {
        do {
            __global_destructor_chain = node->next;
            node->destructor(node->object, -1);
            node = __global_destructor_chain;
        } while (node);
    }
}
