#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma warn_unusedvar off
#pragma warn_uninitializedvar off
#define B(p,o) (((u8 *)(p))[o])
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define T(p,o) (*(u64 *)((u8 *)(p)+(o)))
typedef struct {u8 id,type,pad[2];void *owned;void *value;} Entry;
extern Entry ov00_0221A948[];
typedef struct {u8 pad[0xf4];u32 profiles[32];} Profiles;
typedef struct {u8 active,one,attempt,pad;u32 time,bits,other;u64 start,last;} Timer;
extern struct {Timer *timer;u8 *context;u32 extra[5];} ov00_0221A928;
typedef struct {u8 *send,*recv;int size,sent,received,total,pending;u8 sending,receiving;u16 pad;u16 p20,p22;u64 tick;u32 timeout;} Channel;
typedef struct {Channel channel[32];void (*sent)(int,int);void (*recv)(int,const void *,int);void (*timeout)(int);void (*other)(int,int);u16 max,pad;} Net;
extern Net *ov00_0221B0A0;
extern char ov00_02217B70[],ov00_02217B7C[],ov00_02217AB4[],ov00_02217AC0[],ov00_02217ACC[],ov00_02217AAC[],ov00_02217B88[];
extern void ov00_021EC2EC(int,void *,int),ov00_021EC24C(int,int),ov00_021F2EE4(int,int),ov00_021EF3C0(int,int),ov00_021F0238(int,int);
extern int ov00_02212DB4(void *,const char *,int),ov00_021F03BC(int),ov00_021EE568(void),ov00_021EC230(void),ov00_021EE530(int),ov00_021EE7B4(int),ov00_021FA090(int),ov00_021EE490(void),ov00_021EE64C(int),ov00_021EE6B0(int);
extern void ov00_0220D194(int,const void *,int,int),ov00_021F9E28(int,const void *,int,int),ov00_021F9EA8(int,const void *,int),ov00_021F9F70(int,const void *,int),ov00_021FA054(int,const void *,int);
extern int memcmp(const void *,const void *,u32);extern char *strncpy(char *,const char *,u32);
extern int ov00_021F9630(int,int,const void *,int);
u8 ov00_021F7A98(void);
void ov00_021F7AD8(void);
u8 ov00_021F7B30(void);
int ov00_021F7BA0(int profile);
int ov00_021F7C10(void *p);
u8 ov00_021F7CEC(u32 profile,int first);
u32 ov00_021F7D50(int flag);
void ov00_021F7DB4(void);
void ov00_021F7DDC(int flag);
int ov00_021F8278(unsigned n);
int ov00_021F82EC(unsigned n);
int ov00_021F8380(unsigned n);
int ov00_021F8468(unsigned n);
int ov00_021F8514(unsigned n);
void ov00_021F89CC(void);
void ov00_021F8E60(void);
void ov00_021F8E64(void);
int ov00_021F8F0C(void);
int ov00_021F8F14(unsigned n);
void ov00_021F8F20(u32 a,u16 b);
void ov00_021F9100(void);
int ov00_021F94DC(int flag);
void ov00_021F954C(u8 *p);
u8 * ov00_021F955C(void);
void ov00_021F956C(int state);
int ov00_021F9580(int aid);
int ov00_021F9590(int aid,int flag);
int DWC_SendReliable(int aid,const void *p,int n);
int DWC_SendUnreliable(int aid,const void *p,int n);
int ov00_021F9850(int aid,u8 *p,int n);
int ov00_021F989C(void (*cb)(int,int));
int ov00_021F98BC(void (*cb)(int,const void *,int));
int ov00_021F98DC(void (*cb)(int));
void ov00_021F98FC(void *p,int flag,u32 n);
unsigned ov00_021F992C(const void *p);
int ov00_021F9968(int n);
int ov00_021F9988(int aid,u32 timeout);
void ov00_021F99D4(Net *p);
void ov00_021F9A10(int aid,const void *p,int n,int flag);
void ov00_021F9A4C(int id,int n);
void ov00_021F9C20(int aid);
void ov00_021F9CA8(void);
Channel * ov00_021F9CBC(int aid);
int ov00_021F9CD4(int aid);
unsigned ov00_021F9CF0(int aid);
void ov00_021F9D0C(int aid,const void *p,int n,int flag);
void ov00_021F9D34(int id,const void *p,int n);

extern void ov00_02210D9C(void *,int);extern int ov00_021F3F5C(int,int,int);
extern int ov00_021EE4AC(u8 **);
void ov00_021F8E68(int state,void *p);
void ov00_021F8F40(int profile);
int ov00_021F9630(int flag,int aid,const void *p,int n);
u32 ov00_021F9750(u32 bits,const void *p,int n);
void ov00_021F9A8C(void);
extern u8 ov00_0221A949[];extern void *ov00_0221A950[];
extern void ov00_02210DC8(void *,int),ov00_02210DFC(void *,void *);
void ov00_021F8D18(int,void *);
int ov00_021F8468(unsigned n) { int cat,off;if(!n)return 0;switch(n){case 1:cat=9;off=-1;break;case 2:cat=6;off=-50;break;case 3:cat=6;off=-30;break;}ov00_021F2EE4(cat,off-86000);return n; }
