#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
typedef struct {char name[128],user[24],password[64];int auth,mode;} Settings;typedef struct {u8 p[0x134];void *object;} Work;
extern char ov45_022559B8[],ov45_02255918[],ov45_0225582C[],ov45_02255ABC[],ov45_02255ACC[],ov45_02255ADC[],ov45_022559D4[],ov45_022559E4[],ov45_02255AF4[],ov45_02255B10[];extern void __msl_assertion_failed(char *,char *,char *,int);extern Work *ov45_022475B8(void *,char *);extern void ov00_021FEF74(void *,Settings *);extern int ov00_021FF0C0(void *,Settings *);void ov45_02248888(void *,char *,char *,int,char *,char *);
void ov45_02248888(void *owner,char *name,char *key,int mode,char *user,char *pass){Work *w;if(owner==0)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_0225582C,720);if(name==0)__msl_assertion_failed(ov45_02255ABC,ov45_02255918,ov45_0225582C,722);if(*name==0)__msl_assertion_failed(ov45_02255ACC,ov45_02255918,ov45_0225582C,722);if(strlen(name)>=128)__msl_assertion_failed(ov45_02255ADC,ov45_02255918,ov45_0225582C,722);if(key==0)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_0225582C,723);if(*key==0)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_0225582C,723);if(mode<0||mode>3)__msl_assertion_failed(ov45_02255AF4,ov45_02255918,ov45_0225582C,724);w=ov45_022475B8(owner,key);if(w==0)return;{Settings s={0};strncpy(s.name,name,128);s.name[127]=0;if(user!=0&&pass!=0){strncpy(s.user,user,24);s.user[23]=0;strncpy(s.password,pass,64);s.password[63]=0;s.auth=1;}else s.auth=0;s.mode=mode;ov00_021FEF74(w->object,&s);if(ov00_021FF0C0(w->object,&s)==0)__msl_assertion_failed(ov45_02255B10,ov45_02255918,ov45_0225582C,759);}}
