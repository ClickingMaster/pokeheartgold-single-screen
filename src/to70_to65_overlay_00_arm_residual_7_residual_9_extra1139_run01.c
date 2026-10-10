#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern long atol(const char *);

extern void *memcpy(void *, const void *, u32);
extern int strncmp(const char *, const char *, u32);

typedef struct {
    u8 p[0xc];
    u32 callback, arg;
} Entry;

extern const char ov00_0221923C[];
extern const char ov00_02219244[];
extern const char ov00_02219274[];
extern const char ov00_02219280[];
extern const char ov00_02219290[];
extern const char ov00_02219298[];
extern const char ov00_02219228[];
extern int ov00_0220A3C8(void *, char *, int);
extern int ov00_0220A4A0(char *, const char *, char *, int);
extern void ov00_0220A86C(void *, int, const char *);
extern void ov00_02202990(void *, int, int);
extern void ov00_0220A890(void *, const char *);
extern void ov00_022066D0(void *, Entry *);
extern void *ov00_021EC3A8(int);

typedef struct {
    u32 callback, arg;
} Call;

extern int ov00_02202A84(void *, Call, void *, Entry *, int);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern int ov00_02207DD0(void *, int, int *);
extern void ov00_021FEF74(void *);

typedef struct {
    int a, b, c, d, e, f, g;
} Temp;

static inline void initialize(volatile Temp *t) {
    t->a = 0;
    t->b = 0;
    t->d = 0;
    t->e = 0;
    t->f = 0;
    t->g = 0;
}

int ov00_02207B8C(void *, Entry *, char *);
int ov00_02207CEC(void *, int);

int ov00_02207B8C(void *w, Entry *entry, char *text) {
    char buf[16];
    Call call;
    int value, result;
    int *data;
    if (ov00_0220A3C8(w, text, 1) != 0) {
        return 4;
    }
    if (strncmp(text, ov00_0221923C, 5) != 0) {
        ov00_0220A86C(w, 1, ov00_02219244);
        ov00_02202990(w, 3, 1);
        return 3;
    }
    if (ov00_0220A4A0(text, ov00_02219274, buf, 16) == 0) {
        ov00_0220A86C(w, 1, ov00_02219244);
        ov00_02202990(w, 3, 1);
        return 3;
    }
    value = atol(buf);
    call.callback = entry->callback;
    call.arg = entry->arg;
    if (call.callback) {
        data = ov00_021EC3A8(8);
        if (data == 0) {
            ov00_0220A890(w, ov00_02219280);
            return 1;
        }
        data[1] = value;
        data[0] = 0;
        result = ov00_02202A84(w, call, data, entry, 0);
        if (result != 0) {
            return result;
        }
    }
    ov00_022066D0(w, entry);
    return 0;
}
