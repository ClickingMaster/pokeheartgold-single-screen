#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u8 bytes[2];} Pair;typedef struct{void *a;void *b;u8 buf[33];u8 val;} G;extern Pair ov13_02245454;extern G *ov13_0224F540;
extern void *ov13_0224128C(int,int),*ov13_022408A0(int,int),*ov13_02240184(int,int);
extern void ov13_0222B948(int*,int*),ov13_0223A28C(void*),ov13_0223110C(void),ov13_02227DE4(int),ov13_022277B8(int,int,int),ov13_02227964(int),ov13_022317A8(void),ov13_022316B4(void),ov13_022311C8(void),ov13_0222B8E8(void(*)(void));extern u8 ov13_02242074(void*,int);
void ov13_02230FD8(void);
void ov13_02230FD8(void){Pair p=ov13_02245454;int a,b;ov13_0224F540=ov13_0224128C(44,4);ov13_0222B948(&a,&b);if(a==0){ov13_0223A28C(ov13_0224F540->buf);ov13_0224F540->val=ov13_02242074(ov13_0224F540->buf,32);}ov13_0223110C();ov13_02227DE4(a+9);if(b==1)ov13_022277B8(53,-1,0);else ov13_022277B8(p.bytes[a],-1,0);ov13_02227964(2);ov13_0224F540->a=ov13_022408A0(0,0);ov13_0224F540->b=ov13_02240184(0,62);{u16 *q=ov13_0224F540->b;q[2]=(q[2]&~0xc00)|0xc00;}ov13_022317A8();ov13_022316B4();ov13_0222B8E8(ov13_022311C8);}
