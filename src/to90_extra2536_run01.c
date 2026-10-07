#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u8 *shared,*fifo,*reserved,*receive,*transmit;u16 dma;u8 reserved2[0xb6];u32 callbacks[16],ports[16],busy;u16 ready;}WmBuffer;
typedef struct {u16 initialized,pad;WmBuffer *buffer;} WmState;
extern WmState _021E4220;extern OSMessageQueue _021E4228;extern OSMessage _021E4248[];typedef struct{u16 flag;u8 data[254];}Packet;
extern Packet _021E42C0[];
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
extern void WmClearFifoRecvFlag(void),WmReceiveFifo(PXIFifoTag,u32,BOOL);
int WmInitCore(WmBuffer*,u32,u32);

#pragma opt_rotateloops off
#pragma opt_strength_reduction off
int WmInitCore(WmBuffer*buffer,u32 dma,u32 size){OSIntrMode flags=OS_DisableInterrupts();int i;if(_021E4220.initialized){OS_RestoreInterrupts(flags);return 3;}if(!buffer){OS_RestoreInterrupts(flags);return 6;}if(dma>3){OS_RestoreInterrupts(flags);return 6;}if((u32)buffer&31){OS_RestoreInterrupts(flags);return 6;}PXI_Init();if(!PXI_IsCallbackReady((PXIFifoTag)10,PXI_PROC_ARM7)){OS_RestoreInterrupts(flags);return 4;}DC_InvalidateRange(buffer,size);MI_DmaFill32(dma,buffer,0,size);_021E4220.buffer=buffer;buffer->shared=(u8*)buffer+0x200;_021E4220.buffer->fifo=_021E4220.buffer->shared+0x300;_021E4220.buffer->receive=_021E4220.buffer->fifo+0x800;_021E4220.buffer->transmit=_021E4220.buffer->receive+0x100;WmClearFifoRecvFlag();_021E4220.buffer->dma=dma;_021E4220.buffer->busy=0;_021E4220.buffer->ready=0;for(i=0;i<16;i++){_021E4220.buffer->callbacks[i]=0;_021E4220.buffer->ports[i]=0;}OS_InitMessageQueue(&_021E4228,_021E4248,10);int j;for(j=0;j<10;j++){_021E42C0[j].flag=0x8000;DC_StoreRange(&_021E42C0[j],2);OS_SendMessage(&_021E4228,&_021E42C0[j],OS_MESSAGE_BLOCK);}PXI_SetFifoRecvCallback((PXIFifoTag)10,WmReceiveFifo);_021E4220.initialized=1;OS_RestoreInterrupts(flags);return 0;}
