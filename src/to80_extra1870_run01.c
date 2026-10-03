#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0xf8];void*manager;} Work;
extern void ov01_021FA1DC(Work*);
extern int* ov01_021FA200(Work*);
extern int ov01_021FA20C(Work*),ov01_021FA214(Work*);
extern int ov01_021FA01C(void*,int,int);
extern void ov01_021FC588(void*,int);
void ov01_021F9BD4(void*,Work*);


#pragma force_active on
void ov01_021F9BD4(void*a,Work*w){int*base;int count;int*cursor;ov01_021FA1DC(w);base=ov01_021FA200(w);cursor=base+ov01_021FA20C(w);count=ov01_021FA214(w);do{if(*cursor!=255 && ov01_021FA01C(a,*cursor,0)==0){ov01_021FC588(w->manager,*cursor);*cursor=255;}cursor++;count--;}while(count);}
