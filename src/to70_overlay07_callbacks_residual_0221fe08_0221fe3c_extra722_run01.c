#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0xc];void*mgr;u32 unused;u32 ids[4];void*sprites[4];}Work;
extern void SpriteManager_UnloadCharObjById(void*,u32),SpriteManager_UnloadPlttObjById(void*,u32),Sprite_DeleteAndFreeResources(void*),Heap_Free(void*);
void ov07_0221FE08(Work*);void ov07_0221FE3C(Work*);
void ov07_0221FE08(Work*w){int i;for(i=0;i<4;i++){if(w->sprites[i]){SpriteManager_UnloadCharObjById(w->mgr,w->ids[i]);SpriteManager_UnloadPlttObjById(w->mgr,w->ids[i]);Sprite_DeleteAndFreeResources(w->sprites[i]);}}Heap_Free(w);}
void ov07_0221FE3C(Work*w){int i;for(i=0;i<4;i++){if(w->sprites[i]){SpriteManager_UnloadCharObjById(w->mgr,w->ids[i]);SpriteManager_UnloadPlttObjById(w->mgr,w->ids[i]);Sprite_DeleteAndFreeResources(w->sprites[i]);}}Heap_Free(w);}
