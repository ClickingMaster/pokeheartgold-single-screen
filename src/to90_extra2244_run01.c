#pragma force_active on
#include "global.h"
#pragma force_active on
#include "bg_window.h"
typedef struct{u8 pad[12];Window windows[101];} Work;
void PokedexApp_RemoveAllWindows(Work*);
void PokedexApp_RemoveAllWindows(Work *w){u32 i;for(i=0;i<101;i++){if(w->windows[i].pixelBuffer)RemoveWindow(&w->windows[i]);}}
