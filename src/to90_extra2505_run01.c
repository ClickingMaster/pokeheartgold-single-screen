#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
#include "sprite_system.h"
extern int sub_02074490(void);
void ov96_02209A14(void*);
#pragma force_active on
typedef struct {u8 pad[8];SpriteSystem *system;SpriteManager *manager;u8 pad2[0x214-16];u16 palette;} State;
void ov96_02209A14(void*w){State*s=w;SpriteSystem*system;SpriteManager*manager;manager=s->manager;system=s->system;NNS_G2D_VRAM_TYPE screen=NNS_G2D_VRAM_TYPE_2DMAIN;
 SpriteSystem_LoadCharResObj(system,manager,(NarcId)234,8,TRUE,screen,10000);SpriteSystem_LoadPlttResObj(system,manager,(NarcId)234,7,FALSE,2,screen,10000);
 SpriteSystem_LoadCellResObj(system,manager,(NarcId)234,9,1,10000);SpriteSystem_LoadAnimResObj(system,manager,(NarcId)234,10,1,10000);
 screen=NNS_G2D_VRAM_TYPE_2DSUB;SpriteSystem_LoadCharResObj(system,manager,(NarcId)234,12,TRUE,screen,10001);SpriteSystem_LoadPlttResObj(system,manager,(NarcId)234,11,FALSE,3,screen,10001);
 SpriteSystem_LoadCellResObj(system,manager,(NarcId)234,13,1,10001);SpriteSystem_LoadAnimResObj(system,manager,(NarcId)234,14,1,10001);
 s->palette=SpriteSystem_LoadPlttResObj(system,manager,(NarcId)20,sub_02074490(),FALSE,3,screen,10002);
}
