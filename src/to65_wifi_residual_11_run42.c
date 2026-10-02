#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define B(p,n) (((u8 *)(p))[n])
#define VW(p,n) (*(volatile u32 *)((u8 *)(p)+(n)))
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
extern void (*_021D4660)(void *);
extern u8 _021D5E68[],_version_UBIQUITOUS_SSL[],_021D5FD8[],_021D5E60[],_02110758[],_02110898[],_02110899[];
void OSi_ReferSymbol(void *);
void *CPSi_TcpReadRaw(u32 *,void *);void CPSi_SocConsumeRaw(u32,void *);
int parse_record(void *);int ssl_connect_try(void *);int CPSi_TcpConnectRaw(void *);void try_fill_record(void *);
void CPSi_md5_block(void *,void *);void CPSi_sha1_block(void *,void *);

int tcp_read_raw_nbytes(u8 *a,int b,void *c);
void update_digest(u8 *a,void *b,int c);
void CPS_SslAddRandomSeed(void *a,int b);
int mustget_change_cipher_spec_and_finished(void *a);
int CPSi_SslConnect(void *a);
void CPSi_SslConsume(u32 a,void *b);
int CPSi_SslGetLength(void *a);
void CPSi_SslClose(void *a);
void CPS_SetSsl(int a);
void CPSi_SslCleanup(void);
void char_from_long(void *a,void *b,int c);
void long_from_char(void *a,void *b,int c);
u32 R(u32 a,u32 *b);
void char_from_long__sha1(u8 *a,u32 *b,u32 c);
void long_from_char__sha1(u32 *a,u8 *b,u32 c);
void CPSi_md5_init(u32 *a);
void CPSi_sha1_init(u32 *a);
void CPSi_md5_calc(u8 *a,u8 *b,u32 c);
void CPSi_sha1_calc(u8 *a,u8 *b,u32 c);
void CPSi_md5_result(u8 *a,u8 *b);
void CPSi_sha1_result(u8 *a,u8 *b);
void CPSi_sha1_result_prng(void *a,void *b);
void CPSi_rc4_init(u8 *a,u8 *b,int c);
void CPSi_rc4_crypt(u8 *a,u8 *b,int c);
int count_digits(u16 *a,int b);
int CPSi_big_sign(u16 *a,int b);
void CPSi_big_add(u16 *a,u16 *b,u16 *c,int d);
void CPSi_big_add_small(u16 *a,u16 *b,u32 c,int d);
void CPSi_big_negate(u16 *a,int b);
void CPSi_big_sub(u16 *a,u16 *b,u16 *c,int d);
void CPSi_big_sub_small(u16 *a,u16 *b,u32 c,int d);
int CPSi_big_compare(u16 *a,u16 *b,int c);
void CPSi_big_add_part(u16 *a,u32 b,int c,int d);
void CPSi_big_mult(u16 *a,u16 *b,u16 *c,int d);
void CPSi_big_mult_small(u16 *a,u16 *b,u16 c,int d);
u64 get48bits_1(u16 *a);
u64 get48bits_2(u16 *a);
u64 get48bits_3(u16 *a);
u64 get64bits(u16 *a);

#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
void CPSi_TcpListenRaw(void *);void CPSi_TcpShutdownRaw(void *);
int send_server_hello(void *);void send_change_cipher_spec_and_finished(void *);void send_client_hello(void *);void send_client_key_exchange(void *);
void create_key_block(void *);void create_master_secret(void *);void cache_session(void *,u32,u16);void parse_record_in_buf(void *,void *);
int make_ciphertext(void *,void *);int CPSi_TcpWrite2Raw(void *,u32,void *,u32,void *);
int ssl_listen_try(void *a);
int ssl_connect_try(void *a);
void * CPSi_SslRead(u32 *a,void *b);
void CPSi_SslShutdown(void *a);
void CPSi_SslPeriodical(int a);
void CPSi_big_montmult(u16 *a,u16 *b,u16 *c,int d,int e,u16 *f,u16 *g,u16 *h,u16 *j);
void CPSi_big_sqr(u16 *a,u16 *b,int c);
struct Md {u32 state[4],lo,hi;u8 buf[64];};struct Sh {u32 state[5],hi,lo;u8 buf[64];};
void CPSi_SslListen(void *);
void CPSi_big_sub_small(u16 *a,u16 *b,u32 c,int d) { int i;for(i=0;i<d;i++){u32 v=b[i]-c;a[i]=v;c=(v>>16)&1;if(!c)break;}if(a!=b)for(i++;i<d;i++)a[i]=b[i]; }


int CPSi_big_compare(u16 *a,u16 *b,int c) { int i;for(i=c-1;i>=0;i--){if(a[i]>b[i])return 1;if(a[i]<b[i])return -1;}return 0; }


void CPSi_big_add_part(u16 *a,u32 b,int c,int d) { while(b&&c<d){b+=a[c];a[c++]=b;b>>=16;} }


void CPSi_big_mult(u16 *a,u16 *b,u16 *c,int d) { int n,m,i,j;MI_CpuFill8(a,0,d*2);n=count_digits(b,d);m=count_digits(c,d);for(i=0;i<m;i++)for(j=0;j<n&&j<d-i;j++)CPSi_big_add_part(a,(u32)b[j]*c[i],i+j,d); }


void CPSi_big_mult_small(u16 *a,u16 *b,u16 c,int d) { int n=count_digits(b,d),i;u32 carry=0;for(i=0;i<n;i++){u32 v=c*b[i]+carry;a[i]=v;carry=v>>16;}if(i<d)a[i++]=carry;MI_CpuFill8(a+i,0,(d-i)*2); }
