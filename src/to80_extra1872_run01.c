#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x29];u8 mode;} Arg;typedef struct{Arg *arg;u8 p[0x194];void *sprites[6][5];} Work;typedef struct{u8 x,y;} Pos;extern Pos ov05_0221EC5C[6][5];extern void ManagedSprite_SetPositionXY(void *,s16,s16);void ov05_0221E564(Work *,int);
#pragma force_active on
void ov05_0221E564(Work *w,int shift){int i,first,j;Pos *pos;if(!w->arg->mode){first=3;shift=(s16)(shift*8);}else{first=0;shift=(s16)(-(shift*8));}i=0;pos=ov05_0221EC5C[first];for(;i<3;i++){for(j=0;j<5;j++)ManagedSprite_SetPositionXY(((Work *)((u32)w+(((first+i)*5)<<2)))->sprites[0][j],shift+pos[j].x,pos[j].y);pos+=5;}}
