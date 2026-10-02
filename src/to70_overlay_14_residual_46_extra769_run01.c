#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x2f0];void *value;u8 q[0x88c8-0x2f4];u16 flag;} Data;
typedef struct {u8 p[0x34];Data *data;} State;
extern void ov14_021E84A4(void *),ov14_021E8248(void *),ov14_021E82A8(void *),ov14_021E8328(void *),ov14_021F40DC(State *),ov14_021E88F8(void *),ov14_021EA674(void),ov14_021F0234(State *,void (*)(void),int);
void ov14_021F2898(State *);
void ov14_021F2898(State *s){ov14_021E84A4(s->data->value);ov14_021E8248(s->data->value);ov14_021E82A8(s->data->value);ov14_021E8328(s->data->value);ov14_021F40DC(s);if(s->data->flag)ov14_021E88F8(s->data->value);ov14_021F0234(s,ov14_021EA674,118);}
