#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <nitro/rtc.h>
#define W(p, o) (*(u32 *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define H(p, o) (*(u16 *)((char *)(p) + (o)))
#define B(p, o) (*(u8 *)((char *)(p) + (o)))
#define A(p, o) ((char *)(p) + (o))
extern RTCResult RTC_GetDate(RTCDate *);
extern RTCResult RTC_GetTime(RTCTime *);
extern u8 gCpsState[];
extern u8 gCpsDhcpPacket[];
extern u8 _021D5E68[];
extern u8 _02110708[];
extern u8 _02110710[];
extern u32 _021106B0[];
extern void CPS_SocUse(void);
extern void CPS_SocRelease(void);
extern void CPS_SocDatagramMode(void);
extern void CPS_SocBind(int, int, u32);
extern void CPS_SocWrite(void *, int);
extern u8 *dhcp_setcommon(u8 *, int, int *);
extern u8 *pad_mem(int, int, u8 *, int);
extern int dhcp_analyze_response(int, int);
extern int resolve_common(char *, int, int, void *, int);
extern void CPSi_md5_calc(void *, void *, int);
extern void CPSi_md5_result(void *, void *);
extern void CPSi_md5_init(void *);
extern void CPSi_sha1_calc(void *, void *, int);
extern void CPSi_sha1_result(void *, void *);
extern void CPSi_sha1_init(void *);
extern void CPSi_rc4_crypt(void *, void *, int);
extern void decrypt_premaster_secret(void *, void *, void *);
extern void create_master_secret(void *);
extern void create_key_block(void *);
extern u8 _021106FC[];
extern u8 _02110700[];
extern u8 _02110704[];
extern int validate_signature(void *, void *);
#pragma warn_uninitializedvar off
extern void cache_session(void *, u32, int);
int dhcp_send_discover(void);
int dhcp_discover_server(void);
void dhcp_release_server(void);
u8 *dns_skipname(u8 *p);
u32 strtol10(u8 *p, u8 **end);
int rawip(u8 *s, u32 *out);
int resolve_sub(char *s, u32 ip, int id);
void find_session_from_id(void *s);
void find_session_from_IP(void *s, u32 ip, int port);
void purge_session(void *s);
u32 date2sec(void);
u32 CPS_GetSslHandshakePriority(void);
void CPS_SetSslHandshakePriority(u32 n);
u32 enter_computebound(void);
void exit_computebound(u32 n);
void CPS_SetRootCa(void *a, int n);
void *get_rootCA(void *s, char *name);
int cert_item_len(u8 **pp);
void make_dn(char *out, char *s, int n);
u32 parse_time(u8 *p, int type);
int chars_till_end(char *p);
int compare_fqdn(char *s, char *p);
int has_method(u8 *p, int n, int width, int method);
u16 select_method(u8 *p, int n, int width);
int version_ok(int a, int b);
void rcv_client_key_exchange(void *s, void *p);
void finished_md5(void *s, void *out, int flag);
void finished_sha1(void *s, void *out, int flag);
void add1_be8(u8 *p);
int decrypt(void *s, void *p, int n);
int auth_cert(void *s);
void rcv_server_hello(void *s, u8 *p);
void client_hello_v2(void *s, u8 *p);
void client_hello(void *s, u8 *p);
void create_ms_sub(void *out, char *text, void *s);
void create_master_secret(void *s);
void rcv_finished(void *s, u8 *p);
void cache_session(void *s, u32 ip, int port);
int dhcp_send_request(int flag);
int make_plaintext(void *s, u8 *p);
int make_ciphertext(void *s, u8 *p);
int dhcp_request_server(u32 *, int);

void rcv_client_key_exchange(void *s, void *p) {
    decrypt_premaster_secret(s, p, P(s, 0x81c));
    create_master_secret(s);
    cache_session(s, 0, 0);
    create_key_block(s);
    B(s, 0x455) = 5;
}

void finished_md5(void *s, void *out, int flag) {
    u8 pad[48];
    if ((B(s, 0x454) ^ flag) != 0) {
        CPSi_md5_calc(A(s, 932), _02110708, 4);
    } else {
        CPSi_md5_calc(A(s, 932), _02110710, 4);
    }
    CPSi_md5_calc(A(s, 932), s, 48);
    MI_CpuFill8(pad, 54, 48);
    CPSi_md5_calc(A(s, 932), pad, 48);
    CPSi_md5_result(A(s, 932), out);
    CPSi_md5_init(A(s, 932));
    CPSi_md5_calc(A(s, 932), s, 48);
    MI_CpuFill8(pad, 92, 48);
    CPSi_md5_calc(A(s, 932), pad, 48);
    CPSi_md5_calc(A(s, 932), out, 16);
    CPSi_md5_result(A(s, 932), out);
}

void finished_sha1(void *s, void *out, int flag) {
    u8 pad[40];
    if ((B(s, 0x454) ^ flag) != 0) {
        CPSi_sha1_calc(A(s, 748), _02110708, 4);
    } else {
        CPSi_sha1_calc(A(s, 748), _02110710, 4);
    }
    CPSi_sha1_calc(A(s, 748), s, 48);
    MI_CpuFill8(pad, 54, 40);
    CPSi_sha1_calc(A(s, 748), pad, 40);
    CPSi_sha1_result(A(s, 748), out);
    CPSi_sha1_init(A(s, 748));
    CPSi_sha1_calc(A(s, 748), s, 48);
    MI_CpuFill8(pad, 92, 40);
    CPSi_sha1_calc(A(s, 748), pad, 40);
    CPSi_sha1_calc(A(s, 748), out, 20);
    CPSi_sha1_result(A(s, 748), out);
}

void rcv_finished(void *s, u8 *p) {
    u8 digest[20];
    MI_CpuCopy8(A(s, 0x3a4), A(s, 0x3fc), 0x58);
    finished_md5(s, digest, 1);
    MI_CpuCopy8(A(s, 0x3fc), A(s, 0x3a4), 0x58);
    if (memcmp(p, digest, 16)) {
        B(s, 0x455) = 9;
        return;
    }
    MI_CpuCopy8(A(s, 0x2ec), A(s, 0x348), 0x5c);
    finished_sha1(s, digest, 1);
    MI_CpuCopy8(A(s, 0x348), A(s, 0x2ec), 0x5c);
    if (memcmp(p + 16, digest, 20)) {
        B(s, 0x455) = 9;
    } else {
        B(s, 0x455) = 6;
    }
}

void add1_be8(u8 *p) {
    int n = 8;
    do {
        u8 b;
        --p;
        b = *p + 1;
        *p = b;
        if (b) {
            return;
        }
    } while (--n);
}

int decrypt(void *s, void *p, int n) {
    CPSi_rc4_crypt(A(s, 0x1e0), p, n);
    return n;
}

int make_plaintext(void *s, u8 *p) {
    u8 digest[20];
    u8 pad[48];
    int n = decrypt(s, p + 5, (p[3] << 8) + p[4]);
    int mac;
    switch (H(s, 0x32)) {
    case 4:
        n -= 16;
        p[3] = n >> 8;
        p[4] = n;
        CPSi_md5_init(A(s, 1020));
        CPSi_md5_calc(A(s, 1020), P(s, 468), 16);
        MI_CpuFill8(pad, 54, 48);
        CPSi_md5_calc(A(s, 1020), pad, 48);
        CPSi_md5_calc(A(s, 1020), A(s, 740), 8);
        CPSi_md5_calc(A(s, 1020), p, 1);
        CPSi_md5_calc(A(s, 1020), p + 3, n + 2);
        CPSi_md5_result(A(s, 1020), digest);
        CPSi_md5_init(A(s, 1020));
        CPSi_md5_calc(A(s, 1020), P(s, 468), 16);
        MI_CpuFill8(pad, 92, 48);
        CPSi_md5_calc(A(s, 1020), pad, 48);
        CPSi_md5_calc(A(s, 1020), digest, 16);
        CPSi_md5_result(A(s, 1020), digest);
        mac = 16;
        break;
    case 5:
        n -= 20;
        p[3] = n >> 8;
        p[4] = n;
        CPSi_sha1_init(A(s, 840));
        CPSi_sha1_calc(A(s, 840), P(s, 468), 20);
        MI_CpuFill8(pad, 54, 40);
        CPSi_sha1_calc(A(s, 840), pad, 40);
        CPSi_sha1_calc(A(s, 840), A(s, 740), 8);
        CPSi_sha1_calc(A(s, 840), p, 1);
        CPSi_sha1_calc(A(s, 840), p + 3, n + 2);
        CPSi_sha1_result(A(s, 840), digest);
        CPSi_sha1_init(A(s, 840));
        CPSi_sha1_calc(A(s, 840), P(s, 468), 20);
        MI_CpuFill8(pad, 92, 40);
        CPSi_sha1_calc(A(s, 840), pad, 40);
        CPSi_sha1_calc(A(s, 840), digest, 20);
        CPSi_sha1_result(A(s, 840), digest);
        mac = 20;
        break;
    }
    if (memcmp(p + 5 + n, digest, mac)) {
        B(s, 0x455) = 9;
    }
    add1_be8((u8 *)A(s, 0x2ec));
    return n + 5;
}

int make_ciphertext(void *s, u8 *p) {
    u8 pad[48];
    u8 *data = p + 5;
    int n = (p[3] << 8) + p[4];
    switch (H(s, 0x32)) {
    case 4:
        CPSi_md5_init(A(s, 1020));
        CPSi_md5_calc(A(s, 1020), P(s, 188), 16);
        MI_CpuFill8(pad, 54, 48);
        CPSi_md5_calc(A(s, 1020), pad, 48);
        CPSi_md5_calc(A(s, 1020), A(s, 460), 8);
        CPSi_md5_calc(A(s, 1020), p, 1);
        CPSi_md5_calc(A(s, 1020), p + 3, n + 2);
        CPSi_md5_result(A(s, 1020), data + n);
        CPSi_md5_init(A(s, 1020));
        CPSi_md5_calc(A(s, 1020), P(s, 188), 16);
        MI_CpuFill8(pad, 92, 48);
        CPSi_md5_calc(A(s, 1020), pad, 48);
        CPSi_md5_calc(A(s, 1020), data + n, 16);
        CPSi_md5_result(A(s, 1020), data + n);
        n += 16;
        break;
    case 5:
        CPSi_sha1_init(A(s, 840));
        CPSi_sha1_calc(A(s, 840), P(s, 188), 20);
        MI_CpuFill8(pad, 54, 40);
        CPSi_sha1_calc(A(s, 840), pad, 40);
        CPSi_sha1_calc(A(s, 840), A(s, 460), 8);
        CPSi_sha1_calc(A(s, 840), p, 1);
        CPSi_sha1_calc(A(s, 840), p + 3, n + 2);
        CPSi_sha1_result(A(s, 840), data + n);
        CPSi_sha1_init(A(s, 840));
        CPSi_sha1_calc(A(s, 840), P(s, 188), 20);
        MI_CpuFill8(pad, 92, 40);
        CPSi_sha1_calc(A(s, 840), pad, 40);
        CPSi_sha1_calc(A(s, 840), data + n, 20);
        CPSi_sha1_result(A(s, 840), data + n);
        n += 20;
        break;
    }
    p[3] = n >> 8;
    p[4] = n;
    CPSi_rc4_crypt(A(s, 0xc8), p + 5, n);
    add1_be8((u8 *)A(s, 0x1d4));
    return n + 5;
}
