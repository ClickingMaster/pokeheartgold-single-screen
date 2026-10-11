#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W31777Parent {u32 a;void *save;} W31777Parent;
typedef struct W31777 {W31777Parent *parent;u8 a[0x18];u32 state;u8 b[0x70];u32 field90,field94,field98,field9c;u8 data[0xe7c];u32 error;u8 tail[0x74];u32 timer;} W31777;
extern int NintendoWfcHttp_IsDone(void),NintendoWfcHttp_GetResult(void);
extern void sub_0202D7C0(void *,void *,u8,u8),ov73_021E83EC(W31777 *,int,int),ov00_021ECB94(void *,void *),sub_0202D678(void *,u8,u8,void *),ov73_021EA180(W31777 *),sub_020399EC(void);
int ov73_021E944C(W31777 *);
int ov73_021E944C(W31777 *p){int result;u32 time[3],date[4];if(NintendoWfcHttp_IsDone()){result=NintendoWfcHttp_GetResult();p->timer=0;switch(result){case 0:sub_0202D7C0(p->parent->save,p->data,(u8)p->field90,(u8)p->field9c);ov73_021E83EC(p,39,19);ov00_021ECB94(date,time);sub_0202D678(p->parent->save,(u8)p->field94,(u8)p->field9c,date);break;case -1:p->error=result;p->state=53;ov73_021EA180(p);break;case -2:p->error=result;p->state=53;ov73_021EA180(p);break;case -3:case -4:p->error=result;p->state=11;ov73_021EA180(p);break;case -6:p->error=result;p->state=53;ov73_021EA180(p);break;case -7:p->error=result;p->state=53;ov73_021EA180(p);break;case -5:p->error=result;p->state=53;ov73_021EA180(p);sub_020399EC();break;}}else{p->timer++;if(p->timer==3600)sub_020399EC();}return 3;}
