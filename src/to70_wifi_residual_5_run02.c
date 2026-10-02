#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define S16(p,o) (*(s16 *)((u8 *)(p)+(o)))
#define S8(p,o) (*(s8 *)((u8 *)(p)+(o)))
extern int SOCLi_GetWriteBufferFreeSize(void *);
extern int SOCLi_AllocWriteBuffer(void *,int,int,int *,int);
extern int SOCLi_UdpRecvCallback(const void *,u32,void *);
extern u8 *_02110628;
extern u32 _021D4484[];
extern int SOCLi_ExecWriteCommand(void *,const void *,int,int,u32,u32,int);
extern int SOCLi_WriteBuffer(void *,const void *,int,u32,u32,int);
int SOCLi_AllocWriteBuffer(void *sock,int len,int minimum,int *offset,int flags) {
 u8 *pipe=(void *)U32(sock,0x68); int n; OSIntrMode mode; if(minimum>len) minimum=len; mode=OS_DisableInterrupts(); for(;;) { n=SOCLi_GetWriteBufferFreeSize(sock); if(n>=minimum) { *offset=U16(pipe,0x100); if(n>=len) n=len; break; } if(!(flags&1)) { n=0; break; } OS_SleepThread((void *)(pipe+0x104)); } OS_RestoreInterrupts(mode); return n;
}
