#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern char *mbc;extern u32 _02247480;
extern void MBi_MakeParentSendBuffer(void*,void*);extern int MBi_BlockHeaderEnd(int,u16,void*);
int MBi_CommParentSendMsg(u8,u16);void MBi_calc_sendblock(int);
typedef struct Cache{u32 position,size;void*destination;int state;} Cache;
typedef struct Owner{char pad[16];char name[4];u32 length;FSArchive*archive;} Owner;
typedef struct Request{char pad[16];Cache*cache;Owner*owner;} Request;
void MBi_ReloadCache(Request*);
extern int MBi_CommParentSendDLFileInfo(u16),MBi_CommParentSendBlock(void);
int MBi_CommParentSendData(void);
int MBi_CommParentSendMsg(u8 message,u16 mask){u8 buffer[8];buffer[0]=message;MBi_MakeParentSendBuffer(buffer,mbc);return MBi_BlockHeaderEnd(6,mask,mbc);}
