#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Oam {u32 attr01;u16 attr2;u16 affine;} Oam;
typedef struct Node {struct Node *prev,*next;} Node;
typedef struct OamBlk {Node node;Oam *oam;u8 count;} OamBlk;
typedef struct Work {void *resources[3][4];Oam *objects[47],*extras[4];OamBlk *sprites[2];void *data[4],*one,*effect;u8 flags[12];} Work;
extern Work *ov13_0224F4BC;extern const u8 ov13_02244F4C[4],ov13_02244F50[4];
void ov13_0223FE98(OamBlk*,int,u32,u32);void ov13_022293F0(int,int);
typedef char CheckOam[(sizeof(Oam)==8)?1:-1];
typedef char CheckWork[(sizeof(Work)==296)?1:-1];
static inline void SetPalette(Oam *object,u32 palette) {object->attr01 &= ~0xc00u;object->attr2=(object->attr2&~0xf000u)|(palette<<12);}
void ov13_022293F0(int index,int mode) {
 if(index<0)return;
 if(index<47){Work *w=ov13_0224F4BC;SetPalette(w->objects[index],ov13_02244F4C[mode]);}
 else if(index-47<4){Work *w=ov13_0224F4BC;SetPalette(w->extras[index-47],ov13_02244F4C[mode]);}
 else{Work *w=ov13_0224F4BC;ov13_0223FE98(w->sprites[index-51],-1,0,ov13_02244F50[mode]);}
}
