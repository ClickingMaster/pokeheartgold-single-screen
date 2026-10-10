#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Address {
    u8 bytes[8];
} Address;

int SOC_SendTo(int, const void *, int, int, const void *);
int ov00_021FF7EC(int, int);
int ov00_021FF984(int, const void *, int, int, Address *, int);

int ov00_021FF984(int fd, const void *data, int size, int flags, Address *address, int len) {
    Address temp;
    memcpy(&temp, address, 8);
    temp.bytes[0] = len;
    return ov00_021FF7EC(SOC_SendTo(fd, data, size, flags, &temp), -1);
}
