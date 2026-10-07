#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 x:12,y:12,touch:1,valid:2,reserved:5;} Raw;
typedef struct {u32 pad;void (*callback)(u32,u8,u8);TPData last;volatile u16 index;u16 pad12;TPData *buffer;u16 count;u8 pad1a[0x1c];u16 state,error,busy;} TouchWork;
extern TouchWork _021E36B0;
void TPi_TpCallback(u32 tag,u32 data,BOOL error);
void TPi_TpCallback(u32 tag,u32 data,BOOL error){
    u32 result;
    u16 response=(u16)data;
    u16 command=(response&0x7f00)>>8;
    if(error){
        _021E36B0.error |=1<<command;
        if(_021E36B0.callback)_021E36B0.callback(command,4,0);
        return;
    }
    if(command==16){
        Raw raw;TPData *point;
        _021E36B0.index++;
        if(_021E36B0.index>=_021E36B0.count)_021E36B0.index=0;
        ((u16*)&raw)[0]=*(volatile u16*)0x027fffaa;
        ((u16*)&raw)[1]=*(volatile u16*)0x027fffac;
        point=&_021E36B0.buffer[_021E36B0.index];
        point->x=raw.x;point->y=raw.y;point->touch=(u8)raw.touch;point->validity=(u8)raw.valid;
        if(_021E36B0.callback)_021E36B0.callback(command,0,(u8)_021E36B0.index);
        return;
    }
    if(!(data&0x1000000))return;
    switch(response&0xff){
    case 0:
        switch(command){
        case 0: {
            Raw raw;
            ((u16*)&raw)[0]=*(volatile u16*)0x027fffaa;
            ((u16*)&raw)[1]=*(volatile u16*)0x027fffac;
            _021E36B0.last.x=raw.x;_021E36B0.last.y=raw.y;_021E36B0.last.touch=(u8)raw.touch;_021E36B0.last.validity=(u8)raw.valid;
            _021E36B0.state=0;
        } break;
        case 1:_021E36B0.state=2;break;
        case 2:_021E36B0.state=0;break;
        }
        _021E36B0.busy &=~(1<<command);
        if(_021E36B0.callback)_021E36B0.callback(command,0,0);
        return;
    case 4: result=3;goto failed;
    case 2: result=1;goto failed;
    case 3: result=2;
    failed: {
        _021E36B0.error |=1<<command;
        _021E36B0.busy &=~(1<<command);
        if(_021E36B0.callback)_021E36B0.callback(command,(u8)result,0);
        return;
    }
    default:OS_Terminate();
    }
}
