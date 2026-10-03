#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern char *strchr(const char *,int);
extern u32 ov00_021FF368(void);
extern void *ov00_021EC3A8(u32);
extern void ov00_021EC3D8(void *);
extern int ov38_0221CB7C(void *,void *,int *);
extern int ghiProcessIncomingFileData(void *,void *,int);
typedef struct {u8 p[0x38];int error;u8 p2[0xfc-0x3c];int done,received,total;int other108,other10c,chunked;char header[12];int used,remaining,state;u8 p3[0x158 - 0x12c];u32 timeout;} Work;
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
extern char ov38_02225B68[],ov38_02225B74[],ov38_02225B2C[],ov38_02225CB0[],ov38_02225CC4[],ov38_02225BB0[];
extern void ghiAppendToChunkHeaderBuffer(void *,const char *,int);
extern int ghiParseChunkSize(void *);
extern int ov38_0221F624(void *,const char *,int);
void ov38_0221FEA8(Work *);
void ov38_0221FEA8(Work *w){void *buffer;u32 start=ov00_021FF368(),elapsed=0;buffer=ov00_021EC3A8(8192);int length,result;if(!buffer){w->done=1;w->error=1;return;}while(!w->done&&elapsed<w->timeout){length=8192;result=ov38_0221CB7C(w,buffer,&length);if(result==3||result==1)break;if(result==2){w->done=1;if(w->total>0&&w->received<w->total)w->error=15;break;}if(!ghiProcessIncomingFileData(w,buffer,length))break;elapsed=ov00_021FF368()-start;}ov00_021EC3D8(buffer);}
