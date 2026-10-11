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

void find_session_from_IP(void *s, u32 ip, int port) {
    OSIntrMode mode = OS_DisableInterrupts();
    int i;
    u8 *p = _021D5E68;
    B(s, 0x30) = 0;
    for (i = 0; i < 4; i++, p += 0x5c) {
        if (p[0x5a] && W(p, 0x54) == ip && H(p, 0x58) == port) {
            MI_CpuCopy8(p, A(s, 0x74), 0x20);
            MI_CpuCopy8(p + 0x20, s, 0x30);
            W(p, 0x50) = OS_GetTick() >> 16;
            B(s, 0x30) = 1;
            break;
        }
    }
    OS_RestoreInterrupts(mode);
}
