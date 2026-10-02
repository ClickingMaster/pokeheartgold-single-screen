#pragma force_active on
#include "global.h"
#include "pokemon.h"
#include "pokepic.h"
#pragma force_active on
typedef struct {u8 pad[0x2c];u16 species[30];u32 personality[30];u32 ot[30];u32 mask;u8 forms[84];int selected;u8 pad2[0x178];Pokepic *pic;} Data;
typedef struct {u8 pad[0x64];PokepicManager *manager;u8 pad2[0x7f8];Data *data;} Work;
extern int ov40_022371D4(u32,int);
void ov40_02237474(Work *);
void ov40_02237474(Work *w){Data *d=w->data;int selected=d->selected;u32 ot=d->ot[selected];int form=d->forms[selected];u16 species=d->species[selected];u32 personality=d->personality[selected];BASE_STATS *personal;u8 gender,shiny;PokepicTemplate template;if(species==0){d->pic=NULL;return;}if(ov40_022371D4(d->mask,1<<selected)==1){if(species==490)form=1;else form=0;species=494;}personal=AllocAndLoadMonPersonal(species,(enum HeapID)109);gender=GetGenderBySpeciesAndPersonality(species,personality);shiny=CalcShininessByOtIdAndPersonality(ot,personality);GetMonPicHeightBySpeciesGenderForm(species,gender,2,(u8)form,personality);GetMonSpriteCharAndPlttNarcIdsEx(&template,species,gender,2,shiny,(u8)form,personality);d->pic=PokepicManager_CreatePokepic(w->manager,&template,42,91,0,0,NULL,NULL);FreeMonPersonal(personal);}
