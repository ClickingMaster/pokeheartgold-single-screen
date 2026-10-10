#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
extern int sprintf(char *, const char *, ...);
extern const char ov45_02256BA0[];
extern const char ov45_02256B94[];
extern const char ov45_02256A7C[];
extern const char ov45_022570E4[];
extern const char ov45_022570A4[];
extern const char ov45_022570FC[];
extern const char ov45_0225710C[];
extern const char ov45_02257124[];
extern const char ov45_0225713C[];
extern const char ov45_02256AA4[];
extern const char ov45_022570B8[];

typedef struct {
    char b[5];
} Bytes5;

typedef struct {
    char b[2];
} Bytes2;

extern const Bytes5 ov45_02257140;
extern const Bytes2 ov45_02256BC0;
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02252CB4(void *, const char *);
extern char *ov45_022514F4(void);
extern int ov45_0224ABA0(void *, char *, int, const char **, int, void *, void *);
extern int ov45_0224AA2C(void *, char *, int, const char **, int, int, void *, void *);
extern void ov45_0224F770(void *, int);
extern void ov00_021FF3C0(int);
extern int ov45_0224F660(void *, int);

int ov45_02251650(void *w, const char *a, const char *b, const char *c, int count, const char **entries);
void ov45_022518F0(void *w, const char *a, const char *b, int count, const char **entries, void *cb, void *arg, int wait);

void ov45_022518F0(void *w, const char *a, const char *b, int count, const char **entries, void *cb, void *arg, int wait) {
    char *name;
    int seen, result;
    if (w == 0) {
        __msl_assertion_failed(ov45_02256BA0, ov45_02256B94, ov45_02256AA4, 0xb6f);
    }
    if (w == 0 || *(void **)w == 0) {
        return;
    }
    if (count < 0) {
        __msl_assertion_failed(ov45_022570B8, ov45_02256B94, ov45_02256AA4, 0xb72);
    }
    if (count != 0 && entries == 0) {
        __msl_assertion_failed(ov45_022570FC, ov45_02256B94, ov45_02256AA4, 0xb73);
    }
    name = ov45_022514F4();
    seen = ov45_02251650(w, a, b, name, count, entries);
    if (b == 0 || *b == 0) {
        result = ov45_0224ABA0(w, name, count, entries, seen, cb, arg);
    } else {
        result = ov45_0224AA2C(w, name, count, entries, strcmp(b, ov45_02256BC0.b) == 0, seen, cb, arg);
    }
    if (wait) {
        do {
            ov45_0224F770(w, result);
            ov00_021FF3C0(10);
        } while (ov45_0224F660(w, result) != 0);
    }
}
