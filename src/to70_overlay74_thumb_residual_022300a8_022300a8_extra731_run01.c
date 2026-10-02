#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x61];u8 count;u8 pad2[2];u8 other;}State;
extern State*ov74_0223115C(void);extern int ov74_0222FEC8(void),ov74_022314DC(void),ov74_02231508(void);extern void ov74_0222FE4C(void),ov74_0222FF68(void),ov74_02231448(void);
void ov74_022300A8(u16*);
void ov74_022300A8(u16*p){if(p[1]==0){State*s=ov74_0223115C();if(s->count>p[5]){s->count=p[5];s->other=p[4];}if(ov74_0222FEC8()){if(ov74_022314DC()==0)ov74_0222FE4C();}else{if(s->count<102){ov74_0222FF68();if(ov74_02231508()==0)ov74_0222FE4C();}else ov74_0222FE4C();}}else{ov74_02231448();ov74_0222FE4C();}}
