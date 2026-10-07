// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
typedef struct State{void(*callback)(int,int,int);void(*end)(void);char small[32],big[32];int active;u32 info;char user[65];char data[303];u8 stack[0x9c0-0x1c0];OSThread thread;}State;
typedef char StateExtent[(sizeof(State)==2688)?1:-1];
extern State*ov00_0221A768;extern char ov00_022179B0[],ov00_022179C4[];
extern int ov00_021ECCE0(void),ov00_021EBE08(void*,int,void*,int),ov38_02220434(void*(*)(u32,int),void(*)(void*),char*,char*,char*,char*,int);
extern void ov00_021ECEFC(int,unsigned,int),ov00_021ECEE4(void*),ov00_021EC0FC(void*),ov00_021EC24C(int,int);extern void*ov00_021ECECC(u32,int);
void ov00_021ED038(void*);
void ov00_021ED038(void*userdata){struct Frame{int value;char decoded[400],url[256];}f;int status;State*s;for(;;){status=ov00_021ECCE0();if(status==3){s=ov00_0221A768;if(strlen(s->user)+strlen(ov00_022179B0)>255){ov00_021ECEFC(0,8,-1);return;}OS_SNPrintf(f.url,256,ov00_022179C4,s->user);s=ov00_0221A768;if(ov00_021EBE08(s->data,strlen(s->data),f.decoded,400)<0){ov00_021ECEFC(0,8,-1);return;}s=ov00_0221A768;if(!ov38_02220434(ov00_021ECECC,ov00_021ECEE4,f.url,f.decoded,s->small,s->big,(int)ov00_021ECEFC)){ov00_021ECEFC(0,8,-1);return;}ov00_0221A768->active=1;ov00_021ECEFC(0,0,-1);return;}else if(status==4){ov00_021EC0FC(&f.value);ov00_021EC24C(14,f.value);ov00_0221A768->callback(0,3,-1);return;}else if(status==5)ov00_021ECEFC(0,6,-1);OS_Sleep(100);}}
