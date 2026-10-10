#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Buffer {
    char *data;
    int capacity, length;
} Buffer;

extern char ov00_022184C4[];
extern char ov00_02218410[];
extern char ov00_02218370[];
extern char ov00_022184DC[];
extern char ov00_022184F0[];
extern char ov00_02218504[];
extern char ov00_0221841C[];
extern char ov00_02218518[];
extern char ov00_02218544[];
extern char ov00_02218568[];
void __msl_assertion_failed(char *, char *, char *, int);
void *ov00_021EC3BC(void *, int);
void ov00_0220A890(void *, char *);
int ov00_021FF92C(int, char *, int, int);
int ov00_021FFA60(int);
void ov00_0220A3BC(void *, char *, int, ...);
int ov00_02202454(void *, int, Buffer *, int *, int *, int);
#pragma force_active on
#pragma opt_lifetimes off

int ov00_02202454(void *ctx, int socket, Buffer *buffer, int *count, int *closed, int tag) {
    char *data;
    int length, capacity, read, total, done;

    struct {
        int a, b, c;
    } errors;

    if (socket == -1) {
        __msl_assertion_failed(ov00_022184C4, ov00_02218410, ov00_02218370, 0x150);
    }
    if (!buffer) {
        __msl_assertion_failed(ov00_022184DC, ov00_02218410, ov00_02218370, 0x151);
    }
    if (!count) {
        __msl_assertion_failed(ov00_022184F0, ov00_02218410, ov00_02218370, 0x152);
    }
    if (!closed) {
        __msl_assertion_failed(ov00_02218504, ov00_02218410, ov00_02218370, 0x153);
    }
    length = buffer->length;
    data = buffer->data;
    capacity = buffer->capacity;
    errors.c = -76;
    errors.b = -26;
    errors.a = -6;
    total = 0;
    done = 0;
    do {
        if (length + 2048 > capacity) {
            capacity = length + 2048;
            data = ov00_021EC3BC(data, capacity + 1);
            if (!data) {
                ov00_0220A890(ctx, ov00_0221841C);
                return 1;
            }
        }
        read = ov00_021FF92C(socket, data + length, capacity - length, 0);
        if (read == -1) {
            int error = ov00_021FFA60(socket);
            if (error != errors.a && error != errors.b && error != errors.c) {
                ov00_0220A890(ctx, ov00_02218518);
                return 3;
            }
        } else if (read) {
            length += read;
            total += read;
        } else {
            done = 1;
            ov00_0220A3BC(ctx, ov00_02218544, tag);
        }
        data[length] = 0;
    } while (read != -1 && !done && total < 0x20000);
    if (total) {
        ov00_0220A3BC(ctx, ov00_02218568, tag, total);
    }
    buffer->data = data;
    buffer->length = length;
    buffer->capacity = capacity;
    *count = total;
    *closed = done;
    return 0;
}
