#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Request {
    int unused, type;
    void (*callback)(int, int, int);
    int notify;
    void *source;
    int size;
    void *buffer, *first, *second;
} Request;

extern void (*ov38_02226070)(void *);
extern void *(*ov38_0222605C)(int, int);
extern int ov38_0222606C;
void ov38_02220B60(Request *, void *);
int ov38_02221D9C(void *, char *, const void *);
extern char ov38_02226138[];
extern char ov38_02226144[];
extern char ov38_02225E20[];
extern char ov38_02225E40[];
extern char ov38_022260D8[];
extern char ov38_02225E4C[];
extern char ov38_02226084[];
extern char ov38_02225E54[];
extern char ov38_02225E60[];
extern char ov38_022260BC[];
extern char ov38_02225E68[];
extern char ov38_02225E70[];
extern char ov38_02225E78[];
extern char ov38_02226090[];
extern char ov38_02225E80[];
extern char ov38_02225E88[];
extern char ov38_02225E90[];
extern char ov38_02225E98[];
extern char ov38_02225EA0[];
extern char ov38_022260A4[];
extern void (*ov38_02226068)(int, int, int);
extern char *ov38_02226060;
extern char *ov38_02226058;
extern void *ov38_02226078[3];
extern int ov38_02226054;
int ov38_02220784(char *, char *);
int ov38_02221788(void);
void ov38_022217D0(void);
void ov38_02220830(int, void *, Request *);
void *ov38_022225B8(char *, int, void *, int, void (*)(int, void *, Request *), Request *);
int ov38_02221E10(void *, char *, int);
int ov38_02221D78(void *, char *, char *);
int ov38_02220B8C(Request **, void **, const void *, void *, int);

int ov38_02220B8C(Request **out, void **data, const void *label, void *size, int extra) {
    Request *r;
    void *d;
    if (!ov38_02220784(ov38_02226138, ov38_02226144)) {
        ov38_0222606C = 8;
        return 0;
    }
    r = ov38_0222605C(36, 4);
    if (!r) {
        ov38_0222606C = 1;
        return 0;
    }
    r->callback = ov38_02226068;
    r->notify = 0;
    if (!ov38_02221788()) {
        ov38_02226070(r);
        ov38_0222606C = 2;
        return 0;
    }
    d = ov38_022225B8(ov38_02226060, 1, size, extra, ov38_02220830, r);
    if (!d) {
        ov38_022217D0();
        ov38_02226070(r);
        ov38_0222606C = 3;
        return 0;
    }
    *out = r;
    *data = d;
    if (!ov38_02221E10(d, ov38_02225E20, 1)) {
        goto fail;
    }
    if (!ov38_02221D78(d, ov38_02225E40, ov38_022260D8)) {
        goto fail;
    }
    if (!ov38_02221D9C(d, ov38_02225E4C, ov38_02226084)) {
        goto fail;
    }
    if (!ov38_02221D9C(d, ov38_02225E54, ov38_02226138)) {
        goto fail;
    }
    if (!ov38_02221D9C(d, ov38_02225E60, ov38_022260BC)) {
        goto fail;
    }
    if (!ov38_02221D9C(d, ov38_02225E68, ov38_02226058)) {
        goto fail;
    }
    if (!ov38_02221D9C(d, ov38_02225E70, ov38_02226144)) {
        goto fail;
    }
    if (!ov38_02221D9C(d, ov38_02225E78, ov38_02226090)) {
        goto fail;
    }
    if (!ov38_02221D9C(d, ov38_02225E80, label)) {
        goto fail;
    }
    if (ov38_02226078[0] && !ov38_02221D9C(d, ov38_02225E88, ov38_02226078[0])) {
        goto fail;
    }
    if (ov38_02226078[1] && !ov38_02221D9C(d, ov38_02225E90, ov38_02226078[1])) {
        goto fail;
    }
    if (ov38_02226078[2] && !ov38_02221D9C(d, ov38_02225E98, ov38_02226078[2])) {
        goto fail;
    }
    if (ov38_02226054 && !ov38_02221D9C(d, ov38_02225EA0, ov38_022260A4)) {
        goto fail;
    }
    return 1;
fail:
    ov38_02220B60(r, d);
    ov38_0222606C = 1;
    return 0;
}
