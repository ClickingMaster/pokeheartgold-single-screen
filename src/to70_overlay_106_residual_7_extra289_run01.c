#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[0x10];void*sprites[1];}W;extern void Sprite_DeleteAndFreeResources(void*);void ov106_021E6634(W*,int);
void ov106_021E6634(W*w,int i){if(w->sprites[i]){Sprite_DeleteAndFreeResources(w->sprites[i]);w->sprites[i]=NULL;}}
