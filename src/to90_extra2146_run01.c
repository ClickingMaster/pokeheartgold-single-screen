#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{void*buffer;u16 channel,time;u8 address[6];} Scan;
typedef struct{u16 command,channel;void*buffer;u16 time;u8 address[6];} Command;
int WMi_CheckStateEx(int,...);void WMi_SetCallbackTable(int,void*);int WMi_SendCommandDirect(void*,int);int WM_StartScan(void*,Scan*);
int WM_StartScan(void*callback,Scan*scan){Command command;int result=WMi_CheckStateEx(3,2,3,5);if(result)return result;if(!scan)return 6;if(!scan->buffer)return 6;if(scan->channel<1||scan->channel>14)return 6;WMi_SetCallbackTable(10,callback);command.command=10;command.channel=scan->channel;command.buffer=scan->buffer;command.time=scan->time;command.address[0]=scan->address[0];command.address[1]=scan->address[1];command.address[2]=scan->address[2];command.address[3]=scan->address[3];command.address[4]=scan->address[4];command.address[5]=scan->address[5];result=WMi_SendCommandDirect(&command,16);if(result)return result;return 2;}
