#include "global.h"
#include <string.h>
#include "field_overlay_particle_storage_private.h"
#pragma force_active on
u32 ov01_021F1224(u32,BOOL);
u32 ov01_021F1250(u32,BOOL);
u32 ov01_021F1224(u32 size,BOOL compressed){FieldOverlayParticleWork *owner;u32 old,next;if(!ov01_02209B64.unk0)GF_AssertFail();if(compressed)GF_AssertFail();owner=ov01_02209B64.unk0;old=owner->texOffset;next=old+size;owner->texOffset=next;return old;}
u32 ov01_021F1250(u32 size,BOOL fourPalette){FieldOverlayParticleWork *owner;u32 old,next;if(!ov01_02209B64.unk0)GF_AssertFail();if(fourPalette)GF_AssertFail();owner=ov01_02209B64.unk0;old=owner->plttOffset;next=old+size;owner->plttOffset=next;return old;}
