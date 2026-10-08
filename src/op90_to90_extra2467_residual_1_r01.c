#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u16 head; u16 tail; } Ring;
typedef struct { u8 pad[0xf8]; int size; u8 pad2[4]; Ring ring; } SendBuf;
typedef struct { u8 pad[0x68]; SendBuf *send; } Socket;
int SOCLi_GetWriteBufferFreeSize(Socket *socket);
static inline int RingGetFreeSize(Ring *ring, int size) {
    int head = ring->head;
    int n = ring->tail - head - 1;
    if (n < 0) n += size;
    return n;
}
#pragma push
#pragma opt_propagation off
// @fn SOCLi_GetWriteBufferFreeSize
int SOCLi_GetWriteBufferFreeSize(Socket *socket) {
    SendBuf *buf = socket->send;
    return RingGetFreeSize(&buf->ring, buf->size);
}
#pragma pop
