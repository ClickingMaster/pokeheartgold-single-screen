#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
typedef struct {unsigned char lo:4,hi:4;} Toggle;
extern void ManagedSprite_SetDrawFlag(void*,int),ov18_021F21FC(void*,int,u16);
extern int Pokedex_GetSeenFormByIdx(void*,int,int),GetMonBaseStat_HandleAlternateForm(int,int,int);
void ov18_021F209C(void*,int,int,int);
#pragma opt_common_subs off
#pragma force_active on
void ov18_021F209C(void *w,int species,int position,int index){if(B(w,0x185c)!=2||species==0||H((u8*)w+position*4,0x1032)==1){index*=4;ManagedSprite_SetDrawFlag(P((u8*)w+index,0x670),0);ManagedSprite_SetDrawFlag(P((u8*)w+index,0x674),0);ManagedSprite_SetDrawFlag(P((u8*)w+index,0x678),0);ManagedSprite_SetDrawFlag(P((u8*)w+index,0x67c),0);return;}
 int initialOffset; if(((Toggle*)((u8*)w+0x185f))->hi==0){initialOffset=index*4;ManagedSprite_SetDrawFlag(P((u8*)w+initialOffset,0x670),0);ManagedSprite_SetDrawFlag(P((u8*)w+initialOffset,0x674),0);index+=2;}else{initialOffset=index*4;ManagedSprite_SetDrawFlag(P((u8*)w+initialOffset,0x678),0);ManagedSprite_SetDrawFlag(P((u8*)w+initialOffset,0x67c),0);}u8 next=((Toggle*)((u8*)w+0x185f))->hi^1;((Toggle*)((u8*)w+0x185f))->hi=next;
 int form=Pokedex_GetSeenFormByIdx(P(P(w,0),0),species,0);if(species==172){if(form==2)form=1;else form=0;}
 u16 ability1;int offset;ability1=GetMonBaseStat_HandleAlternateForm(species,form,6);ov18_021F21FC(w,index,ability1);offset=index*4;ManagedSprite_SetDrawFlag(P((u8*)w+offset,0x670),1);
 u16 ability2=GetMonBaseStat_HandleAlternateForm(species,form,7);if(ability2==0||ability1==ability2){ManagedSprite_SetDrawFlag(P((u8*)w+offset,0x674),0);}else{ov18_021F21FC(w,index+1,ability2);ManagedSprite_SetDrawFlag(P((u8*)w+offset,0x674),1);}
}
