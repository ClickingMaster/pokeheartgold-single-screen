#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x2fc];void *sprites[1];}Work;
extern void ManagedSprite_SetDrawFlag(void*,int);
extern int ManagedSprite_GetDrawFlag(void*);

void PCBox_SetSpriteVisible(Work*w,int index,int state);
int PCBox_IsSpriteVisible(Work*w,int index);

void PCBox_SetSpriteVisible(Work*w,int index,int state){if(state==1)ManagedSprite_SetDrawFlag(w->sprites[index],1);else ManagedSprite_SetDrawFlag(w->sprites[index],0);}
int PCBox_IsSpriteVisible(Work*w,int index){if(ManagedSprite_GetDrawFlag(w->sprites[index])==1)return 1;return 0;}
