#pragma force_active on
#include "global.h"
void ov96_0221A400(void*);void ov96_02219FE4(u8*);
void ov96_02219FE4(u8*p){int i=0;if(i<p[44]){u8*entry=p+8;do{ov96_0221A400(entry);i++;entry+=12;}while(i<p[44]);}}
