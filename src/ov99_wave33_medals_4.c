#include "ov99_wave33_medals_private.h"

BOOL PokeathlonMedals_Exit(void *manager){R8Work *w=OverlayManager_GetData(manager);Heap_Free(w->alloc3f0);ov99_021E7AB8(w);PokeathlonUtil_FreeSpriteContext(w->unk404,&w->all[0],50);PokeathlonUtil_FreeTextContext(w->text,12);ov99_021E7EBC(w);PokeathlonUtil_DisableDisplay();OverlayManager_FreeData(manager);Heap_Destroy((enum HeapID)0x84);UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));return TRUE;}
