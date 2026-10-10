#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef struct HashTable HashTable;
typedef struct DArray DArray;
#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))
void __msl_assertion_failed(char *, char *, char *, int);
int ArrayLength(void *);
void *ov00_021FE6EC(void *, int);
void ov00_021FE968(void *, int);
void ov00_021EC3D8(void *);
void *ov00_021EC3A8(int);
void ov00_021FEE8C(void *);
void ov00_021FE678(void *);
void *ov00_021FF0C0(void *, void *);
void *ov00_021FE5B4(int, int, int);
HashTable *ov00_021FED4C(int, int, int, int (*)(const void *, int), int (*)(const void *, const void *), void (*)(void *));
void ov00_021FE754(void *, void *);
void ov00_021FE8D8(void *, int);
void ov00_021FF01C(void *, void *);
int ov00_021FEA40(DArray *, const void *, int (*)(const void *, const void *), int, int);
int ov00_021FEF00(void *);
int strnicmp(const char *, const char *);
char *strncpy(char *, const char *, size_t);
extern u8 __lower_mapC[];
int ov45_0224739C(void *, int);
int ov45_02247650(const void *, const void *);
extern char ov45_0225565C[];
extern char ov45_02255668[];
extern char ov45_02255678[];
extern char ov45_02255688[];
extern char ov45_022556B0[];
extern char ov45_02255700[];
extern char ov45_02255714[];
extern char ov45_02255728[];
extern char ov45_0225573C[];
extern char ov45_022557A0[];
extern char ov45_022557CC[];
extern char ov45_022557E4[];
extern char ov45_022557FC[];
extern char ov45_02255814[];
extern char ov45_02255844[];
extern char ov45_022558A4[];
extern char ov45_022558D4[];
extern char ov45_022558F0[];
extern char ov45_0225590C[];
extern char ov45_02255918[];
extern char ov45_02255928[];
extern char ov45_02255938[];
extern char ov45_02255948[];
extern char ov45_022559B8[];
extern char ov45_022559C8[];
extern char ov45_022559D4[];
extern char ov45_022559E4[];
extern char ov45_022559F8[];
extern char ov45_02255A2C[];
void ov00_021FF160(void *, void (*)(void *, void *), void *);
extern char ov45_02255420[];
extern char ov45_02255458[];
extern char ov45_02255468[];
extern char ov45_02255478[];
extern char ov45_02255488[];
extern char ov45_022554A4[];
extern char ov45_022554BC[];
extern char ov45_022554DC[];
extern char ov45_022554F0[];
extern char ov45_0225565C[];
extern char ov45_02255668[];
extern char ov45_02255678[];
extern char ov45_02255688[];
extern char ov45_022556B0[];
extern char ov45_022556EC[];
extern char ov45_02255700[];
extern char ov45_02255714[];
extern char ov45_02255728[];
extern char ov45_0225573C[];
extern char ov45_02255750[];
extern char ov45_022557A0[];
extern char ov45_022557B4[];
extern char ov45_022557CC[];
extern char ov45_022557E4[];
extern char ov45_022557FC[];
extern char ov45_02255814[];
extern char ov45_02255844[];
extern char ov45_0225588C[];
extern char ov45_022558A4[];
extern char ov45_022558BC[];
extern char ov45_022558D4[];
extern char ov45_022558F0[];
extern char ov45_0225590C[];
extern char ov45_02255918[];
extern char ov45_02255928[];
extern char ov45_02255938[];
extern char ov45_02255948[];
extern char ov45_02255958[];
extern char ov45_02255968[];
extern char ov45_02255978[];
extern char ov45_02255988[];
extern char ov45_02255998[];
extern char ov45_022559A8[];
extern char ov45_022559B8[];
extern char ov45_022559C8[];
extern char ov45_022559D4[];
extern char ov45_022559E4[];
extern char ov45_022559F8[];
extern char ov45_02255A2C[];
extern char ov45_02255A30[];
extern char ov45_02255AA8[];
extern char ov45_02255B44[];
extern char ov45_02255B54[];
extern char ov45_02255B64[];
extern char ov45_02255B7C[];
extern char ov45_02255B94[];
extern char ov45_02255BA8[];
extern char ov45_02255BC4[];
int ov45_022474FC(void *c, int n);
int ov45_02247518(char *s, int n);
void *ov45_022475B8(void *c, char *s);
int ov45_022475F0(const void *s, int n);
int ov45_022477B8(const void *s, int n);
void ov45_02247770(void *c);
void ov45_02247934(void *c);
int ov45_02247968(void *c);
void ov45_022479F0(void *c);
void ov45_02247A44(void *c, char *s);
int ov45_02247AD4(const void *a, const void *b);
int ov45_02247AE0(void *c, char *s);
void ov45_02247D84(void *c, char *s);
void *ov45_02247E98(void *c, char *s);
int ov45_02248238(void *c, char *s);
int ov45_02248288(void *c, char *s, void *out);
void ov45_02248374(void *c, char *s, void *data);
void ov45_02248454(void *c, char *s, char *data);
void ov45_02248558(void *c, char *s);
int ov45_0224861C(void *c, char *s);
void ov45_022486E0(void *c, char *s, char *data);
int ov45_022487BC(void *c, char *s);
int ov45_02247650(const void *a, const void *b);
int ov45_02247818(const void *a, const void *b);
void ov45_02248A98(void *c, char *s, char *name);
void ov45_02248BCC(void *c, void *t);
void ov45_02248CE8(void *c, char *s, void *cb, void *arg);
int ov45_0224739C(void *c, int id);

void *ov45_022475B8(void *c, char *s) {
    char buf[484];
    strncpy(buf, s, 257);
    buf[256] = 0;
    return ov00_021FF0C0(P(c, 0x80c), buf);
}

int ov45_022475F0(const void *input, int n) {
    char *s = (char *)input;
    if (!s) {
        __msl_assertion_failed(ov45_02255938, ov45_02255918, ov45_022557FC, 0xb4);
    }
    if (n <= 0) {
        __msl_assertion_failed(ov45_02255948, ov45_02255918, ov45_022557FC, 0xb5);
    }
    return ov45_02247518(s, n);
}

int ov45_02247650(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    if (!a) {
        __msl_assertion_failed(ov45_02255958, ov45_02255918, ov45_022558BC, 0xc1);
    }
    if (!b) {
        __msl_assertion_failed(ov45_02255968, ov45_02255918, ov45_022558BC, 0xc2);
    }
    if (!a) {
        __msl_assertion_failed(ov45_02255978, ov45_02255918, ov45_022558BC, 0xc6);
    }
    if (!b) {
        __msl_assertion_failed(ov45_02255988, ov45_02255918, ov45_022558BC, 0xc7);
    }
    if (!a) {
        __msl_assertion_failed(ov45_02255978, ov45_02255918, ov45_022558BC, 0xc8);
    }
    if (!*a) {
        __msl_assertion_failed(ov45_02255998, ov45_02255918, ov45_022558BC, 0xc8);
    }
    if (!b) {
        __msl_assertion_failed(ov45_02255988, ov45_02255918, ov45_022558BC, 0xc9);
    }
    if (!*b) {
        __msl_assertion_failed(ov45_022559A8, ov45_02255918, ov45_022558BC, 0xc9);
    }
    return strnicmp(a, b);
}

void ov45_02247770(void *c) {
    if (!c) {
        __msl_assertion_failed(ov45_02255938, ov45_02255918, ov45_022558F0, 0xd2);
    }
    ov00_021EC3D8(P(c, 0x15c));
    if (P(c, 0x134)) {
        ov00_021FEE8C(P(c, 0x134));
    }
}

int ov45_022477B8(const void *input, int n) {
    char *s = (char *)input;
    if (!s) {
        __msl_assertion_failed(ov45_02255938, ov45_02255918, ov45_02255714, 0xe1);
    }
    if (n <= 0) {
        __msl_assertion_failed(ov45_02255948, ov45_02255918, ov45_02255714, 0xe2);
    }
    return ov45_02247518(s, n);
}

int ov45_02247818(const void *first, const void *second) {
    char *a = (char *)first, *b = (char *)second;
    if (!a) {
        __msl_assertion_failed(ov45_02255958, ov45_02255918, ov45_022557B4, 0xee);
    }
    if (!b) {
        __msl_assertion_failed(ov45_02255968, ov45_02255918, ov45_022557B4, 0xef);
    }
    if (!a || !b) {
        if (!a && !b) {
            return 0;
        }
        return !a ? -1 : 1;
    }
    if (!a) {
        __msl_assertion_failed(ov45_02255978, ov45_02255918, ov45_022557B4, 0x102);
    }
    if (!*a) {
        __msl_assertion_failed(ov45_02255998, ov45_02255918, ov45_022557B4, 0x102);
    }
    if (!b) {
        __msl_assertion_failed(ov45_02255988, ov45_02255918, ov45_022557B4, 0x103);
    }
    if (!*b) {
        __msl_assertion_failed(ov45_022559A8, ov45_02255918, ov45_022557B4, 0x103);
    }
    return strnicmp(a, b);
}

void ov45_02247934(void *c) {
    if (!c) {
        __msl_assertion_failed(ov45_02255938, ov45_02255918, ov45_022558D4, 0x10a);
    }
}

int ov45_02247968(void *c) {
    P(c, 0x80c) = ov00_021FED4C(484, 7, 2, ov45_022475F0, ov45_02247650, ov45_02247770);
    if (!P(c, 0x80c)) {
        return 0;
    }
    P(c, 0x810) = ov00_021FE5B4(484, 0, 0);
    if (P(c, 0x810)) {
        return 1;
    }
    ov00_021FEE8C(P(c, 0x80c));
    return 0;
}

void ov45_022479F0(void *c) {
    if (!c) {
        __msl_assertion_failed(ov45_022559B8, ov45_02255918, ov45_02255700, 0x127);
    }
    if (P(c, 0x80c)) {
        ov00_021FEE8C(P(c, 0x80c));
    }
    if (P(c, 0x810)) {
        ov00_021FE678(P(c, 0x810));
    }
}

void ov45_02247A44(void *c, char *s) {
    if (!c) {
        __msl_assertion_failed(ov45_022559B8, ov45_02255918, ov45_0225573C, 0x134);
    }

    struct {
        u32 words[121];
    } buf = { 0 };

    strncpy((char *)&buf, s, 257);
    ((char *)&buf)[256] = 0;
    ov00_021FE754(P(c, 0x810), &buf);
}

int ov45_02247AD4(const void *a, const void *b) {
    return strnicmp((const char *)a, (const char *)b);
}

int ov45_02247AE0(void *c, char *s) {
    int i, n;
    void *p;
    if (!c) {
        __msl_assertion_failed(ov45_022559B8, ov45_02255918, ov45_022557A0, 0x14d);
    }
    n = ArrayLength(P(c, 0x810));
    for (i = 0; i < n; i++) {
        p = ov00_021FE6EC(P(c, 0x810), i);
        if (!p) {
            __msl_assertion_failed(ov45_022559C8, ov45_02255918, ov45_022557A0, 0x153);
        }
        if (!strnicmp(p, s)) {
            return 1;
        }
    }
    return 0;
}
