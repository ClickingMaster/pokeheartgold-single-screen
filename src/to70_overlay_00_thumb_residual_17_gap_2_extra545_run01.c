#pragma force_active on
#include "global.h"
#include "to90_vct_ssp_private.h"
typedef struct{u8 pad[0x19e8];u32 id;int state;u8 pad2[0x64];void*session;}Work;
extern struct{u32 unused;Work*w;} _0221A684;
extern void VCT_Response(void*,int),VCT_DeleteSession(void*),ov00_021E7638(void*),ov00_021E6A4C(void);extern int VCT_StartStreaming(void*);
void ov00_021E756C(u32,u32,Session*,void*);
void ov00_021E756C(u32 unusedId,u32 event,Session*s,void*unusedContext){switch(event){case 1:if(_0221A684.w->session!=NULL){VCT_Response(s,3);VCT_DeleteSession(s);}else _0221A684.w->session=s;break;case 8:VCT_Response(s,0);ov00_021E7638(s);ov00_021E6A4C();break;case 9:ov00_021E7638(s);ov00_021E6A4C();break;case 4:VCT_Response(s,4);ov00_021E7638(s);break;case 7:if(s->state!=_0221A684.w->id)ov00_021E7638(s);else if(VCT_StartStreaming(s)!=0)_0221A684.w->state=2;break;case 2:ov00_021E7638(s);break;case 3:case 11:case 12:ov00_021E7638(s);break;}}
