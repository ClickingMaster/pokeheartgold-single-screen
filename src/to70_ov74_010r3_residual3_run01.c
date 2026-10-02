#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Text {u32 v[19];} Text;
typedef struct Work {u8 pad0[0x20];void *bg;} Work;
extern int PmAgbCartridge_GetLanguage(void);
extern void ConvertRSStringToDPStringInternational(void*,u16*,int,int);
extern void ov74_02231A1C(void*,Text*,int);
extern void *GetBgTilemapBuffer(void*,int);
extern u32 AGB_GetBoxMonData(void*,int,void*);
extern void BgCommitTilemapBufferToVram(void*,int);
void ov74_02232678(u8*,int);void ov74_02232700(Work*,void*);
void ov74_02232678(u8*w,int box){Text t={0};u16 name[10];t.v[0]=(u32)(w+0x478);t.v[2]=6;t.v[3]=21;t.v[4]=13;t.v[5]=2;t.v[13]=-1;t.v[6]=0;t.v[7]=0;t.v[9]=1;t.v[10]=1;t.v[11]=0x10200;t.v[8]=160;int language=PmAgbCartridge_GetLanguage();ConvertRSStringToDPStringInternational(*(u8**)(w+0xe880)+0x8344+box*9,name,9,language);t.v[14]=(u32)name;ov74_02231A1C(w,&t,1);}
