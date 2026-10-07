// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 bytes[0x1020];int status;u8 gap[0x1b38-0x1024];OSThread thread;u8 tail[0x1c14-0x1b38-sizeof(OSThread)];}Request;typedef struct{u8 bytes[0x1004];int active,error;u8 gap[0x1314-0x100c];Request*request;u8 tail[0x13d8-0x1318];OSMutex mutex;int cancel;}Context;typedef struct{int state,pad;Context*context;}Global;extern Global ov00_0221A694;void ov00_021E8940(int),ov00_021E9754(void*);int ov00_021E82FC(void),ov00_021E7F38(int);
#include <stddef.h>
typedef char W41005RequestExtent[(sizeof(Request)==0x1c14)?1:-1];
typedef char W41005ThreadOffset[(offsetof(Request,thread)==0x1b38)?1:-1];
typedef char W41005LiveOffset[(offsetof(Request,thread)+offsetof(OSThread,id)==0x1ba4)?1:-1];
typedef char W41005MutexOffset[(offsetof(Context,mutex)==0x13d8)?1:-1];
typedef char W41005CancelOffset[(offsetof(Context,cancel)==0x13f0)?1:-1];
void ov00_021E8048(void);
void ov00_021E8048(void){int retries=0;for(;;){int restart,status;OSTick start;if(ov00_0221A694.context->request->thread.id)OS_JoinThread(&ov00_0221A694.context->request->thread);if(ov00_0221A694.context->request->status!=8){ov00_0221A694.context->error=20100;status=ov00_0221A694.context->request->status;if(status==7){ov00_021E8940(20);return;}if(retries>2){if(status==2){ov00_021E8940(9);return;}if(status==3){ov00_021E8940(11);return;}ov00_021E8940(13);return;}restart=1;retries++;}else{status=ov00_021E82FC();switch(status){case 21:ov00_021E8940(21);return;case 16:if(retries>2){ov00_021E8940(16);return;}restart=0;retries++;break;case 17:ov00_021E8940(status);return;default:if(retries>=2){ov00_021E8940(status);return;}restart=1;retries++;break;}}start=OS_GetTick();while(((OS_GetTick()-start)<<6)/33514<5000){OS_LockMutex(&ov00_0221A694.context->mutex);if(ov00_0221A694.context->cancel==1){ov00_0221A694.context->error=20100;OS_UnlockMutex(&ov00_0221A694.context->mutex);ov00_021E8940(20);return;}OS_UnlockMutex(&ov00_0221A694.context->mutex);OS_Sleep(5000);}ov00_021E9754(ov00_0221A694.context->request);OS_LockMutex(&ov00_0221A694.context->mutex);ov00_0221A694.context->active=ov00_021E7F38(restart);if(ov00_0221A694.context->active){ov00_0221A694.context->error=20100;OS_UnlockMutex(&ov00_0221A694.context->mutex);return;}OS_UnlockMutex(&ov00_0221A694.context->mutex);continue;}}
