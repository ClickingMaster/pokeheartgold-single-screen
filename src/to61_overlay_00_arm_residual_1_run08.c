#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma warn_unusedvar off
#pragma warn_uninitializedvar off
#include <string.h>
extern int strncmp(const char *,const char *,u32);
extern char *strchr(const char *,int);
extern int OS_SNPrintf(char *,u32,const char *,...);
typedef struct {u32 magic,size;u8 pad[24];} Header;
typedef struct {void (*free)(int,void *,u32);void *(*alloc)(int,u32,int);} Alloc;
extern Alloc ov00_0221A754;
typedef struct {int handle;u16 state,flag,priority,one;u8 active,pad[3];u32 phase;u8 x14[32],x34[32];u32 x54;u8 x58[12];} Context;
typedef struct {int error;Context *p;} Root;
extern Root ov00_0221A75C;
typedef struct {int handle;u32 x04[8];int state;u32 x28;u8 aid,b2d;u16 name[26];u8 x62[6];void *x68;u32 x6c,cb70,arg74,cb78,arg7c,cb80,arg84,cb88,arg8c,cb90,arg94;u8 x98[0x2e9];u8 b381;u8 pad382[7];volatile u8 mode;u8 pad38a[2];u8 b38c;u8 pad38d[0x2b7];u32 list[8],bits;} Big;
extern Big *ov00_0221A784;
typedef struct {void (*cb)(int,int,int);void (*end)(void);u8 x8[0x40];u32 running;u8 pad[0x974];OSThread thread;} ThreadData;
extern ThreadData *ov00_0221A768;
extern u8 *ov00_0221A764;
extern char ov00_02217938[],ov00_02217960[],ov00_02217984[],ov00_022179A4[],ov00_022179AC[],ov00_022179E0[],ov00_022179F4[];
extern void CPS_SetSslHandshakePriority(u16);
extern int WCM_GetLinkLevel(void),ov00_021FAAC8(void),ov00_021EC230(void),ov00_021EBB18(void),ov00_021FAA40(void),ov00_021EE64C(int),ov00_021F3570(void),ov00_021F3550(void),ov00_021F3618(void **),ov00_021F35E4(void **),ov00_021EE880(void *,int),ov00_021EE7B4(int);
extern void ov00_021E7B60(const char *),ov00_021EC24C(int,int),ov00_021EB864(void),ov00_021E7D48(void),ov00_0220D218(void),ov00_0220D238(int),ov00_021F125C(void),ov00_021EE800(int),ov00_021F04A0(int,const char *,int);
extern int ov00_021EB7CC(void *(*)(int,u32),void (*)(int,void *,u32)),ov00_021EB8B0(const char *,void *);
extern void ov38_02220620(void (*)(void));
extern int ov00_021EC724(void);extern void ov00_021EC60C(void);
extern int ov38_02220090(int,int,int),ov38_02220F18(int),ov38_02220F8C(void *,int,int),ov38_0222124C(int,int,int),ov38_02220648(void),ov38_02221408(int,int);
Header * ov00_021EC264(void *p);
void * ov00_021EC26C(Header *p,u32 n);
u32 ov00_021EC284(void *p);
void ov00_021EC294(void *(*a)(int,u32,int),void (*f)(int,void *,u32));
void * ov00_021EC2A8(int a,u32 n);
void * ov00_021EC2B8(int a,u32 n,int align);
void ov00_021EC2EC(int a,void *p,u32 unused);
void * ov00_021EC328(int a,void *p,u32 n,u32 size);
void * ov00_021EC33C(int a,void *p,u32 unused,u32 n,int align);
void * ov00_021EC3A8(u32 n);
void * ov00_021EC3BC(void *p,u32 n);
void ov00_021EC3D8(void *p);
void ov00_021EC3F0(Context *p,u16 priority,int unused,u16 ssl);
void ov00_021EC454(int n);
int ov00_021EC5B4(void);
void ov00_021EC8D8(void);
int ov00_021EC938(void);
int ov00_021EC9A8(void);
int ov00_021EC9D4(void);
int ov00_021ECB40(void);
int ov00_021ECC6C(const char *name,void *p);
int ov00_021ECCE0(void);
void ov00_021ECEC0(void);
void * ov00_021ECECC(u32 n,int align);
void ov00_021ECEE4(void *p);
void ov00_021ECFF4(void);
int ov00_021ED308(void (*end)(void));
int ov00_021ED354(int a,int b,int c);
int ov00_021ED388(int a);
int ov00_021ED3AC(void *a,int b,int c);
int ov00_021ED3F4(int a,int b,int c);
int ov00_021ED428(void);
int ov00_021ED444(int a,int b);
u32 ov00_021ED470(const char *a,const char *b,char *dest,int d);
u32 ov00_021ED4A8(const char *a,const char *b,char *dest,int d);
u32 ov00_021ED704(const u16 *p);
int ov00_021EE24C(u32 a,u32 b);
int ov00_021EE31C(int aid);
int ov00_021EE448(void);
int ov00_021EE490(void);
int ov00_021EE4AC(void **out);
int ov00_021EE4FC(void);
int ov00_021EE530(int aid);
int ov00_021EE568(void);

typedef struct {void *(*alloc)(int,u32);void (*free)(int,void *,u32);u8 priority,one,pad[2];} Params;
extern int ov00_021FA75C(Params *),ov00_021FAB04(void *,void *,u32,void *,int);
extern void ov00_021FE070(void);extern int ov00_021FDD3C(void),ov00_021FA904(void),WCM_GetPhase(void);
extern void ov00_021E7B70(void *),ov00_021E7DD4(void),ov00_021E7EA8(void *);
extern int ov00_021E7E5C(void);
void ov00_021EC4A4(void);
void ov00_021EC558(void *a,void *b,u32 c,void *d,int e);
void ov00_021EC60C(void);
int ov00_021ECDC8(void);
void ov00_021ECEFC(int a,unsigned b,int c);
int ov00_021ED4E8(const char *key,char *out,const char *src,int sep);
int ov00_021EE278(void);
int ov00_021EE384(u32 *bits);
extern int ov00_021FA9D4(void);extern u32 CPS_GetSslHandshakePriority(void);extern void ov00_021FDCDC(void *);
extern void ov00_021EE6FC(void),ov00_021F1938(int,int,void *,int,int,int),ov00_021F1B24(int,void *,int,int,int);
extern void ov00_021EEB1C(void);
int ov00_021EC724(void);
int ov00_021EDF1C(int aid,int a,u32 cb,u32 arg,int x,int y);
int ov00_021EDFBC(int aid,u32 cb,u32 arg,int x,int y);
extern RTCResult RTC_GetDateTime(RTCDate *,RTCTime *);
extern int ov00_0221A734;extern s64 ov00_0221A738,ov00_0221A740;
extern void ov00_021F125C(void),ov00_0220AE60(void),ov00_021EF410(void),ov00_021F05D8(void),ov00_021F378C(void),ov00_021F9CA8(void),ov00_0220CFAC(int),ov00_022136E4(void *),ov00_02210D18(void *);
extern void ov00_021FFDD0(void *,int,int,int),ov00_021FFD9C(void *),ov00_021FFD80(void *);
int ov00_021ECB94(RTCDate *date,RTCTime *time);
int ov00_021ECD04(void);
void ov00_021ED9B4(void);
void ov00_021ED9B4(void) { if(!ov00_0221A784)return;if(*(void **)((u8 *)ov00_0221A784+0x384)){ov00_02210D18(*(void **)((u8 *)ov00_0221A784+0x384));*(void **)((u8 *)ov00_0221A784+0x384)=0;}ov00_0221A784->b38c=0;if(*(void **)((u8 *)ov00_0221A784+0x458)){ov00_022136E4(*(void **)((u8 *)ov00_0221A784+0x458));*(void **)((u8 *)ov00_0221A784+0x458)=0;}ov00_021F125C();ov00_0220AE60();if(ov00_0221A784->x04[6]){ov00_021FFDD0(&ov00_0221A784->x04[6],0,0,0);ov00_021FFDD0(&ov00_0221A784->x04[6],3,0,0);ov00_021FFDD0(&ov00_0221A784->x04[6],1,0,0);ov00_021FFDD0(&ov00_0221A784->x04[6],2,0,0);ov00_021FFD9C(&ov00_0221A784->x04[6]);ov00_021FFD80(&ov00_0221A784->x04[6]);ov00_0221A784->x04[6]=0;}ov00_021EF410();ov00_021F05D8();ov00_021F378C();ov00_021F9CA8();if(ov00_0221A784->handle){ov00_0220CFAC(ov00_0221A784->handle);ov00_0221A784->handle=0;}ov00_0221A784=0; }
