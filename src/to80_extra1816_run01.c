#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 words[10];}Banks;extern const Banks ov96_0221CF94;extern void GfGfx_SetBanks(Banks*);
void ov96_0220E868(void);
void ov96_0220E868(void){Banks b=ov96_0221CF94;GfGfx_SetBanks(&b);}
