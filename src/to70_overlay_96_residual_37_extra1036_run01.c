#pragma force_active on
#include "global.h"
typedef struct{u8 data[28];}Entry;extern void ov96_021ECD24(Entry*);extern int ov96_021ECDC4(Entry*);

void ov96_021ECBFC(Entry*p);
int ov96_021ECC14(Entry*p);

void ov96_021ECBFC(Entry*p){int i;for(i=0;i<4;i++)ov96_021ECD24(&p[i]);}
int ov96_021ECC14(Entry*p){int i;int result=1;for(i=0;i<4;i++){if(ov96_021ECDC4(&p[i])==0)result=0;}return result;}
