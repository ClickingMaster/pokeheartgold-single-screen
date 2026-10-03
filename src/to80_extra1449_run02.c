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
void MBi_ReloadCache(Request*request){Owner*owner=request->owner;Cache*c=request->cache;FSArchive*archive=FS_FindArchive(owner->name,owner->length);FSFile file;if(!archive)archive=owner->archive;FS_InitFile(&file);if(FS_OpenFileDirect(&file,archive,c->position,c->position+c->size,(u32)-1)){if(FS_ReadFile(&file,c->destination,c->size)==c->size)c->state=2;FS_CloseFile(&file);}if(c->state==2)return;c->position=0;c->state=2;}
