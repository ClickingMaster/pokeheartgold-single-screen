#include "overlay02_port_private.h"

WIP_LOCAL void *ov02_0224955C(void *a0) {
    void *ptr = Heap_AllocAtEnd(HEAP_ID_FIELD1, 0x2f8);
    memset(ptr, 0, 0x2f8);
    *(void **)((u8 *)ptr + 0x60) = a0;
    return ptr;
}
