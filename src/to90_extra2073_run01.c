#pragma force_active on
#include "global.h"
extern void ov112_021F15E4(void *),ov112_021F1904(void *,void *,void *),ov112_021F1D58(void *,void *),ov112_021F194C(void *,void *);
extern void *ov112_021F0F60(void *);
extern void ov112_021F257C(void *),ov112_021F2B2C(void *),ov112_021F25FC(void *),ov112_021F2688(void *),ov112_021F26E8(void *),ov112_021F2768(void *),ov112_021F27B8(void *),ov112_021F27E0(void *),ov112_021F2810(void *),ov112_021F2874(void *),ov112_021F28DC(void *),ov112_021F2920(void *),ov112_021F2944(void *),ov112_021F298C(void *),ov112_021F2A78(void *,void *);
void ov112_021F2B80(void *);
#pragma force_active on
void ov112_021F2B80(void *argument){
 u8 *work=argument,*entry=*(u8 **)(work+0xc0+work[0x13d]*4);void *session;u8 state;
 ov112_021F15E4(work);session=*(void **)(work+0x130);ov112_021F1904(work,session,ov112_021F0F60(entry));ov112_021F1D58(work,ov112_021F0F60(entry));
 if((u8)*(u32 *)(work+0x144)!=entry[0x76])ov112_021F194C(work,ov112_021F0F60(entry));
 *(u32 *)(work+0x144)=(*(u32 *)(work+0x144)&~255u)|entry[0x76];state=entry[0x84];
 if(state>=1&&state<=10){if((u16)(*(u16 *)(entry+8)+0xfff9)<=1)ov112_021F257C(work);else ov112_021F2B2C(work);}
 else if(state==11)ov112_021F25FC(work);
 else if(state==12)ov112_021F2688(work);
 else if(state==13)ov112_021F26E8(work);
 else if(state==14)ov112_021F2768(work);
 else if(state==15)ov112_021F27B8(work);
 else if(state==16)ov112_021F27E0(work);
 else if(state==17)ov112_021F2810(work);
 else if(state>=18&&state<=22)ov112_021F2874(work);
 else if(state==23)ov112_021F28DC(work);
 else if(state==24)ov112_021F2920(work);
 else if(state==25)ov112_021F2944(work);
 else if(state==26)ov112_021F298C(work);
 else if(state==27)ov112_021F2A78(work,session);
}
