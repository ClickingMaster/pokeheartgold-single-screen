#pragma force_active on
#include "global.h"
typedef struct {u8 p[4];void*save;}Root;typedef struct{Root*root;u8 p[0x1c-4];int state;u8 q[0x94-0x20];int a;u8 r[4];int b;u8 t[0x6dc-0xa0];u8 data[0xf5c-0x6dc];int result;u8 u[0xfd4-0xf60];volatile int timer;}State;
extern int ov72_02237B54(void),ov72_02237B74(void);extern void sub_0202D8A4(void*,void*,u8,u8),ov72_02238680(State*,int,int),ov72_0223A444(State*),sub_020399EC(void);
int ov72_02239D6C(State*);
int ov72_02239D6C(State*s){if(ov72_02237B54()){int result=ov72_02237B74();s->timer=0;switch(result){case 0:sub_0202D8A4(s->root->save,s->data,(u8)s->a,(u8)s->b);ov72_02238680(s,39,38);break;case -1:s->result=result;s->state=53;ov72_0223A444(s);break;case -2:s->result=result;s->state=53;ov72_0223A444(s);break;case -3:s->result=result;s->state=11;ov72_0223A444(s);break;case -6:s->result=result;s->state=53;ov72_0223A444(s);break;case -7:case -4:s->state=11;s->result=result;ov72_0223A444(s);break;case -5:s->result=result;s->state=53;ov72_0223A444(s);sub_020399EC();break;}}else{++s->timer;if(s->timer==3600)sub_020399EC();}return 3;}
