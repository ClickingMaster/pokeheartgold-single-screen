#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Entry {u16 a,b;void *nodes;} Entry;
typedef struct Bank {u16 count,pad;Entry *entries;void *nodes;u32 padc;void *extended;} Bank;
void UnpackExtendedData_(void*);void NNS_G2dUnpackNMC(Bank*);
void NNS_G2dUnpackNMC(Bank *bank){u16 i=0;bank->entries=(Entry*)((u32)bank->entries+(u32)bank);bank->nodes=(void*)((u32)bank->nodes+(u32)bank);Entry *entries=bank->entries;for(;i<bank->count;i++){entries[i].nodes=(u8*)bank->nodes+(u32)entries[i].nodes;}if(bank->extended){bank->extended=(void*)((u32)bank->extended+(u32)bank);UnpackExtendedData_(bank->extended);}}
