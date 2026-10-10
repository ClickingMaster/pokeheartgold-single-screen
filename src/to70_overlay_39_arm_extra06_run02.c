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

int ov39_0222981C(void) {
    return W(0x13c);
}

int ov39_0222982C(void) {
    ov39_022298FC();
    if (ov39_0222981C() != 8) {
        ov39_02229910();
        return 0;
    }
    ov39_02229910();
    return W(608) + 4;
}

int ov39_02229864(void) {
    ov39_022298FC();
    if (ov39_0222981C() != 8) {
        ov39_02229910();
        return -1;
    }
    ov39_02229910();
    return W(604);
}

int ov39_02229898(void) {
    ov39_022298FC();
    if (ov39_0222981C() != 9) {
        ov39_02229910();
        return 0;
    }
    ov39_02229910();
    return W(592);
}

void *ov39_022298CC(int n) {
    return ov00_021EC2A8(0, n);
}

void ov39_022298E0(void *p) {
    if (p) {
        ov00_021EC2EC(0, p, 0);
    }
}

void ov39_022298FC(void) {
    OS_LockMutex(&ov39_0222AEB8);
}

void ov39_02229910(void) {
    OS_UnlockMutex(&ov39_0222AEB8);
}

int ov39_02229924(void) {
    OS_InitMutex(&ov39_0222AEB8);
    return 1;
}

int ov39_0222993C(void) {
    return 1;
}

int ov39_02229944(void) {
    if (!OS_IsTickAvailable()) {
        OS_InitTick();
    }
    W(0x270) = 16;
    return 1;
}

void ov39_02229970(void) {
}

void ov39_02229974(void) {
    ov39_02229474();
}

int ov39_02229980(void) {
    OS_CreateThread(&ov39_0222ADF8, (void (*)(void *))ov39_02229974, 0, ov39_0222B6D0, 0x800, W(0x270));
    OS_WakeupThreadDirect(&ov39_0222ADF8);
    return 1;
}

int ov39_022299D4(signed char *p) {
    int digits = 0, dots = 0;
    int c;
    while ((c = *p) != 0) {
        if (c >= 48 && c <= 57) {
            if (++digits >= 4) {
                return 0;
            }
        } else if (c == 46) {
            if (!digits) {
                return 0;
            }
            digits = 0;
            if (++dots >= 4) {
                return 0;
            }
        } else {
            return 0;
        }
        p++;
    }
    if (dots == 3 && digits) {
        return 1;
    }
    return 0;
}

int ov39_02229A6C(char *p, char *out) {
    if (!ov39_022299D4((signed char *)p)) {
        Host *h = SOC_GetHostByName(p);
        if (h) {
            InAddr addr = { **h->addr };
            strcpy(out, SOC_InetNtoA(addr));
        } else {
            return 0;
        }
    } else {
        strcpy(out, p);
    }
    return 1;
}

int ov39_02229AD4(void) {
    char s[16];
    W(0x26c) = SOC_Socket(2, 1, 0);
    if (W(0x26c) == -1) {
        W(0x250) = 3;
        return 0;
    }
    int v = SOC_Fcntl(W(0x26c), 3, 0);
    if (SOC_Fcntl(W(0x26c), 4, v | 4) < 0) {
        W(0x250) = 4;
        return 0;
    }
    if (!ov39_02229A6C(ov39_0222ACC4, s)) {
        W(0x250) = 5;
        return 0;
    }
    W(0x264) = ov00_021FFA44(s);
    return 1;
}

void ov39_02229BA4(void) {
    if (W(0x26c) == -1) {
        return;
    }
    SOC_Shutdown(W(0x26c), 2);
    SOC_Close(W(0x26c));
    W(0x26c) = -1;
}
