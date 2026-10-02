#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *a,*bg;u8 p[0x2cc-8];u8 values[];} Data;
typedef struct {u8 p[12];Data *data;u8 q[12];u16 row;} State;
extern void sub_0201980C(void *,u16),sub_020197F4(void *,u16),ov103_021EE468(State *,u16,u16,u8);
void ov103_021ED0C0(State *);
void ov103_021ED0C0(State *s){u32 offset=(u16)(s->row*10);u16 id=0;u8 *values=s->data->values+offset;u16 index=offset;{u16 i;for(i=0;i<10;i++){u8 value=values[i];if(value==255)sub_0201980C(s->data->bg,id);else{ov103_021EE468(s,id,index,value);sub_020197F4(s->data->bg,id);}index++;id++;}}}
