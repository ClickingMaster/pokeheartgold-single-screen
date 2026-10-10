#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
#pragma force_active on
#define W(n) (*(int *)(_0222AB84 + (n)))
extern u8 _0222AB84[];
extern u8 ov39_0222AD84[];
extern u8 ov39_0222B6D0[];
extern u8 ov39_0222AA7C[];
extern u8 ov39_0222AA78[];
extern char ov39_0222ACC4[];
extern char ov39_0222AA5C[];
extern OSMutex ov39_0222AEB8;
extern OSThread ov39_0222ADF8;

typedef struct {
    u8 bytes[4];
} B4;

typedef union {
    int n;
    u8 bytes[4];
} U4;

typedef struct {
    u32 x[78];
} Z312;

typedef struct {
    B4 size;
    u16 cmd, pad;
    Z312 config;
} Packet;

typedef struct {
    int x[12];
} B48;

typedef struct {
    int type, len;
    void *p;
    u32 **addr;
} Host;

typedef struct {
    u8 len, family;
    u16 port;
    u32 addr;
} Addr;

extern u8 _0222A9AC[];
extern Host *SOC_GetHostByName(char *);

typedef struct {
    u32 addr;
} InAddr;

extern char *SOC_InetNtoA(InAddr);
extern int SOC_Connect(int, Addr *);
extern int SOC_Send(int, void *, int, int);
extern int SOC_Recv(int, void *, int, int);
extern int ov39_02229FCC(void);
extern int ov39_02229E40(void);

typedef struct {
    u16 a, b;
    int n, h;
} I12;

typedef struct {
    u32 x[4];
} B16;

typedef struct {
    u32 x[56];
} B224;

typedef struct {
    u32 x[135];
} B540;

typedef struct {
    int n;
    B540 b;
} P544;

typedef struct {
    u32 x[9];
} B36;

typedef struct {
    u32 x[1875];
} B7500;

typedef struct {
    u16 a, c;
    B16 b;
    int tail[2];
} P28;

typedef struct {
    u16 a, c;
    int data[6];
} Q28;

extern void *ov00_021EC2A8(int, int);
extern void ov00_021EC2EC(int, void *, int);
extern int ov39_02229630(int, void *, void *, int);
extern int ov39_0222A6DC(int);
extern void ov39_02229474(void);
extern int ov39_02229A6C(char *, char *);
extern int ov00_021FFA44(char *);
extern int SOC_Socket(int, int, int);
extern int SOC_Fcntl(int, int, int);
extern int SOC_Shutdown(int, int);
extern int SOC_Close(int);
void ov39_02229310(void);
int ov39_02229374(char *p, int n, Z312 *q);
void ov39_02229408(void);
int ov39_0222981C(void);
int ov39_0222982C(void);
int ov39_02229864(void);
int ov39_02229898(void);
void *ov39_022298CC(int n);
void ov39_022298E0(void *p);
void ov39_022298FC(void);
void ov39_02229910(void);
int ov39_02229924(void);
int ov39_0222993C(void);
int ov39_02229944(void);
void ov39_02229970(void);
void ov39_02229974(void);
int ov39_02229980(void);
int ov39_022299D4(signed char *p);
int ov39_02229AD4(void);
void ov39_02229BA4(void);
void ov39_02229FC0(u32 n);
int ov39_0222A110(Z312 *p);
void ov39_0222A130(void);
int ov39_0222A13C(void);
int ov39_0222A158(void);
int ov39_0222A164(B224 *p, void *out);
int ov39_0222A1C0(int n, void *out);
int ov39_0222A268(int n, void *out);
int ov39_0222A200(int n, B540 *p, void *out);
int ov39_0222A2A8(void);
int ov39_0222A2B4(void);
int ov39_0222A2C0(int n);
int ov39_0222A2CC(void *p);
int ov39_0222A2EC(B36 *p, void *out);
int ov39_0222A33C(B7500 *p, void *out);
int ov39_0222A394(B16 *p, void *out);
int ov39_0222A3DC(int *p, void *out);
int ov39_0222A434(int *p, void *out);
int ov39_0222A48C(int a, int b, int c, void *out);
int ov39_0222A4C0(int a, int b, void *out);
void ov39_0222A4EC(u8 *p, int n, u8 *out);
void ov39_0222A544(u8 *p, int n, u8 *out);
int ov39_0222A59C(int n);
void ov39_0222A8AC(void *p, void *q);
int ov39_02229A6C(char *p, char *out);
int ov39_02229BE4(void);
int ov39_02229D2C(void);
void ov39_02229474(void);
int ov39_0222A6DC(int n);
int ov39_02229E40(void);
int ov39_02229630(int cmd, void *in, void *out, int n);

int ov39_0222A110(Z312 *p) {
    return ov39_02229374(ov39_0222AA5C, 0x3070, p);
}

void ov39_0222A130(void) {
    ov39_02229310();
}

int ov39_0222A13C(void) {
    int x;
    ov39_022298FC();
    x = ov39_0222981C();
    ov39_02229910();
    return x;
}

int ov39_0222A158(void) {
    return ov39_02229898();
}

int ov39_0222A164(B224 *p, void *out) {
    B224 v = *p;
    ov39_0222A8AC(p, ov39_0222AA78);
    return ov39_02229630(0x4e20, &v, out, -1);
}

int ov39_0222A1C0(int n, void *out) {
    I12 v;
    v.a = 0;
    v.b = 0;
    v.n = n;
    v.h = n >> 31;
    return ov39_02229630(20001, &v, out, -1);
}

int ov39_0222A200(int n, B540 *p, void *out) {
    P544 v;
    v.n = n;
    v.b = *p;
    ov39_0222A8AC(p, ov39_0222AA78);
    return ov39_02229630(0x5208, &v, out, -1);
}

int ov39_0222A268(int n, void *out) {
    I12 v;
    v.a = 0;
    v.b = 0;
    v.n = n;
    v.h = n >> 31;
    return ov39_02229630(21001, &v, out, -1);
}

int ov39_0222A2A8(void) {
    return ov39_02229864();
}

int ov39_0222A2B4(void) {
    return ov39_0222982C();
}

int ov39_0222A2C0(int n) {
    return ov39_0222A6DC(n);
}

int ov39_0222A2CC(void *p) {
    return ov39_02229630(0x55f0, 0, p, -1);
}

int ov39_0222A2EC(B36 *p, void *out) {
    B36 v = *p;
    return ov39_02229630(22001, &v, out, -1);
}

int ov39_0222A33C(B7500 *p, void *out) {
    B7500 v = *p;
    return ov39_02229630(23000, &v, out, -1);
}

int ov39_0222A394(B16 *p, void *out) {
    P28 v;
    v.a = 0;
    v.b = *p;
    v.a = 0;
    v.c = 0;
    return ov39_02229630(0x59d9, &v, out, -1);
}

int ov39_0222A3DC(int *p, void *out) {
    Q28 v = { 0 };
    v.a = 1;
    v.data[2] = *p;
    return ov39_02229630(0x59d9, &v, out, -1);
}

int ov39_0222A434(int *p, void *out) {
    Q28 v = { 0 };
    v.a = 2;
    v.data[2] = *p;
    return ov39_02229630(0x59d9, &v, out, -1);
}

int ov39_0222A48C(int a, int b, int c, void *out) {
    int v[3];
    v[0] = a;
    v[1] = b;
    v[2] = c;
    return ov39_02229630(0x59da, v, out, -1);
}

int ov39_0222A4C0(int a, int b, void *out) {
    int v[2];
    v[0] = a;
    v[1] = b;
    return ov39_02229630(0x59db, v, out, -1);
}
