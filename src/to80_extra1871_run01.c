#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0xfc];void*manager;} Work;
extern void ov01_021FA1E4(Work*);
extern int* ov01_021FA204(Work*);
extern int ov01_021FA21C(Work*),ov01_021FA224(Work*);
extern int ov01_021FA094(void*,int,int);
extern void ov01_021FC588(void*,int);
void ov01_021F9CF8(void*,Work*);


#pragma force_active on
void ov01_021F9CF8(void*a,Work*w){int*base;int count;int*cursor;ov01_021FA1E4(w);base=ov01_021FA204(w);cursor=base+ov01_021FA21C(w);count=ov01_021FA224(w);do{if(*cursor!=255 && ov01_021FA094(a,*cursor,0)==0){ov01_021FC588(w->manager,*cursor);*cursor=255;}cursor++;count--;}while(count);}
