#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on

typedef struct {void*buffer;u16 bufferSize,channel,dwell;u8 address[6];u16 type,length;u8 ssid[32];u16 matchLength;} Scan;
typedef struct {u16 command,channel;void*buffer;u16 bufferSize,dwell;u8 address[6];u16 type,length;u8 ssid[32];u16 matchLength;u8 unused_tail[4];} Command;
extern int WMi_CheckStateEx(int,...);
extern int WMi_SendCommandDirect(const void *,u32);
extern void WMi_SetCallbackTable(int,void*);
int WM_StartScanEx(void*,void*);
int WM_StartScanEx(void*callback,void*param){Scan*p=(Scan*)param;Command command;int result=WMi_CheckStateEx(3,2,3,5);if(result)return result;if(!p)return 6;if(!p->buffer)return 6;if(p->bufferSize>1024)return 6;if(p->length>32)return 6;if(p->type!=0&&p->type!=1&&p->type!=2&&p->type!=3)return 6;if((u16)(p->type+0xfffeu)<=1&&p->matchLength>32)return 6;WMi_SetCallbackTable(38,callback);command.command=38;command.channel=p->channel;command.buffer=p->buffer;command.bufferSize=p->bufferSize;command.dwell=p->dwell;MI_CpuCopy8(p->address,command.address,6);command.type=p->type;command.matchLength=p->matchLength;command.length=p->length;MI_CpuCopy8(p->ssid,command.ssid,32);result=WMi_SendCommandDirect(&command,60);if(!result)result=2;return result;}
