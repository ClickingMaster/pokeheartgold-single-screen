#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define B(p, n)  (((u8 *)(p))[n])
#define VW(p, n) (*(volatile u32 *)((u8 *)(p) + (n)))
#define W(p, n)  (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n)  (*(void **)((u8 *)(p) + (n)))
extern void (*gCpsFree)(void *);
extern u8 _021D5E68[];
extern u8 _version_UBIQUITOUS_SSL[];
extern u8 _021D5FD8[];
extern u8 _021D5E60[];
extern u8 gCpsMd5Padding[];
extern u8 gCpsSha1Padding[];
extern u8 _02110899[];
void OSi_ReferSymbol(void *);
void *CPSi_TcpReadRaw(u32 *, void *);
void CPSi_SocConsumeRaw(u32, void *);
int parse_record(void *);
int ssl_connect_try(void *);
int CPSi_TcpConnectRaw(void *);
void try_fill_record(void *);
void CPSi_md5_block(void *, void *);
void CPSi_sha1_block(void *, void *);

int tcp_read_raw_nbytes(u8 *a, int b, void *c);
void update_digest(u8 *a, void *b, int c);
void CPS_SslAddRandomSeed(void *a, int b);
int mustget_change_cipher_spec_and_finished(void *a);
int CPSi_SslConnect(void *a);
void CPSi_SslConsume(u32 a, void *b);
int CPSi_SslGetLength(void *a);
void CPSi_SslClose(void *a);
void CPS_SetSsl(int a);
void CPSi_SslCleanup(void);
void char_from_long(void *a, void *b, int c);
void long_from_char(void *a, void *b, int c);
u32 R(u32 a, u32 *b);
void char_from_long__sha1(u8 *a, u32 *b, u32 c);
void long_from_char__sha1(u32 *a, u8 *b, u32 c);
void CPSi_md5_init(u32 *a);
void CPSi_sha1_init(u32 *a);
void CPSi_md5_calc(u8 *a, u8 *b, u32 c);
void CPSi_sha1_calc(u8 *a, u8 *b, u32 c);
void CPSi_md5_result(u8 *a, u8 *b);
void CPSi_sha1_result(u8 *a, u8 *b);
void CPSi_sha1_result_prng(void *a, void *b);
void CPSi_rc4_init(u8 *a, u8 *b, int c);
void CPSi_rc4_crypt(u8 *a, u8 *b, int c);
int count_digits(u16 *a, int b);
int CPSi_big_sign(u16 *a, int b);
void CPSi_big_add(u16 *a, u16 *b, u16 *c, int d);
void CPSi_big_add_small(u16 *a, u16 *b, u32 c, int d);
void CPSi_big_negate(u16 *a, int b);
void CPSi_big_sub(u16 *a, u16 *b, u16 *c, int d);
void CPSi_big_sub_small(u16 *a, u16 *b, u32 c, int d);
int CPSi_big_compare(u16 *a, u16 *b, int c);
void CPSi_big_add_part(u16 *a, u32 b, int c, int d);
void CPSi_big_mult(u16 *a, u16 *b, u16 *c, int d);
void CPSi_big_mult_small(u16 *a, u16 *b, u16 c, int d);
u64 get48bits_1(u16 *a);
u64 get48bits_2(u16 *a);
u64 get48bits_3(u16 *a);
u64 get64bits(u16 *a);

#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
void CPSi_TcpListenRaw(void *);
void CPSi_TcpShutdownRaw(void *);
int send_server_hello(void *);
void send_change_cipher_spec_and_finished(void *);
void send_client_hello(void *);
void send_client_key_exchange(void *);
void create_key_block(void *);
void create_master_secret(void *);
void cache_session(void *, u32, u16);
void parse_record_in_buf(void *, void *);
int make_ciphertext(void *, void *);
int CPSi_TcpWrite2Raw(void *, u32, void *, u32, void *);
int ssl_listen_try(void *a);
int ssl_connect_try(void *a);
void *CPSi_SslRead(u32 *a, void *b);
void CPSi_SslShutdown(void *a);
void CPSi_SslPeriodical(int a);
void CPSi_big_montmult(u16 *a, u16 *b, u16 *c, int d, int e, u16 *f, u16 *g, u16 *h, u16 *j);
void CPSi_big_sqr(u16 *a, u16 *b, int c);

struct Md {
    u32 state[4], lo, hi;
    u8 buf[64];
};

struct Sh {
    u32 state[5], hi, lo;
    u8 buf[64];
};

void CPSi_SslListen(void *);

void CPSi_SslShutdown(void *a) {
    void *p = P(a, 12);
    u8 packet[28];
    if (B(p, 0x455) == 8) {
        int len;
        packet[0] = 21;
        packet[1] = 3;
        packet[2] = 0;
        packet[3] = 0;
        packet[4] = 2;
        packet[5] = 1;
        packet[6] = 0;
        len = make_ciphertext(p, packet);
        CPSi_TcpWrite2Raw(packet, len, 0, 0, a);
    }
    B(p, 0x455) = 0;
}

void CPSi_SslClose(void *a) {
    void *p = P(a, 12);
    B(p, 0x455) = 0;
    if (P(p, 0x824)) {
        gCpsFree(P(p, 0x824));
    }
    P(p, 0x824) = 0;
}

void CPS_SetSsl(int a) {
    void *p;
    OSi_ReferSymbol(_version_UBIQUITOUS_SSL);
    p = P(OSi_ThreadInfo.current, 0xa4);
    if (p) {
        B(p, 9) = a;
    }
}

void CPSi_SslPeriodical(int a) {
    OSIntrMode m = OS_DisableInterrupts();
    int i;
    u8 *p = _021D5E68;
    OSThread *t;
    for (i = 0; i < 4; i++, p += 0x5c) {
        if (B(p, 0x5a) && (int)(a - W(p, 0x50)) > 957) {
            B(p, 0x5a) = 0;
        }
    }
    OS_RestoreInterrupts(m);
    for (t = OSi_ThreadInfo.list; t; t = t->next) {
        p = P(t, 0xa4);
        if (p && P(p, 0) && B(p, 9) && B(p, 8) == 4 && B(P(p, 12), 0x455) < 8 && (int)(a - W(p, 16)) > 239 && W(p, 4) == 2) {
            B(p, 8) = 0;
            W(p, 4) = 0;
            OS_WakeupThreadDirect(P(p, 0));
        }
    }
}

void CPSi_SslCleanup(void) {
    MI_CpuFill8(_021D5E68, 0, 0x170);
}

void char_from_long(void *a, void *b, int c) {
    MI_CpuCopy8(b, a, c);
}

void long_from_char(void *a, void *b, int c) {
    MI_CpuCopy8(b, a, c);
}
