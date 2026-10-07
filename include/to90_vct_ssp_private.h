#ifndef TO90_VCT_SSP_PRIVATE_H
#define TO90_VCT_SSP_PRIVATE_H
#include "global.h"
typedef struct Session {u32 state;u8 aid,other,pad[2];u32 mask,mode,pad2;struct Session *next;} Session;
typedef void (*VctCallback)(u32,u32,Session*,void*);
typedef struct Vct {u8 aid,peer,pad[2];VctCallback callback;void *context;s32 state,mode,flag;} Vct;
typedef struct Ssp {u32 count,mask;u64 tick;Session *free,*used;u64 (*clock)(void);u32 seed,add;} Ssp;
typedef struct Control {u32 frame,received,active;Vct vct;} Control;
typedef struct InitArgs {Session *data;u32 count;s32 mode;u8 id,pad[3];void *buffer;u32 size;VctCallback callback;void *arg;} InitArgs;
typedef char VctSessionExtent[(sizeof(Session)==24)?1:-1];
typedef char VctExtent[(sizeof(Vct)==24)?1:-1];
typedef struct SspBank {Ssp core;Session reject,server;} SspBank;
extern SspBank _021D6F90;extern Session _021D6FB4,_021D6FCC;extern Vct __vct;extern Control _021D848C;
s32 vct_init_ssp(InitArgs*);
s32 VCT_Init(InitArgs*);
#endif
