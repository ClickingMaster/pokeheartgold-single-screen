#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x318];int state;u8 pad2[0x4a18-0x31c];int count,previous,changed;u8 pad3[5];s8 mode;} State;
extern int ov73_021E746C(void);extern int sub_02037454(void);extern u32 sub_02033250(void);extern void sub_02037030(int,u8 *,int),ov73_021E781C(State *,int),ov73_021E6830(State *),sub_0205A904(int);
int ov73_021E7870(State *,int);
int ov73_021E7870(State *s,int arg){u8 flag[1];int n=ov73_021E746C();if(n>s->previous){flag[0]=1;sub_02037030(117,flag,1);s->changed=1;}else s->changed=0;if(n==s->count)return 1;switch(n){case 1:if(sub_02037454()>1||sub_02033250()>1)return 1;s->state=17;ov73_021E781C(s,-1);ov73_021E6830(s);return 2;case 2:case 3:case 4:sub_0205A904(2);if(n<s->count){switch(s->mode){case 0:ov73_021E781C(s,arg);break;case 1:ov73_021E781C(s,0);break;}s->previous=sub_02037454();}break;case 5:sub_0205A904(9);ov73_021E781C(s,arg);break;}s->count=ov73_021E746C();return 1;}
