#pragma force_active on
#include "global.h"
#include "pokemon_types_def.h"
extern void GetMonSpriteCharAndPlttNarcIdsEx(PokepicTemplate*,u16,u8,u8,u8,u8,u32),sub_02014540(u16,int,int,void*,u32,int,int,int),Heap_Free(void*);
typedef struct{u32 personality;u16 species;u8 shiny,form,gender;}Info;
typedef struct{int heap;u8 p[0x254];void*chars;u8 palette[32];}Work;
void ov97_0221FBDC(Work*,Info*);
void ov97_0221FBDC(Work*w,Info*i){PokepicTemplate pic;NNSG2dPaletteData*palette;void*buffer;GetMonSpriteCharAndPlttNarcIdsEx(&pic,i->species,i->gender,2,i->shiny,i->form,i->personality);sub_02014540(pic.narcID,pic.charDataID,w->heap,w->chars,i->personality,0,2,i->species);buffer=AllocAndReadWholeNarcMemberByIdPair((NarcId)pic.narcID,pic.palDataID,(enum HeapID)w->heap);if(NNS_G2dGetUnpackedPaletteData(buffer,&palette)!=1)GF_AssertFail();MIi_CpuCopy16(palette->pRawData,(u16*)w->palette,32);Heap_Free(buffer);}
