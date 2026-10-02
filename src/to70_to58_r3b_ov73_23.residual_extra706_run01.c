#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x1c];int state;u8 pad2[0xf10-0x20];int result,error;} Work;
extern u32 ov00_021ECDC8(void);extern void ov73_021EA180(Work *);extern int ov00_021EC11C(int *,int *);extern void ov00_021EC210(void);extern void ov00_021EC8D8(void);extern void ov00_021FA0D8(void);extern void ov00_021ED9B4(void);extern void sub_020399EC(void);
int ov73_021E8DC8(Work *);
int ov73_021E8DC8(Work *w){int error,kind;switch(ov00_021ECDC8()){case 3:w->state=6;break;case 0:case 4:case 5:ov73_021EA180(w);w->result=ov00_021EC11C(&error,&kind);w->error=error;ov00_021EC210();ov00_021EC8D8();w->state=55;switch(kind){case 1:case 2:w->state=55;break;case 4:ov00_021FA0D8();w->state=55;break;case 6:w->state=55;break;case 3:ov00_021ED9B4();w->state=55;break;case 5:case 7:sub_020399EC();break;}if(error< -20000 && error>= -29999)w->state=55;break;}return 3;}
