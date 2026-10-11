#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 x[52];} Template;
extern Template ov99_021E9840[];
typedef struct {void*arg;u32 pad[4];void*manager;void*sprites[26];} Work;
extern void* PokeathlonUtil_NewSprite(void*,Template*);
extern void ManagedSprite_SetAnimateFlag(void*,int);
extern void ManagedSprite_SetDrawFlag(void*,int);
extern void ManagedSprite_SetAnim(void*,int);
extern int ov99_021E5BB4(void*,int);
extern void* ov99_021E5B90(void*,int);
extern int ov98_0221EF64(void*);
void ov99_021E6438(Work*);


#pragma force_active on
void ov99_021E6438(Work*w){int i;s32 j;Template*t=ov99_021E9840;Work*second,*cursor;i=0;cursor=w;for(;i<26;i++){cursor->sprites[0]=PokeathlonUtil_NewSprite(w->manager,t);ManagedSprite_SetAnimateFlag(cursor->sprites[0],1);t++;cursor=(Work*)((u8*)cursor+4);}j=0;second=w;for(;j<5;j++){int type;if(!ov99_021E5BB4(w->arg,j))ManagedSprite_SetDrawFlag(second->sprites[1],0);type=ov98_0221EF64(ov99_021E5B90(w->arg,j));if(type==2){if(j%2==0)ManagedSprite_SetAnim(second->sprites[6],5);else ManagedSprite_SetAnim(second->sprites[6],6);}else if(type==1){if(j%2==0)ManagedSprite_SetAnim(second->sprites[6],2);else ManagedSprite_SetAnim(second->sprites[6],3);}else ManagedSprite_SetDrawFlag(second->sprites[6],0);second=(Work*)((u8*)second+4);}}
