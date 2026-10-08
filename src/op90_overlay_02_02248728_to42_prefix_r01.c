#pragma force_active on
#include "global.h"

#include "field_metatile_query_internal.h"
#include "field_system.h"
#include "heap.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "safari_zone.h"
#include "unk_0205FD20.h"
#include "unk_02097268.h"

int ov02_0224E31C(s32 x, s32 z);
BOOL ov02_0224E828(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state);
BOOL ov02_0224EB48(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state);
BOOL ov02_0224EE4C(FieldSystem *fieldSystem, s16 x, s16 z, u8 direction, fx32 y, s16 *outX, s16 *outZ, u8 state);
SafariDecorationArgs *SafariDecoration_CreateArgs(FieldSystem *fieldSystem, enum HeapID heapID);
// @fn SafariDecoration_CreateArgs
SafariDecorationArgs *SafariDecoration_CreateArgs(FieldSystem *f,enum HeapID heap){SafariDecorationArgs*a=Heap_AllocAtEnd(heap,36);
    s16 x;
    s16 z;
    u8 state;
    u8 direction;
    s16 outputA,outputB;
    u8 selector;
    VecFx32 pos;
MI_CpuFill8(a,0,36);a->saveData=FieldSystem_GetSaveData(f);a->unk04=(BOOL*)((u8*)f+0x10c);a->unk20=(u8*)f+0x111;state=(u8)(PlayerAvatar_GetState(f->playerAvatar)==2?1:0);a->unk18=state;direction=(u8)PlayerAvatar_GetFacingDirection(f->playerAvatar);x=(s16)PlayerAvatar_GetXCoord(f->playerAvatar);z=(s16)PlayerAvatar_GetZCoord(f->playerAvatar);PlayerAvatar_CopyPositionVector(f->playerAvatar,&pos);a->unk19=(u8)ov02_0224E31C(x,z);pos.y=sub_02054774(f,pos.y,pos.x,pos.z,&selector);if(selector!=1)return a;a->unk1A=(u8)(pos.y>>12);a->deltaX=(u8)(x+GetDeltaXByFacingDirection(direction));a->deltaY=(u8)(z+GetDeltaYByFacingDirection(direction));a->unk08=1;if(ov02_0224E828(f,x,z,direction,pos.y,&outputA,&outputB,state)){a->unk0D=(u8)outputA;a->unk0F=(u8)outputB;a->unk0C=1;}if(ov02_0224EB48(f,x,z,direction,pos.y,&outputA,&outputB,state)){a->unk11=(u8)outputA;a->unk13=(u8)outputB;a->unk10=1;}if(ov02_0224EE4C(f,x,z,direction,pos.y,&outputA,&outputB,state)){a->unk15=(u8)outputA;a->unk17=(u8)outputB;a->unk14=1;}return a;}
