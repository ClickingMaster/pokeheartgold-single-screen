#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x2f0];void *value;u8 q[0x88c8-0x2f4];u16 flag;} Data;
typedef struct {u8 p[0x34];Data *data;} State;
extern void PCBox_SlideOutActionMenu(void *),PCBox_SlideInPartyButton(void *),ov14_021E82A8(void *),ov14_021E8328(void *),PCBox_HideSelectedIconOutline(State *),ov14_021E88F8(void *),ov14_021EA674(void),PCBox_WaitForUpdateFunc(State *,void (*)(void),int);
void ov14_021F2898(State *);
void ov14_021F2898(State *s){PCBox_SlideOutActionMenu(s->data->value);PCBox_SlideInPartyButton(s->data->value);ov14_021E82A8(s->data->value);ov14_021E8328(s->data->value);PCBox_HideSelectedIconOutline(s);if(s->data->flag)ov14_021E88F8(s->data->value);PCBox_WaitForUpdateFunc(s,ov14_021EA674,118);}
