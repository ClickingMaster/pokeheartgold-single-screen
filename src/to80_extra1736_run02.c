#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern char ov45_022554F0[];
extern char ov45_02255468[];
extern char ov45_022553D8[];
extern char ov45_02255458[];
extern char ov45_02255478[];
extern char ov45_02255488[];
extern char ov45_022554A4[];
extern char ov45_022554BC[];
extern char ov45_022554DC[];
extern char ov45_022554EC[];
void __msl_assertion_failed(const char*,const char*,const char*,int);
void ov45_02248558(void*,u32);
void ov45_02244500(void*);
typedef struct{int type;void (*callback)(void);void *arguments;void *user;int field;}Task;
typedef struct{u32 value[1];} Args0;
typedef struct{u32 value[1];} Args1;
typedef struct{u32 value[3];} Args2;
typedef struct{u32 value[2];} Args3;
typedef struct{u32 value[4];} Args4;
typedef struct{u32 value[3];} Args5;
typedef struct{u32 value[3];} Args6;
typedef struct{u32 value[5];} Args7;
typedef struct{u32 value[3];} Args8;
typedef struct{u32 value[2];} Args9;
typedef struct{u32 value[2];} Args10;
typedef struct{u32 value[3];} Args11;
typedef struct{u32 value[1];} Args12;
typedef struct{u32 value[5];} Args13;
typedef struct{u32 value[5];} Args14;
typedef struct{u32 value[3];} Args15;
typedef struct{u32 value[3];} Args16;
typedef struct{u32 value[3];} Args17;
typedef struct{u32 value[4];} Args18;
typedef struct{u32 value[5];} Args19;
typedef struct{u32 value[7];} Args20;
typedef struct{u32 value[4];} Args21;
typedef struct{u32 value[5];} Args22;
typedef struct{u32 value[4];} Args23;
typedef struct{u32 value[4];} Args24;
typedef struct{u32 value[4];} Args25;
typedef struct{u32 value[3];} Args26;
typedef struct{u32 value[4];} Args27;
typedef struct{u32 value[4];} Args28;
typedef struct{u32 value[5];} Args29;
typedef struct{u32 value[6];} Args30;
typedef struct{u32 value[2];} Args31;
void ov45_02246BA4(void*,Task*);
void ov45_02246BA4(void *context,Task*t){void *user;
if(!context)__msl_assertion_failed(ov45_022554F0,ov45_02255468,ov45_022553D8,909);
if(!t)__msl_assertion_failed(ov45_02255458,ov45_02255468,ov45_022553D8,911);
if(t->type<0)__msl_assertion_failed(ov45_02255478,ov45_02255468,ov45_022553D8,911);
if(t->type>=32)__msl_assertion_failed(ov45_02255488,ov45_02255468,ov45_022553D8,911);
if(!t->callback)__msl_assertion_failed(ov45_022554A4,ov45_02255468,ov45_022553D8,911);
if(!t->arguments)__msl_assertion_failed(ov45_022554BC,ov45_02255468,ov45_022553D8,911);
if(t->field<0)__msl_assertion_failed(ov45_022554DC,ov45_02255468,ov45_022553D8,911);
user=t->user;switch(t->type){
case 0:{Args0 *a=t->arguments;void(*f)(void*,u32,void*)=(void(*)(void*,u32,void*))t->callback;f(context,a->value[0],user);break;}
case 1:{Args1 *a=t->arguments;void(*f)(void*,u32,void*)=(void(*)(void*,u32,void*))t->callback;f(context,a->value[0],user);break;}
case 2:{Args2 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 3:{Args3 *a=t->arguments;void(*f)(void*,u32,u32,void*)=(void(*)(void*,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],user);break;}
case 4:{Args4 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 5:{Args5 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 6:{Args6 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 7:{Args7 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],user);break;}
case 8:{Args8 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 9:{Args9 *a=t->arguments;void(*f)(void*,u32,u32,void*)=(void(*)(void*,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],user);break;}
case 10:{Args10 *a=t->arguments;void(*f)(void*,u32,u32,void*)=(void(*)(void*,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],user);break;}
case 11:{Args11 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 12:{Args12 *a=t->arguments;void(*f)(void*,u32,void*)=(void(*)(void*,u32,void*))t->callback;f(context,a->value[0],user);break;}
case 13:{Args13 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],user);break;}
case 14:{Args14 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],user);break;}
case 15:{Args15 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;ov45_02248558(context,a->value[2]);f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 16:{Args16 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 17:{Args17 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 18:{Args18 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 19:{Args19 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],user);break;}
case 20:{Args20 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],a->value[5],a->value[6],user);break;}
case 21:{Args21 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 22:{Args22 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],user);break;}
case 23:{Args23 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 24:{Args24 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 25:{Args25 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 26:{Args26 *a=t->arguments;void(*f)(void*,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],user);break;}
case 27:{Args27 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 28:{Args28 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],user);break;}
case 29:{Args29 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],user);break;}
case 30:{Args30 *a=t->arguments;void(*f)(void*,u32,u32,u32,u32,u32,u32,void*)=(void(*)(void*,u32,u32,u32,u32,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],a->value[2],a->value[3],a->value[4],a->value[5],user);break;}
case 31:{Args31 *a=t->arguments;void(*f)(void*,u32,u32,void*)=(void(*)(void*,u32,u32,void*))t->callback;f(context,a->value[0],a->value[1],user);break;}
default:__msl_assertion_failed(ov45_022554EC,ov45_02255468,ov45_022553D8,0x5ef);break;}ov45_02244500(t);}
