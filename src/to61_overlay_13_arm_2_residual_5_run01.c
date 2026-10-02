#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma warn_unusedvar off
#pragma warn_uninitializedvar off
typedef struct { u8 *data; void *a,*b,*c; u32 *d[5],*e[5]; void *task,*other; u16 max,v42[3],v48[3]; u8 x4e[3],count,index,sel,flag,timer,dirty,flag57,flag58,flag59; } State;
typedef struct { u8 sel,pad; u16 scroll; State *p; } Root;
typedef struct { void *a; u32 *b; u8 text[33]; u8 count,flag; } Text;
typedef struct { void *a; u32 *b; u8 text[12]; u8 count,flag; } Numeric;
extern Root ov13_0224F538;
extern Text *ov13_0224F540;
extern Numeric *ov13_0224F544;
extern u8 ov13_022453CC[],ov13_0224546C[],ov13_02245470[];
extern u16 ov13_02245424[],ov13_02245426[],ov13_02245428[];
extern void ov13_0222B8E8(void (*)(void));
extern void ov13_022403B8(int,int,int,int),ov13_02240EA8(int,int),ov13_0223ABB8(int),ov13_022405B8(int);
extern int ov13_02240394(int),ov13_022283EC(void),ov13_0222A070(void),ov13_0223C1DC(void),ov13_0223C1F0(void),ov13_0222A0C0(void);
extern void ov13_022272E4(void),ov13_02227228(void),ov13_022272CC(void),ov13_02239584(void),ov13_02228054(void),ov13_02228400(int),ov13_02228414(int),ov13_022283B8(void),ov13_0223C198(void),ov13_02229D00(void),ov13_0222A084(int),ov13_0222A098(int),ov13_0222A0AC(int),ov13_0222A03C(void);
extern void ov13_0223BE6C(int,int,int,int,int);
extern void *ov13_02242528(int,void (*)(void *),int,int);
extern void ov13_022425D0(int,void *),ov13_02239578(int),ov13_0223959C(void),ov13_02227D18(void),ov13_02227B6C(int,int,int,int);
extern void ov13_02240D80(void *,int),ov13_02240E2C(void *);
extern int ov13_02232168(int);
extern void ov13_0222FF3C(void);
extern void ov13_02230540(void);
extern void ov13_022305B0(void);
extern void ov13_02230B14(void *);
extern void ov13_02230BD0(void *);
extern void ov13_02230FB0(void);
extern void ov13_0223121C(void);
extern void ov13_02231298(void);
extern void ov13_022312B8(void);
extern void ov13_022312C8(void);
extern void ov13_02231488(void);
extern void ov13_022314A8(void);


extern void ov13_02231AD4(void);
extern void ov13_02231D9C(void);
extern void ov13_02231E28(void);
extern void ov13_02231E48(void);
extern void ov13_02231E58(void);
extern void ov13_022321FC(void);
extern void ov13_0223221C(void);
extern void ov13_0223224C(void);
extern void ov13_022322D8(void);
extern void ov13_02232494(void);
extern void ov13_02232770(void);

void ov13_02230520(void);
void ov13_02230540(void);
void ov13_02230C68(void);
void ov13_02230CC4(void);
void ov13_02230E5C(void);
void ov13_02230ED0(void);
void ov13_02230F84(void);
void ov13_02230FB0(void);
void ov13_022311C8(void);
void ov13_0223121C(void);
void ov13_02231298(void);
void ov13_022312B8(void);
void ov13_02231488(void);
void ov13_0223148C(void);
void ov13_022314A8(void);
void ov13_022317A8(void);
void ov13_02231810(void);
void ov13_02231AA8(void);
void ov13_02231AD4(void);
void ov13_02231D48(void);
void ov13_02231D9C(void);
void ov13_02231E28(void);
void ov13_02231E48(void);
void ov13_022321FC(void);
void ov13_02232200(void);
void ov13_0223221C(void);
void ov13_0223224C(void);
extern void ov13_02230854(int,int),ov13_022309CC(int,int);
extern void ov13_02239378(void *,int,int);
extern void ov13_0222B948(int *,int *);
extern u32 ov13_02245454[];
extern int ov13_02228428(void);
extern long atol(const char *);

void ov13_022307AC(void);

void ov13_022309CC(int a,int b);

void ov13_02230A58(void);

void ov13_02230B14(void *task);

void ov13_02230BD0(void *task);

void ov13_02230F54(void);

void ov13_022314D8(void);

void ov13_0223186C(void);

int ov13_02232168(int limit);
extern void ov13_022315B0(void);
void ov13_02230520(void);
void ov13_02230540(void);
void ov13_022307AC(void);
void ov13_022309CC(int a,int b);
void ov13_02230A58(void);
void ov13_02230B14(void *task);
void ov13_02230BD0(void *task);
void ov13_02230C68(void);
void ov13_02230CC4(void);
void ov13_02230E5C(void);
void ov13_02230ED0(void);
void ov13_02230F54(void);
void ov13_02230F84(void);
void ov13_02230FB0(void);
void ov13_022311C8(void);
void ov13_0223121C(void);
void ov13_02231298(void);
void ov13_022312B8(void);
void ov13_02231488(void);
void ov13_0223148C(void);
void ov13_022314A8(void);
void ov13_022314D8(void);
void ov13_022317A8(void);
void ov13_02231810(void);
void ov13_0223186C(void);
void ov13_02231AA8(void);
void ov13_02231AD4(void);
void ov13_02231D48(void);
void ov13_02231D9C(void);
void ov13_02231E28(void);
void ov13_02231E48(void);
int ov13_02232168(int limit);
void ov13_022321FC(void);
void ov13_02232200(void);
void ov13_0223221C(void);
void ov13_0223224C(void);
extern int FX_DivS32(int,int);
extern void ov13_02227924(const void *,void (*)(const void *,u32,u32));
void ov13_0223110C(void);
extern u8 ov13_02246370[];
extern u8 ov13_02246384[];
extern u8 ov13_0224639C[];
extern u8 ov13_022463B4[];
void ov13_02231C8C(void);
extern u8 ov13_022463E4[];
extern u8 ov13_022463F8[];
extern u8 ov13_02246410[];
extern u8 ov13_02246428[];
extern void ov13_0222788C(void),ov13_02241D98(void *),ov13_02240B04(int),ov13_02240EFC(int,int),ov13_0222B8F8(int,int),ov13_0222B934(int,int),ov13_022412B0(void *);
extern u8 ov13_022463C8[];
extern void ov13_02232E04(void),ov13_0222FA3C(void),ov13_02238440(void);
void ov13_022315B0(void);
extern int ov13_02242074(const void *,int),ov13_0223191C(void);
extern void ov13_022316B4(void),ov13_022317A8(void);
extern void ov13_022277B8(int,int,int);
int ov13_0223191C(void);
void ov13_022312C8(void);
extern u8 ov13_0224F53C[];
void ov13_02230520(void) { ov13_022272E4(); ov13_022405B8(8); ov13_0222B8E8(ov13_02230540); }

void ov13_02230540(void) { if(ov13_02240394(1)) return; if(ov13_0224F538.p->flag) ov13_02227228(); else ov13_022272E4(); ov13_022403B8(3,1,1,8); ov13_022403B8(3,0,29,8); ov13_0222B8E8(ov13_022305B0); }
