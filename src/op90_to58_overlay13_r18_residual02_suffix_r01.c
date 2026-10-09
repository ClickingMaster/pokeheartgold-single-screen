#pragma force_active on
#include "global.h"
#include <string.h>

typedef struct Addr {
    u8 len;
    u8 family;
    u16 port;
    u32 addr;
} Addr;

typedef struct Work {
    u8 flag;
    u8 pad[15];
    int abort;
    int length;
    int pad18;
    int state;
    u8 p20[0x14];
    u32 value;
    int phase;
    u8 tail[0x14];
} Work;

typedef struct Deadline {
    u8 p[12];
    u32 deadline;
} Deadline;

typedef struct Flag270 {
    u8 p[20];
    s8 flag;
    u8 tail[43];
} Flag270;

typedef struct ResultStorage {
    u32 words[149];
} ResultStorage;

extern Work ov13_0224DF30;
extern u8 ov13_0224DF10[];
extern Deadline ov13_02245A58;
extern u8 ov13_0224E3F0[16];
extern u8 ov13_0224EC64[2048];
extern u8 ov13_0224DF80[];
extern u8 ov13_0224E464[2048];
extern u8 ov13_0224E400[8];
extern u8 ov13_0224E408[24];
extern ResultStorage ov13_0224E184;
extern Flag270 ov13_0224E270;

void OS_Sleep(u32 msec);
int SOC_Socket(int domain, int type, int protocol);
int SOC_Bind(int socket, void *addr);
int SOC_Close(int socket);
int SOC_RecvFrom(int socket, void *buf, int len, int flags, void *from);
int ov13_02224330(void);
int ov13_02223FDC(void);
int ov13_02223DE0(void);
int ov13_02224DB4(void);
u32 ov13_02226CBC(void);
void ov13_02226F3C(void);
int ov13_02224A30(u8 *pair, Addr *from);
int ov13_02224864(void *buf, int *out);
u32 ov13_02224988(void *buf);
int ov13_02224618(int socket, Addr *to, const void *buf, int len);
int ov13_02224938(u8 *buf, u32 type, void *out, const u8 *key);
u8 *ov13_022246E4(u8 *buf, u32 *type, u32 *size);
void ov13_02226C94(u8 *dst, const u8 *src, u32 len);
u32 ov13_0222483C(u8 *buf, u16 type, const void *data, u32 len);
u32 ov13_02224718(u8 *out, u32 type, u8 *buf, u32 len, const u8 *key);
int ov13_02224B2C(void *buf);
int ov13_02224F3C(void);

static inline int BindSocket(int socket, Addr *address) {
    memset(address, 0, sizeof(*address));
    address->len = 8;
    address->family = 2;
    address->port = 486;
    address->addr = 0;
    return SOC_Bind(socket, address);
}

typedef struct Id8 { u8 b[8]; } Id8;
static inline void CopyIdentity(const u8 *src, u8 *dst) {
    u32 k = 8;
    do {
        *dst++ = *src++;
    } while (--k);
}

#pragma opt_dead_assignments off
// @fn ov13_02224F3C
int ov13_02224F3C(void) {
    Addr bind, recv;
    u32 type, size;
    u32 timestamp;
    u8 *p;
    int sock;
    int done;
    int result;
    u32 last;
    int retry;

    sock = 0;
    result = -5;
    last = 0;
    retry = 0;
    done = 0;

    ov13_0224DF30.state = 1;
    while (!done && !ov13_0224DF30.abort) {
        OS_Sleep(500);
        switch (ov13_0224DF30.state) {
        case 1:
            result = ov13_02224330();
            if (result != 1) {
                done = 1;
                break;
            }
            ov13_0224DF30.phase = 3;
            ov13_02226F3C();
            ov13_0224DF30.state = 2;
            break;
        case 2:
            result = ov13_02223FDC();
            if (result != 1) {
                done = 1;
                break;
            }
            ov13_0224DF30.state = 3;
            break;
        case 3:
            sock = SOC_Socket(2, 2, 0);
            if (sock < 0) {
                result = -2;
                done = 1;
                break;
            }
            result = BindSocket(sock, &bind);
            if (result < 0) {
                result = -2;
                done = 1;
                break;
            }
            ov13_0224DF30.state = 4;
            break;
        case 4:
            if (ov13_02226CBC() >= ov13_02245A58.deadline) {
                SOC_Close(sock);
                result = -3;
                done = 1;
                break;
            }
            recv.len = 8;
            ov13_02224A30(ov13_0224E3F0, &recv);
            if (SOC_RecvFrom(sock, ov13_0224EC64, 2048, 4, &recv) > 0 && ov13_02224864(ov13_0224EC64, (int *)ov13_0224DF80)) {
                ov13_02245A58.deadline = ov13_02226CBC() + 30000;
                ov13_0224DF30.state = 5;
                ov13_0224DF30.phase = 4;
                ov13_02226F3C();
            }
            break;
        case 5:
            ov13_0224DF30.length = ov13_02224988(ov13_0224EC64);
            ov13_02224618(sock, &recv, ov13_0224EC64, ov13_0224DF30.length);
            last = ov13_02226CBC();
            ov13_0224DF30.state = 6;
            break;
        case 6:
            if (ov13_02226CBC() >= ov13_02245A58.deadline) {
                SOC_Close(sock);
                result = -4;
                done = 1;
                break;
            }
            if (SOC_RecvFrom(sock, ov13_0224EC64, 2048, 4, &recv) > 0 && ov13_02224938(ov13_0224EC64, 3, ov13_0224E464, ov13_0224E3F0)) {
                p = ov13_022246E4(ov13_0224E464, &type, &size);
                if (type == 257) {
                    timestamp = ov13_02226CBC();
                    *(Id8 *)ov13_0224E400 = *(Id8 *)p;
                    ov13_02226C94(ov13_0224E408, (const u8 *)&timestamp, 4);
                    retry = 0;
                    ov13_0224DF30.state = 7;
                    ov13_0224DF30.phase = 5;
                    ov13_02245A58.deadline = -1;
                    ov13_02226F3C();
                }
                p = NULL;
                break;
            }
            if (ov13_02226CBC() >= last + 1000) {
                ov13_0224DF30.state = 5;
            }
            break;
        case 7:
            ov13_0224DF30.value = ov13_0222483C(ov13_0224E464, 258, ov13_0224E408, 8);
            ov13_0224DF30.length = ov13_02224718(ov13_0224EC64, 4, ov13_0224E464, ov13_0224DF30.value, ov13_0224E3F0);
            ov13_02224618(sock, &recv, ov13_0224EC64, ov13_0224DF30.length);
            last = ov13_02226CBC();
            memset(&ov13_0224E184, 0, sizeof(ov13_0224E184));
            ov13_0224DF30.state = 8;
            break;
        case 8:
            if (SOC_RecvFrom(sock, ov13_0224EC64, 2048, 4, &recv) > 0) {
                ov13_0224DF30.value = ov13_02224938(ov13_0224EC64, 5, ov13_0224E464, ov13_0224E400);
                if (ov13_0224DF30.value && ov13_02224B2C(ov13_0224E464)) {
                    if (ov13_0224E270.flag) {
                        ov13_0224DF30.flag = 1;
                    } else {
                        ov13_0224DF30.flag = 0;
                    }
                    retry = 0;
                    ov13_0224DF30.state = 9;
                    break;
                }
            }
            if (ov13_02226CBC() >= last + 1000) {
                retry++;
                if (retry >= 10) {
                    SOC_Close(sock);
                    result = -2;
                    done = 1;
                } else {
                    ov13_0224DF30.state = 7;
                }
            }
            break;
        case 9:
            ov13_0224DF30.value = ov13_0222483C(ov13_0224E464, 769, ov13_0224DF10 + 0x20, 1);
            ov13_0224DF30.length = ov13_02224718(ov13_0224EC64, 6, ov13_0224E464, ov13_0224DF30.value, ov13_0224E400);
            if (ov13_02223DE0() != 7) {
                last = ov13_02226CBC() + 1000;
                retry = 10;
                ov13_0224DF30.state = 10;
                break;
            }
            ov13_02224618(sock, &recv, ov13_0224EC64, ov13_0224DF30.length);
            last = ov13_02226CBC();
            ov13_0224DF30.state = 10;
            break;
        case 10:
            if (ov13_02226CBC() >= last + 1000) {
                retry++;
                if (retry >= 10) {
                    done = 1;
                    result = ov13_02224DB4();
                } else {
                    ov13_0224DF30.state = 9;
                }
            }
            break;
        }
    }
    if (sock) {
        SOC_Close(sock);
    }
    if (ov13_0224DF30.abort) {
        result = -8;
    }
    return result;
}
#pragma opt_dead_assignments reset
