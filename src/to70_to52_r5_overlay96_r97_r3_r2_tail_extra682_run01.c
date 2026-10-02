#pragma force_active on
#include "global.h"
typedef struct {unsigned a:9;unsigned limit:9;unsigned value:8;unsigned b:6;} Field;

void ov96_0220D2AC(Field*fields,u32 tick,int selected);

void ov96_0220D2AC(Field*fields,u32 tick,int selected){int i=0;u32 mod60=tick%60,mod30=tick%30;for(;i<3;i++,fields++){u16 value=fields->value,limit=fields->limit;if(tick!=0&&value<limit){if(i==selected){if(mod60==0)fields->value+=8;}else if(mod30==0)fields->value+=5;if(fields->value>limit)fields->value=limit;}}}
