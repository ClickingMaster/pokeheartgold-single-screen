#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    void *vtable;
    const char *name;
} Type;

int strcmp(const char *, const char *);
int _ZNKSt9type_infoeqERKS_(const Type *, const Type *);

int _ZNKSt9type_infoeqERKS_(const Type *a, const Type *b) {
    if (a == b || strcmp(a->name, b->name) == 0) {
        return 1;
    }
    return 0;
}
