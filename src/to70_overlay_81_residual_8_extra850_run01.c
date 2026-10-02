#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x1a8];void*manager;void*pics[4];}Work;
extern void*ov81_02242C80(void*,int,void*,int,int,int);extern void Pokepic_SetAttr(void*,int,int);extern u32 GetMonData(void*,int,void*);extern int GetMonBaseStat_HandleAlternateForm(u32,u32,int);extern void ov81_02242CB0(void*,int);void ov81_02240F48(Work*,int,void*,int,int,int);
void ov81_02240F48(Work*w,int i,void*mon,int arg,int extra,int last){u32 species;w->pics[i]=ov81_02242C80(w->manager,0,mon,arg,extra,0);Pokepic_SetAttr(w->pics[i],6,0);species=GetMonData(mon,5,0);if(GetMonBaseStat_HandleAlternateForm(species,GetMonData(mon,112,0),28)==0)ov81_02242CB0(w->pics[i],last);}
