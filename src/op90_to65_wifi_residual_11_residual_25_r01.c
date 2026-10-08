// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{char*name;} CA;typedef struct {u8 pad[0x814];CA**roots;int count;} Work;CA*get_rootCA(Work*,const char*);
CA*gr4(Work*w,const char*name);
// @fn get_rootCA
CA*get_rootCA(Work*w,const char*name){int i;CA**roots;int count;count=w->count;i=0;if(count>0){roots=w->roots;do{CA*r=roots[i];if(!strcmp(r->name,name))return r;i++;}while(i<count);}return 0;}
