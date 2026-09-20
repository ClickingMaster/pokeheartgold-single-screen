#include "to55_overlay13_r17_private.h"

int ov13_022245C4(int socket, const void *addr, const void *unused, const void *data, int length) {
    int result = SOC_SendTo(socket, data, length, 0, addr);
    if (result < 0) {
        result = -4;
    }
    return result;
}

int ov13_022245DC(int socket, const void *data, int length) {
    struct {
        u8 host[4];
        u8 size;
        u8 family;
        u16 port;
        s32 tail;
    } addr;
    addr.size = 8;
    addr.family = 2;
    addr.tail = -1;
    addr.port = 0x1e6;
    SOC_U32to4U8(SOCL_GetHostID(), addr.host);
    return ov13_022245C4(socket, &addr.size, addr.host, data, length);
}

int ov13_02224618(int socket, int unused, const void *data, int length) {
    return ov13_022245DC(socket, data, length);
}
