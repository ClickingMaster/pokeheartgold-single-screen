#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[4];void *save;} Inner;
typedef struct {Inner *inner;u8 pad[0x1c-4];int state;u8 pad2[0x94-0x20];int a;u8 pad3[4];int b;u8 pad4[0x6dc-0xa0];u8 data[0xf1c-0x6dc];int result;u8 pad5[0xf94-0xf20];int timer;} State;
extern int ov72_02237B54(void),ov72_02237B74(void);extern void sub_0202D8A4(void *,void *,u8,u8),ov73_021E83EC(State *,int,int),ov73_021EA180(State *),sub_020399EC(void);
int ov73_021E9A6C(State *);
int ov73_021E9A6C(State *s){if(ov72_02237B54()){int n=ov72_02237B74();s->timer=0;switch(n){case 0:sub_0202D8A4(s->inner->save,s->data,s->a,s->b);ov73_021E83EC(s,39,38);break;case -1:s->result=n;s->state=53;ov73_021EA180(s);break;case -2:s->result=n;s->state=53;ov73_021EA180(s);break;case -3:s->result=n;s->state=11;ov73_021EA180(s);break;case -6:s->result=n;s->state=53;ov73_021EA180(s);break;case -4:case -7:s->state=11;s->result=n;ov73_021EA180(s);break;case -5:s->result=n;s->state=53;ov73_021EA180(s);sub_020399EC();break;}}else{if(++s->timer==3600)sub_020399EC();}return 3;}
