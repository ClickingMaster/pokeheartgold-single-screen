#include "overlay99_event_helpers_private.h"

BOOL PokeathlonEventRecord_Exit(void *manager,int *state){u8 *w=OverlayManager_GetData(manager);ov99_021E9418(*(void **)w);PokeathlonUtil_FreeSpriteContext(*(void **)(w+0x14),w+0x18,0x25);PokeathlonUtil_FreeTextContext(*(void **)(w+0x10),0x13);ov99_021E875C(w);PokeathlonUtil_DisableDisplay();OverlayManager_FreeData(manager);Heap_Destroy(0x84);UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));return TRUE;}
