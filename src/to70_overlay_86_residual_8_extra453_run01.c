#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Group{u16*items;u32 count;}Group;typedef struct W{u8 pad[6];u8 mode;u8 gap[541];void*save;u8 tail[56];Group groups[26];}W;extern int GetPokedexDataNarcID(void),sub_0205C144(u8),sub_020312E0(void*,void*,int,u16);extern void*GfGfxLoader_LoadFromNarc_GetSizeOut(int,int,int,int,int,u32*),*sub_020312C4(void*,int,int*),*Heap_Alloc(int,u32);extern void Heap_Free(void*);u16*ov86_021E73EC(int,u32*);void ov86_021E7418(W*);
u16*ov86_021E73EC(int member,u32*count){u32 size;u16*data=GfGfxLoader_LoadFromNarc_GetSizeOut(GetPokedexDataNarcID(),member+18,0,121,1,&size);*count=size>>1;return data;}
void ov86_021E7418(W*w){int enabled;u32 count;void*flags=sub_020312C4(w->save,11,&enabled);u16 index;if(enabled==1){for(index=0;index<26;index++){u16*data=ov86_021E73EC(index,&count);u16 i;W*view=(W*)((u8*)w+8*index);view->groups[0].items=Heap_Alloc(121,count*2);for(i=0;i<count;i++){if(sub_020312E0(w->save,flags,sub_0205C144(w->mode),data[i])){view->groups[0].items[view->groups[0].count]=data[i];view->groups[0].count++;}}Heap_Free(data);}}if(flags)Heap_Free(flags);}
