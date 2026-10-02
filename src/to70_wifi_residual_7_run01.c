#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define S16(p,o) (*(volatile s16 *)((u8 *)(p)+(o)))
#define S8(p,o) (*(s8 *)((u8 *)(p)+(o)))
extern u8 *_02110628;
extern void CPS_TcpShutdown(void);
extern void CPS_TcpClose(void);
extern void CPS_SocRelease(void);
extern void CPS_SocUnRegister(void);
extern void SOCLi_SendCommandPacket(void *,void *);
extern void SOCLi_SocketUnregister(void *);
extern void SOCLi_SocketRegisterTrash(void *);
extern void SOCLi_FreeCommandPacket(void *);
extern int SOCLi_CloseCallBack(void *);
extern void SOCLi_FreeCommandPipe(void *);
extern int SOCL_SocketIsInTrash(void *);
extern int SOCL_SocketIsInvalid(void *);
extern void *SOCLi_CreateCommandPacket(int (*)(void *),void *,int);
extern void SOCLi_SendCommandPacketToCtrlPipe(void *,void *);
extern void SOCLi_SocketUnregisterTrash(void *);
extern int SOCL_Close(int);
extern void SOCLi_CleanupSocket(void *);
static inline BOOL isStreamSocket(void *sock) { int type=S8(sock,0x73); return type==0 || type==4; }
int SOCL_Close(int fd) { u8 *sock=(void *)fd; void *packet; int ready; int tcp; if(fd<=0) return -28; if(SOCL_SocketIsInTrash(sock)) return -26; if(SOCL_SocketIsInvalid(sock)) return 0; ready=0; if(sock) ready=(S16(sock,0x70)&1)!=0; if(!ready) return -39; if(S16(sock,0x70)&0x10) return -26; S16(sock,0x70)|=0x18; tcp=S8(sock,0x73)==0 || S8(sock,0x73)==4; if(tcp) SOCLi_SendCommandPacket((void *)U32(sock,0x68),NULL); packet=SOCLi_CreateCommandPacket(SOCLi_CloseCallBack,sock,1); U32(packet,8)=0; SOCLi_SendCommandPacketToCtrlPipe(sock,packet); return 0; }
int SOCLi_CloseCallBack(void *packet) { u8 *sock=(void *)U32(packet,4); OSIntrMode mode; int tcp=(S8(sock,0x73)==0 || S8(sock,0x73)==4); if(tcp) { OS_JoinThread((void *)((u8 *)U32(sock,0x68)+0x20)); CPS_TcpShutdown(); CPS_TcpClose(); CPS_SocRelease(); } CPS_SocUnRegister(); S16(sock,0x70)&=~6; SOCLi_SendCommandPacket(S8(sock,0x73)==2?(void *)U32(sock,0x68):(void *)U32(sock,0x64),NULL); mode=OS_DisableInterrupts(); SOCLi_SocketUnregister(sock); SOCLi_SocketRegisterTrash(sock); OS_RestoreInterrupts(mode); S16(sock,0x70)|=0x20; return 0; }
