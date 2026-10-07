#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    int len;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
} Req;
typedef struct {
    u8 pad0[0x428];
    void *mutex;
    int count;
} Inner;
typedef struct {
    Inner *inner;
} Outer;
extern const char ov00_02219290[];
extern const char ov00_02219298[];
extern const char ov00_02219228[];
void __msl_assertion_failed(const char *expr, const char *file, const char *func, int line);
int ov00_02207DD0(Outer *o, int len, int *out);
void ov00_021FEF74(void *mutex, Req *req);
int ov00_02207CEC(Outer *o, int len);
// @fn ov00_02207CEC
int ov00_02207CEC(Outer *o, int len) {
    Inner *inner = o->inner;
    int out;
    if (!(len > 0)) {
        __msl_assertion_failed(ov00_02219290, ov00_02219298, ov00_02219228, 0x2B5);
    }
    if (len <= 0) {
        return 0;
    }
    if (ov00_02207DD0(o, len, &out)) {
        return out;
    }
    {
        Req req = {0};
        req.len = len;
    req.a = 0;
    req.c = 0;
    req.d = 0;
    req.f = 0;
    req.e = 0;
        ov00_021FEF74(inner->mutex, &req);
    }
    inner->count++;
    if (ov00_02207DD0(o, len, &out)) {
        return out;
    }
    return 0;
}
