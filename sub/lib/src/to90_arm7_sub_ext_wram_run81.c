#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u8 wmspW[];
typedef struct {u8 pad[0x54c];u8 *iface;u8 *state;} To90W35372Global;
#define G ((To90W35372Global *)(wmspW+0x1000))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern void __VENEER_PM_SetLEDPattern(u32);
typedef struct {u16 next,port,dest,remaining,sent,retry,pad,len,sequence,retryNumber;u32 data,callback,arg;} To90W35372Entry;
typedef struct {u16 head,tail;} To90W35372Pair;
typedef struct {u8 pad[0x2f8];To90W35372Entry entries[32];To90W35372Pair free,active[4],pending[4];u8 mutex[24];} To90W35372State;
extern void __VENEER_OS_LockMutex(void *);
extern void __VENEER_OS_UnlockMutex(void *);
extern void __VENEER_MIi_CpuClear16(u32,void *,u32);
void WMSP_InitSendQueue(void);
// @fn WMSP_InitSendQueue
void WMSP_InitSendQueue(void) {
 To90W35372State *s=(To90W35372State *)G->state;u16 i;
 __VENEER_OS_LockMutex(s->mutex);__VENEER_MIi_CpuClear16(0,s->entries,0x400);
 for(i=0;i<31;i++)s->entries[i].next=i+1;
 s->entries[i].next=0xffff;s->free.head=0;s->free.tail=i;
 for(i=0;i<4;i++){s->pending[i].head=0xffff;s->pending[i].tail=0xffff;s->active[i].head=0xffff;s->active[i].tail=0xffff;}
 __VENEER_OS_UnlockMutex(s->mutex);
}
