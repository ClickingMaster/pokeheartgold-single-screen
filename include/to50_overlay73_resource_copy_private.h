#ifndef TO50_OVERLAY73_RESIDUAL41_PRIVATE_H
#define TO50_OVERLAY73_RESIDUAL41_PRIVATE_H
#include "global.h"

#include "heap.h"
typedef struct Ov73Loader {
    u32 (*getSize)(void *ctx);
    void *(*load)(void *ctx, u32 heapId, u32 size);
    void (*finish)(void *);
} Ov73Loader;
extern const Ov73Loader ov73_021EA744[10];
void RecordMixing_BuildSendBuffer(void *ctx, void *dest);
#endif
