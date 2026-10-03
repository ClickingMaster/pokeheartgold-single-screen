#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 p;int mode;u8 mail[8];u16 a,b;} Work;extern int MailMsg_AllFieldsAreInit(void *);int ov102_021E8F14(Work *);
int ov102_021E8F14(Work *w){switch(w->mode){case 0:if(w->a!=65535)return 1;return 0;case 1:if(w->a!=65535&&w->b!=65535)return 1;return 0;case 2:return MailMsg_AllFieldsAreInit(w->mail);default:return 0;}}
