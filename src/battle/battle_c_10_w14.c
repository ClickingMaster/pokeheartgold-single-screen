#include "global.h"
#include "constants/heap.h"
#include "heap.h"
#include "sys_task_api.h"
#include "sound.h"

#define U8(p, n) (((u8 *)(p))[n])
#define U16(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define U32(p, n) (*(u32 *)((u8 *)(p) + (n)))
typedef struct BattleSystem BattleSystem;
void ov12_0226430C(BattleSystem *, int, int);
void ov12_0225FF80(SysTask *, void *); void ov12_0226037C(SysTask *, void *);
void ov12_02260584(SysTask *, void *); void ov12_022609F8(SysTask *, void *);
void ov12_02260B30(SysTask *, void *); void ov12_02260D84(SysTask *, void *);
void ov12_02261CA8(BattleSystem *, void *, void *, int);
void ov07_0223494C(void *, int); void ov07_02234A20(void *, int);
void ov12_0225A9B0(BattleSystem *, void *); void ov12_0225ABE8(BattleSystem *, void *);
void ov12_0225AEA0(BattleSystem *, void *); void ov12_0225B028(BattleSystem *, void *, void *);
void ov12_0225B060(BattleSystem *, void *, void *); void ov12_0225B200(BattleSystem *, void *, void *);
void ov12_0225B3A0(BattleSystem *, void *, void *); void ov12_0225B434(BattleSystem *, void *, void *);
void ov12_0225B454(BattleSystem *, void *, void *);

void ov12_0225B028(BattleSystem *b, void *c, void *m) { void *w=Heap_Alloc(HEAP_ID_BATTLE,16); U8(w,10)=0; U32(w,0)=(u32)b; U8(w,8)=U8(m,0); U8(w,9)=U8(c,0x194); U16(w,12)=U16(m,2); U8(w,14)=U8(m,1); SysTask_CreateOnMainQueue(ov12_022609F8,w,0); }

void ov12_0225B060(BattleSystem *b, void *c, void *m) { void *w=Heap_Alloc(HEAP_ID_BATTLE,16); U8(w,10)=0; U32(w,0)=(u32)b; U32(w,4)=U32(c,0x20); U8(w,8)=U8(m,0); U8(w,9)=U8(c,0x194); U8(w,11)=U8(m,1); U8(w,12)=0; U8(w,13)=U8(m,2); SysTask_CreateOnMainQueue(ov12_02260B30,w,0); }
