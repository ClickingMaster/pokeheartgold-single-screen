#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern int rand(void);
extern void __msl_assertion_failed(const char*,const char*,const char*,int);
typedef struct {int state;u8 p[0x70];char token[8];void*buffer;int pos;u8 q[0x4a4-0x84];int extra;u8 r[8];int socket;u8 t[4];int number;u8 u[0x5c4-0x4bc];u32 flags;int timer;} Work;
extern int ov00_021FF970(int,const void*,int,int);
extern void ov00_02214908(Work*);
extern void ov00_02214130(Work*);
extern int ov00_02214244(Work*);
extern void ov00_022143C0(u8**,int,int*);
extern void ov00_022143E4(u8**,u32,int*);
extern void ov00_02214370(u8**,const void*,int*);
extern void ov00_02214438(u8**,const void*,int,int*);
extern void *ov00_021EC3A8(int);
extern char ov00_0221A618[],ov00_0221A5C8[],ov00_0221A59C[],ov00_0221A5B8[];
static inline void copy2(u8 *dst,const u8 *src){dst[0]=src[0];dst[1]=src[1];}
void ov00_0221446C(Work*);
int ov00_02214558(Work*,const void*,int);
int ov00_022145F0(Work*,const char*,const char*,u32,int);
int ov00_02214558(Work*w,const void*data,int size){int n,retry=1,r;do{retry--;n=ov00_021FF970(w->socket,data,size,0);if(n>0||retry<0)break;ov00_02214908(w);r=ov00_022145F0(w,0,0,2,0);if(r){ov00_02214130(w);return r;}}while(retry>=0);return n<=0?3:0;}
