#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x1c];int state,previous;u8 pad2[0x98-0x24];int value;u8 pad3[0xf1c-0x9c];int result;u8 pad4[0xf94-0xf20];volatile int timeout;} Work;
extern int ov72_02237B54(void),ov72_02237B74(void);extern void ov73_021EA180(Work*),sub_020399EC(void);

int ov73_021E9144(Work*w);

int ov73_021E9144(Work*w){if(ov72_02237B54()){int r=ov72_02237B74();w->timeout=0;switch(r){case -1:w->result=r;w->state=53;break;case -2:w->result=r;w->state=53;break;case -3:w->result=r;w->state=53;break;case -6:w->result=r;w->state=53;break;case -7:case -4:w->result=r;w->state=53;break;case -5:w->result=r;w->state=53;ov73_021EA180(w);sub_020399EC();break;default:if(r>0){w->value=r;w->state=w->previous;}break;}ov73_021EA180(w);}else{w->timeout++;if(w->timeout==3600)sub_020399EC();}return 3;}
