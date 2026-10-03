#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union {u32 all;struct{u32 a:8,b:4,c:20;};} Flags;
typedef struct{u8 p[0x3c];u16 size;u16 p2;u32 p3,value;u32 p4[2];Flags flags;union {u32 other;struct{u32 low:16,high:16;};};} Packet;
typedef struct{u32 records[8][14];u8 p[3];u8 index:4,other:4;} State;
extern Packet*ov74_02231100(void);
extern State*ov74_02231154(void);
extern void*ov74_02231054(void);
extern int ov74_0222FE78(void*);
extern u32 ov74_022311A0(void),ov74_022311AC(void),ov74_022311BC(void),ov74_022311CC(void);
extern void ov74_02231454(State*);
extern u32 ov74_0222FE5C(void*);
extern int ov74_0223161C(void);
extern void ov74_0222FE4C(void);
int ov74_02230478(void);
int ov74_02230478(void){Packet*p;Flags*flags;State*s;p=ov74_02231100();s=ov74_02231154();if(ov74_0222FE78(ov74_02231054()))return 1;DC_InvalidateRange(p,0xc0);ov74_02231454(ov74_02231154());if(p->size>=8 && p->value==ov74_022311A0()){flags=&p->flags;if(flags->a==ov74_022311AC() && flags->b<=ov74_022311BC() && (*(Flags *)((u8 *)flags+4)).all>>16==ov74_022311CC()){u32 value=ov74_0222FE5C((u8*)p+4);volatile struct{u32 a,b;} pair;pair.a=value;pair.b=value;u32 record=s->records[s->index][0];if(record==value){if(!ov74_0223161C())ov74_0222FE4C();return 1;}}}return 0;}
