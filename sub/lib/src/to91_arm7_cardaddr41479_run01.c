#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35247Param{u32 count;const u8 *source;u8 *destination;u32 verified;} W35247Param;
typedef struct {u32 result,type,id,source,destination,length;u32 capacity,sector,subsector,page,addressBytes;u32 timing[10];u8 initialStatus,padding[3];u32 caps,reserved;} AddressRequest;
typedef struct {AddressRequest *request;u8 remaining[0x1fc];} AddressCommon;
extern AddressCommon cardi_common;
void CARDi_CommWriteCore(W35247Param*);
void CARDi_CommArray(const void*,void*,u32,void(*)(W35247Param*));
void CARDi_SendSpiAddressingCommand(u32,u32);
void CARDi_SendSpiAddressingCommand(u32 address,u32 command){u32 packed;u32 bytes=cardi_common.request->addressBytes;switch(bytes){case 1:packed=command|((address>>5)&8)|((address&255)<<8);break;case 2:packed=command|(address&0xff00)|((address&255)<<16);break;case 3:packed=command|((address>>8)&0xff00)|((address&0xff00)<<8)|(address<<24);break;}CARDi_CommArray(&packed,0,bytes+1,CARDi_CommWriteCore);}
