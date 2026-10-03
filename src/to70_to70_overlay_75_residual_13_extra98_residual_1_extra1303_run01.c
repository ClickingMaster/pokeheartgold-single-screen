#pragma force_active on
#include "global.h"
typedef struct {u8 p[0xfc];int state,kind;} Outer;
typedef struct {Outer *outer;int p,state,q,error;u8 pad[0xd4];int timer;} Work;
extern int ov70_02237F38(void*),ov70_02237F58(void);extern void ov75_02247878(Work*),sub_020399EC(void);
int ov75_02248A20(Work*);
int ov75_02248A20(Work*w){if(ov70_02237F38(w)){int result=ov70_02237F58();w->timer=0;switch(result){case 0:switch(w->outer->state){case 0:switch(w->outer->kind){case 1:w->state=19;break;case 2:ov75_02247878(w);w->error=-5001;w->state=32;break;case 0:ov75_02247878(w);w->error=-5002;w->state=32;break;case 3:ov75_02247878(w);w->error=-5003;w->state=32;break;default:sub_020399EC();break;}break;case 1:ov75_02247878(w);w->error=-5004;w->state=32;break;case 2:ov75_02247878(w);w->error=-5005;w->state=32;break;default:ov75_02247878(w);sub_020399EC();break;}break;case 1:ov75_02247878(w);w->error=result;w->state=32;break;case -1:case 2:ov75_02247878(w);w->error=result;w->state=32;break;case -15:case -12:ov75_02247878(w);w->error=result;w->state=32;break;case -14:case -2:ov75_02247878(w);w->error=result;w->state=32;break;default:ov75_02247878(w);sub_020399EC();break;}}else{w->timer++;if(w->timer==3600)sub_020399EC();}return 0;}
