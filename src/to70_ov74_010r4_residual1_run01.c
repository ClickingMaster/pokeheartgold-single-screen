#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x20];void *bg;u8 gap[0x12604-0x24];void (*callback)(void);} Work;
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void DoScheduledBgGpuUpdates(void*);

void ov74_02233024(Work *w);

extern void *ov74_02233F84(Work*),*MessageFormat_New(int),*String_New(int,int);
extern int PmAgbCartridge_GetLanguage(void);
extern void ConvertRSStringToDPStringInternational(void*,u16*,int,int),CopyU16ArrayToString(void*,u16*),BufferString(void*,int,void*,int,int,int),ov74_02232B18(Work*),ov74_02231A1C(Work*,void*,int),String_Delete(void*),MessageFormat_Delete(void*),ov74_02232F5C(Work*);
extern int ov74_0223C980[];
void ov74_02232F9C(Work*);
void ov74_02232F9C(Work*w){u16 text[8];void *fmt,*str;void*name=ov74_02233F84(w);ConvertRSStringToDPStringInternational(name,text,8,PmAgbCartridge_GetLanguage());fmt=MessageFormat_New(76);str=String_New(8,76);CopyU16ArrayToString(str,text);BufferString(fmt,1,str,0,1,2);ov74_02232B18(w);*(int*)((u8*)w+0x460)=ov74_0223C980[*(int*)w];*(void**)((u8*)w+0x46c)=fmt;ov74_02231A1C(w,(u8*)w+0x42c,24);String_Delete(str);MessageFormat_Delete(fmt);ov74_02232F5C(w);}
void ov74_02233024(Work *w){volatile u32 *base=(volatile u32*)0x027e0000;if(w->callback){w->callback();w->callback=0;}GF_RunVramTransferTasks();OamManager_ApplyAndResetBuffers();DoScheduledBgGpuUpdates(w->bg);base[0x3ff8/4]|=1;}
