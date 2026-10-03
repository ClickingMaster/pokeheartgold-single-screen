#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma inlineintrinsics on
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
int ov00_022145F0(Work*w,const char*a,const char*b,u32 flags,int extra){u8 buffer[0x300];int length;u8*cursor;u16 swapped;int r;u32 swap;
if(w->state!=1)__msl_assertion_failed(ov00_0221A618,ov00_0221A5C8,ov00_0221A59C,522);if(!a)a=ov00_0221A5B8;if(!b)b=ov00_0221A5B8;if(strlen(a)>256)return 6;if(strlen(b)>256)return 6;r=ov00_02214244(w);if(r)return r;w->flags=flags;ov00_0221446C(w);length=2;cursor=buffer+2;ov00_022143C0(&cursor,0,&length);ov00_022143C0(&cursor,1,&length);ov00_022143C0(&cursor,3,&length);ov00_022143E4(&cursor,w->number,&length);ov00_02214370(&cursor,(u8*)w+0xc,&length);ov00_02214370(&cursor,(u8*)w+0x30,&length);ov00_02214438(&cursor,w->token,8,&length);ov00_02214370(&cursor,b,&length);ov00_02214370(&cursor,a,&length);swap=((flags<<24)&0xff000000)|(((flags<<8)&0xff0000)|(((flags>>24)&0xff)|((flags>>8)&0xff00)));ov00_022143E4(&cursor,swap,&length);if(w->flags&8)ov00_022143E4(&cursor,w->extra,&length);if(w->flags&0x80)ov00_022143E4(&cursor,extra,&length);{u8 *p=(u8*)&swapped;volatile u8 *dst=buffer;swapped=(((u16)length>>8)&0xff)|(((u16)length<<8)&0xff00);memcpy((u8*)dst,p,2);}r=ov00_021FF970(w->socket,buffer,length,0);if(r<=0){ov00_02214908(w);return 3;}w->state=3;w->timer=0;if(!w->buffer){w->buffer=ov00_021EC3A8(4096);if(!w->buffer)return 5;w->pos=0;}return 0;}
