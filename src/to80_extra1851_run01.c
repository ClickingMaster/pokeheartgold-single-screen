#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 words[78];} Snapshot;
typedef struct {Snapshot snapshot;u32 pad;int state;u8 q[0x250-0x140];int error;u32 length;void*buffer;int token;u32 kind;u8 r[0x268-0x264];int stop;} Global;
extern Global _0222AB84;
extern int ov39_02229FCC(void),ov39_02229AD4(void),ov39_02229BE4(void),ov39_02229D2C(void),ov39_02229E40(void);extern void ov39_022298FC(void),ov39_02229910(void),ov39_022298E0(void*),ov39_02229FC0(int);
extern int ov39_0222A6DC(int),ov39_0222A59C(int),ov39_02229980(void);extern void *ov39_022298CC(int);extern void ov39_02229408(void),ov39_0222A4EC(void *,u32,void *);extern void *memcpy(void *,const void *,u32);
int ov39_02229630(int,void*,u32,int);
void ov39_02229474(void);
static inline int Next2(BOOL *done){int next;if(!ov39_02229FCC()){next=_0222AB84.error==13?7:9;*done=1;}else next=3;return next;}
void ov39_02229474(void){BOOL done;int next;for(;;){done=0;switch(_0222AB84.state){case 0:case 1:case 7:case 8:case 9:done=1;break;case 2:next=Next2(&done);ov39_022298FC();_0222AB84.state=next;break;case 3:if(!ov39_02229AD4()){done=1;next=_0222AB84.error==13?7:9;}else next=4;ov39_022298FC();_0222AB84.state=next;break;case 4:if(!ov39_02229BE4()){done=1;next=_0222AB84.error==13?7:9;}else next=5;ov39_022298FC();_0222AB84.state=next;break;case 5:if(!ov39_02229D2C()){done=1;next=_0222AB84.error==13?7:9;}else next=6;if(_0222AB84.buffer){ov39_022298E0(_0222AB84.buffer);_0222AB84.buffer=0;}ov39_022298FC();_0222AB84.state=next;break;case 6:switch(ov39_02229E40()){case 0:next=_0222AB84.error==13?7:9;break;default:next=8;break;}done=1;ov39_022298FC();_0222AB84.state=next;break;}if(done)break;if(_0222AB84.stop==1){_0222AB84.state=7;_0222AB84.error=13;break;}ov39_02229910();ov39_02229FC0(1);}ov39_02229910();}
