#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u16 ov40_02245CD4[];
u16 *ov40_0222DD68(int,int,int *);
void *Save_Pokedex_Get(void *);
int Pokedex_CheckMonSeenFlag(void *,int);
void Heap_Free(void *);
int ov40_02235DAC(u8 *,int);
int ov40_02235DAC(u8 *w,int idx){
 int found=0;
 int end=ov40_02245CD4[idx+1];
 int start=ov40_02245CD4[idx];
 int size,i;
 u16 *list=ov40_0222DD68(109,found,&size);
 void *dex=Save_Pokedex_Get(*(void **)(w+0x830));
 for(i=start;i<end;i++)if(!Pokedex_CheckMonSeenFlag(dex,list[i]))list[i]=0xffff;
 for(;start<end;start++)if(list[start]!=0xffff){found=1;break;}
 Heap_Free(list);return found;
}
