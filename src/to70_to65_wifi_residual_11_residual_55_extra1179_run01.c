#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 p[0x2260];int state;u8 q[7];u8 flag;} Work;
extern Work *WCMi_GetSystemWork(void);
extern OSMutex _021D5FFC; extern OSThreadQueue _021D5FF4;
extern struct {u8 p[0x24];int result;} _021D5FF0;
extern void WcmCpsifWmCallback(void*);
extern int WM_SetDCFData(void(*)(void*),const void*,const void*,u16);
int WCM_SendDCFDataEx(const void*,const void*,int,const void*,int);
int WCM_SendDCFDataEx(const void *dest,const void *data,int size,const void *extra,int size2){OSIntrMode mode;int total;Work *w;mode=OS_DisableInterrupts();if(!WCMi_GetSystemWork()){OS_RestoreInterrupts(mode);return -1;}OS_LockMutex(&_021D5FFC);w=WCMi_GetSystemWork();if(!w){OS_UnlockMutex(&_021D5FFC);OS_RestoreInterrupts(mode);return -1;}if(w->state!=9||w->flag==1){OS_UnlockMutex(&_021D5FFC);OS_RestoreInterrupts(mode);return -4;}MI_CpuCopy8(data,(u8*)w+0xf00,size);if(size2>0)MI_CpuCopy8(extra,(u8*)w+0xf00+size,size2);total=size+size2;switch(WM_SetDCFData(WcmCpsifWmCallback,dest,(u8*)w+0xf00,(u16)total)){case 2:break;case 0:case 1:case 3:case 4:case 5:case 6:case 7:case 8:default:OS_UnlockMutex(&_021D5FFC);OS_RestoreInterrupts(mode);return -5;}OS_SleepThread(&_021D5FF4);switch(_021D5FF0.result){case 0:break;case 1:default:OS_UnlockMutex(&_021D5FFC);OS_RestoreInterrupts(mode);return -5;}OS_UnlockMutex(&_021D5FFC);OS_RestoreInterrupts(mode);return total;}
