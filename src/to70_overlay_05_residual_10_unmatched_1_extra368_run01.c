#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 words[6];} Settings;
typedef struct {u8 data[28];} Record;
typedef struct {u8 pad[0x198];void *objects[12];u8 pad2[0x48];int count;} Work;
extern const Settings ov05_0221EB04;extern const Record ov05_0221EDA4[12];
extern void ov05_0221CD24(Work *,void *,int);extern void ov05_0221D318(Work *);extern void *ov05_0221CDC4(Work *,const void *);extern void ov05_0221D3AC(Work *,int,int,int);extern void ov05_0221D4D0(Work *);extern void ov05_0221D5DC(Work *);extern void ov05_0221D664(Work *,int,int,int);
void ov05_0221D240(Work *);
void ov05_0221D240(Work *w){Settings settings=ov05_0221EB04;u32 i;ov05_0221CD24(w,&settings,21);ov05_0221D318(w);for(i=0;i<12;i++)w->objects[i]=ov05_0221CDC4(w,&ov05_0221EDA4[i]);ov05_0221D3AC(w,0,-256,0);ov05_0221D3AC(w,1,-256,0);ov05_0221D3AC(w,2,256,0);ov05_0221D3AC(w,3,256,0);ov05_0221D4D0(w);w->count=12;ov05_0221D5DC(w);ov05_0221D664(w,0,-256,0);ov05_0221D664(w,1,-256,0);ov05_0221D664(w,2,256,0);ov05_0221D664(w,3,256,0);}
