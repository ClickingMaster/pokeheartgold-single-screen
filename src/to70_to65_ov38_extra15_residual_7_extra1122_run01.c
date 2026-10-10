#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef void *(*Alloc)(u32, int);
typedef void (*Free)(void *);

typedef struct Work {
    int pad, result;
    void *bufB;
    Alloc alloc;
    void *bufA;
    int active, value, error;
    Free release;
    int minus, zero28, zero2c, zero30;
} Work;

extern Work ov38_02226050;
extern char ov38_02226090[];
extern char ov38_022260D8[];
extern char ov38_02226084[];
extern char ov38_022260BC[];
extern char ov38_022260A4[];
extern void ov38_02221810(void *);
extern void ov38_02220810(void *);
extern void ov38_02221730(void);
extern void *ov38_022203E8(char *);
extern int ov38_022201C8(char *, char *, int);
extern int ov38_0222175C(void);
extern int ov38_02221718(void);
extern int ov38_022219FC(Alloc, Free, int);
extern int ov38_02220684(void *);
int ov38_02220434(Alloc, Free, char *, char *, char *, char *, int);

int ov38_02220434(Alloc alloc, Free release, char *a, char *b, char *small, char *big, int value) {
    ov38_02226050.alloc = alloc;
    ov38_02226050.release = release;
    ov38_02226050.value = value;
    ov38_02226050.error = 0;
    ov38_02226050.zero28 = 0;
    ov38_02226050.zero2c = 0;
    ov38_02226050.zero30 = 0;
    ov38_02226050.result = 0;
    ov38_02226050.minus = -1;
    ov38_02226050.bufB = 0;
    ov38_02226050.bufA = 0;
    ov38_02221810(ov38_02226090);
    ov38_02220810(ov38_022260D8);
    if (!(ov38_02226050.bufA = ov38_022203E8(a))) {
        goto failed;
    }
    if (!(ov38_02226050.bufB = ov38_022203E8(b))) {
        goto failed;
    }
    ov38_02226084[ov38_022201C8(ov38_02226084, small, 4)] = 0;
    ov38_022260BC[ov38_022201C8(ov38_022260BC, big, 16)] = 0;
    if (ov38_0222175C() && ov38_02221718()) {
        if (ov38_022219FC(alloc, release, 10)) {
            ov38_02226050.result = ov38_02220684(ov38_022260A4);
            ov38_02226050.active = 1;
            return 1;
        }
        ov38_02221730();
    }
    ov38_02226050.release(ov38_02226050.bufB);
    ov38_02226050.release(ov38_02226050.bufA);
    ov38_02226050.error = 8;
    return 0;
failed:
    if (ov38_02226050.bufB) {
        ov38_02226050.release(ov38_02226050.bufB);
    }
    if (ov38_02226050.bufA) {
        ov38_02226050.release(ov38_02226050.bufA);
    }
    ov38_02226050.error = 1;
    return 0;
}
