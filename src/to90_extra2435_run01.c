#pragma force_active on
#include "global.h"
#pragma force_active on

extern int ov00_021ECDC8(void),ov00_021EC11C(int*,u32*);
extern void ov75_02247878(void*),ov00_021EC210(void),ov00_021EC8D8(void),sub_0203A914(void),sub_0203957C(void),Sys_ClearSleepDisableFlag(u32),ov70_02237F2C(void),ov00_021FA0D8(void),ov00_021ED9B4(void),sub_020399EC(void);
typedef struct{u32 *owner;u32 pad,state,padC,pad10,status;int error;} W31790;
int ov75_022488BC(W31790*);
int ov75_022488BC(W31790*w){int error;u32 reason;switch(ov00_021ECDC8()){case 3:w->state=16;break;case 0:case 4:case 5:ov75_02247878(w);w->status=ov00_021EC11C(&error,&reason);w->error=error;ov00_021EC210();ov00_021EC8D8();sub_0203A914();sub_0203957C();Sys_ClearSleepDisableFlag(4);if(w->owner[0x46]==1){ov70_02237F2C();w->owner[0x46]=0;}w->state=27;switch(reason){case 1:case 2:w->state=27;break;case 4:ov00_021FA0D8();w->state=27;break;case 6:w->state=27;break;case 3:ov00_021ED9B4();w->state=27;break;case 5:case 7:sub_020399EC();break;}if(error<-20000&&error>=-29999)w->state=27;break;}return 0;}
