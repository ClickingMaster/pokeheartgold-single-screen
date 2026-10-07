#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u16 valid,received;u8 data[1];} Shared;
extern void *WmGetSharedDataAddress(void*,u32,void*,int);
void *WM_GetSharedDataAddress(void*,Shared*,int);
#pragma opt_propagation off
void *WM_GetSharedDataAddress(void *context,Shared *shared,int id){u32 valid=shared->valid,received=shared->received;int mask=1;if(!context)return 0;if(!shared)return 0;if(!(valid&(mask<<id)))return 0;if(!(received&(mask<<id)))return 0;return WmGetSharedDataAddress(context,valid,shared->data,id);}
