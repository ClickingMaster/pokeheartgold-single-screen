#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

typedef struct {
    u8 *data;
    int capacity, length, offset;
} Buffer;

extern char ov00_022183F8[];
extern char ov00_02218410[];
extern char ov00_02218380[];
extern char ov00_0221857C[];
extern char ov00_02218588[];
extern char ov00_02218594[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern int ov00_02202154(void *, int, void *, int, int *, int *, int);
int ov00_0220267C(void *, int, Buffer *, int *, int, int);
#pragma force_active on

int ov00_0220267C(void *context, int arg, Buffer *buffer, int *out, int shift, int mode) {
    if (!buffer) {
        __msl_assertion_failed(ov00_022183F8, ov00_02218410, ov00_02218380, 441);
    }
    int processed = 0, left;
    u8 *data;
    int offset, total;
    total = buffer->length;
    offset = buffer->offset;
    data = buffer->data;
    left = total - offset;
    int ret, done, used;
    if (left == 0) {
        return processed;
    }
    do {
        ret = ov00_02202154(context, arg, data + (offset + processed), left, &done, &used, mode);
        if (ret) {
            return ret;
        }
        if (used) {
            left -= used;
            processed += used;
        }
    } while (used && left);
    if (shift) {
        if (processed > 0) {
            memmove(data, data + processed, left + 1);
            total -= processed;
        }
    } else {
        offset += processed;
    }
    if (total < 0) {
        __msl_assertion_failed(ov00_0221857C, ov00_02218410, ov00_02218380, 478);
    }
    if (offset < 0) {
        __msl_assertion_failed(ov00_02218588, ov00_02218410, ov00_02218380, 479);
    }
    if (offset > total) {
        __msl_assertion_failed(ov00_02218594, ov00_02218410, ov00_02218380, 480);
    }
    buffer->length = total;
    buffer->offset = offset;
    if (out) {
        *out = done;
    }
    return 0;
}
