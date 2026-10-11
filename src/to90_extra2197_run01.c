#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "heap.h"
#pragma force_active on
typedef struct{u32 words[4];} Parameters;
typedef struct{u16 narc,file,other[6];} Picture;
extern const Parameters ov18_021FA338;extern const u16 ov18_021FB5B4[];
u32 Pokedex_GetSeenSpindaPersonality(void*,BOOL);void GetMonSpriteCharAndPlttNarcIdsEx(Picture*,u16,u8,u8,u8,u8,u32);void sub_02014510(u16,int,int,Parameters*,void*,u32,BOOL,int,int);void PokedexApp_LoadMonPicToBg(void*,u16,u8,u8,u8);
void PokedexApp_LoadMonPicToBg(void*context,u16 species,u8 form,u8 gender,u8 facing){u32*c=context;Picture picture;Parameters parameters=ov18_021FA338;u8*buffer=Heap_AllocAtEnd((enum HeapID)37,3200);u32 personality;u8*converted;if(species==327)personality=Pokedex_GetSeenSpindaPersonality(*(void**)c[0],0);else personality=0;GetMonSpriteCharAndPlttNarcIdsEx(&picture,species,gender,facing,0,form,personality);sub_02014510(picture.narc,picture.file,37,&parameters,buffer,personality,0,2,species);converted=Convert4bppTo8bpp(buffer,3200,15,(enum HeapID)37);BG_LoadCharTilesData((BgConfig*)c[1],7,converted,6400,1);Heap_Free(converted);Heap_Free(buffer);LoadRectToBgTilemapRect((BgConfig*)c[1],7,ov18_021FB5B4,11,11,10,10);ScheduleBgTilemapBufferTransfer((BgConfig*)c[1],7);}
