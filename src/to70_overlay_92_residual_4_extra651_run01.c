#pragma force_active on
#include "global.h"
extern int ov90_02258B54(void*), ov90_02258B98(void*), IsPaletteFadeFinished(void); extern void sub_0200FB70(void),sub_0200FC20(int),ov92_0225E360(void*),ov92_0225E100(void*),ov92_0225D1FC(void*),ov90_02258A04(void*),ov90_02258938(void*);
typedef struct{u32 x;void *a;u8 pad[0x74];u8 flag;u8 gap[15];u8 obj[16];void*b;u8 gap2[24];void*c;}Work;
int ov92_0225D8E4(Work*w);
int ov92_0225D8E4(Work*w){if(ov90_02258B54(w->obj)==1){if(w->flag==0){if(IsPaletteFadeFinished()==1)sub_0200FB70();sub_0200FC20(0);w->flag=1;return 1;}else{if(ov90_02258B98(w->obj)==0)return 1;if(w->a){ov92_0225E360(w->a);ov92_0225E100(w->a);ov92_0225D1FC(w);}if(w->c){ov90_02258A04(w->c);w->c=0;}if(w->b){ov90_02258938(w->b);w->b=0;}return 2;}}return 0;}
