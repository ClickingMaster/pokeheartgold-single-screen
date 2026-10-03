#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { char text[13]; } Text;
typedef struct { int type,index; } State;
typedef struct { u64 id; u8 other[12]; } Info;
extern const Text ov38_02224EBC;
extern State ov38_022260FC;
extern char ov38_02226104[];
extern int ov00_021EC9E0(void *);
static inline int getState(void) { return ov00_021EC9E0((void*)(u32)&ov38_022260FC); }
extern void ov38_022218C0(char *,int);
extern void ov38_0222189C(void *,const void *,int);
extern void ov38_022201C8(void *,const void *,int);
extern void ov38_02221888(void *,int);
extern void DWCi_BM_GetWiFiInfo(void *);
extern int ov38_022218CC(char *,u32,u32,int);
int ov38_02220684(void *);
int ov38_02220784(void *,void *);
int ov38_02220784(void *a,void *b) {
 Info info;
 char buf[13];
 ov38_02221888(&info,20); DWCi_BM_GetWiFiInfo(&info);
 if(info.id==0) return 0;
 if(ov38_022218CC(buf,(u32)info.id,(u32)(info.id>>32),13)<0) return 0;
 ov38_022201C8(b,buf,13); ov38_022201C8(a,(void*)0x027FFE0C,4); return 1;
}
