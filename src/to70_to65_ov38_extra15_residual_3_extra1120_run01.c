#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[0x38];int result;u8 pad2[0x3c];char *buf;u32 spare;int size;u8 pad3[0x60];int major,minor,status,offset;u8 pad4[8];int error;} Request;
extern char ov38_02225B68[],ov38_02225B74[],ov38_02225AF0[],ov38_02225C78[],ov38_02225C98[];
extern u16 __msl_digit[];
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
extern int sscanf(const char *,const char *,...);
int ghiParseStatus(Request *);
static inline int digit(int ch) { int invalid=1;if(ch>=0 && ch<128)invalid=0;return invalid?0:__msl_digit[ch]&0x100; }
int ghiParseStatus(Request *w) {
 int major,minor,status,end;int count;int i;
 if(!w)__msl_assertion_failed(ov38_02225B68,ov38_02225B74,ov38_02225AF0,590);
 if(w->size<=0)__msl_assertion_failed(ov38_02225C78,ov38_02225B74,ov38_02225AF0,591);
 count=sscanf(w->buf,ov38_02225C98,&major,&minor,&status,&end);
 i=end;
 while(w->buf[i] && digit((signed char)w->buf[i])) {i++;end=i;}
 if(count!=3 || major<1 || status<100 || status>=600) {w->error=1;w->result=7;return 0;}
 w->major=major;w->minor=minor;w->status=status;w->offset=end;return 1;
}
