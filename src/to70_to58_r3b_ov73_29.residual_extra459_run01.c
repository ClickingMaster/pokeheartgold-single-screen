#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x18];int mode;} Settings;
typedef struct {Settings *settings;u8 pad[0x18];int state;u8 pad2[0xefc];int response;u8 pad3[0x74];volatile int timer;} Work;
extern int NintendoWfcHttp_IsDone(void);extern int NintendoWfcHttp_GetResult(void);extern void ov73_021EA180(Work *);extern void ov73_021E83EC(Work *,int,int);extern void sub_020399EC(void);
extern void sub_0202D5DC(void *,int,int);
int ov73_021E9618(Work *);
int ov73_021E9618(Work *w){if(NintendoWfcHttp_IsDone()){int result=NintendoWfcHttp_GetResult();w->timer=0;switch(result){case 0:sub_0202D5DC(*(void **)w->settings,5,2);ov73_021E83EC(w,39,28);break;case -1:w->response=result;w->state=53;ov73_021EA180(w);break;case -2:w->response=result;w->state=53;ov73_021EA180(w);break;case -3:w->response=result;w->state=53;ov73_021EA180(w);break;case -6:w->response=result;w->state=53;ov73_021EA180(w);break;case -7:case -4:w->response=result;w->state=53;ov73_021EA180(w);break;case -5:w->response=result;w->state=53;ov73_021EA180(w);sub_020399EC();break;}}else{w->timer++;if(w->timer==3600)sub_020399EC();}return 3;}
