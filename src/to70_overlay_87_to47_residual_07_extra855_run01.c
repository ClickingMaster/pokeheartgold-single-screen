#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x350];void *sprites[3],*icon;u8 settings[0x3a2-0x360];u8 ids[3];} State;
extern void ov87_021E803C(void *,u8),ov87_021E7FD4(void *,int),ov87_021E8084(void *,int),PlaySE(int);
int ov87_021E70D0(State *);
int ov87_021E70D0(State *s){u8 i;for(i=0;i<3;i++){ov87_021E803C(s->sprites[i],s->settings[s->ids[i]]);ov87_021E7FD4(s->sprites[i],1);}ov87_021E7FD4(s->icon,1);ov87_021E8084(s->icon,1);PlaySE(1377);return 0;}
