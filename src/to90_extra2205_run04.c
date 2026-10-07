#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x670];void*sprites[1];} State;
extern void Sprite_DeleteAndFreeResources(void*);
void ov18_021F10E8(State*,int);
void ov18_021F10E8(State*s,int index){if(s->sprites[index]){Sprite_DeleteAndFreeResources(s->sprites[index]);s->sprites[index]=0;}}
