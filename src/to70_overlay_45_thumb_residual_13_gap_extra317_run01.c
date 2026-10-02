#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[0x1c0];int a;u16 id;u8 active;u8 gap[3];u8 flag;u8 gap2[0x1e0-0x1cb];int data;};void*ov45_0222A578(struct S*,int);int ov45_0222A920(void*);void ov45_0222BE00(void*,int);void*ov45_0222EC90(int);void ov45_0222EF4C(int,void*,void*,int);void ov45_0222BE48(void*);int ov45_0222AE74(struct S*,int);
int ov45_0222AE74(struct S*s,int id){if(s->flag)return 0;if(ov45_0222A920(ov45_0222A578(s,id))!=1)return 0;s->id=id;s->active=1;s->flag=1;ov45_0222BE00(&s->a,0);ov45_0222EF4C(0,ov45_0222EC90(id),&s->data,4);ov45_0222BE48(&s->a);return 1;}
