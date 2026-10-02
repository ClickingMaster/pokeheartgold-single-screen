#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern s32 FX_DivS32(s32,s32);
extern int WM_GetAllowedChannel(void),WM_GetLinkLevel(void);
struct Sprite {u32 bits;u16 attr;};
static inline void ReadPos(struct Sprite *s,int *x,int *y) {*x=(s->bits&0x01ff0000)>>16;*y=(s->bits&255)-12;}
struct A {void *task; struct Sprite *sprite; u8 type,tick;};
struct B {void *sprite[3];void *task,*task2;u16 tick;s8 selected;u8 type,flag,ending;};
struct C {struct Sprite *sprite;void *task;s8 type;};
struct D {void *win[12];struct Sprite *sprite[47],*extra[4];void *other[2],*handle[4],*last,*task;u8 selected,page;s8 down;s8 hover;u8 pad,index,hold,flag1,flag2;};
extern struct A *ov13_0224F464[1];

extern struct B *ov13_0224F468;
extern struct C *ov13_0224F4B8[1];
extern struct D *ov13_0224F4BC;
extern u8 ov13_0224F46C,ov13_02244EB0[],ov13_02244EE0[],ov13_02244EC4[],ov13_02244ED2[],ov13_02244F30[],ov13_02244F38[],ov13_02244F44[];
extern u16 ov13_02244EBC[],ov13_02244EBE[],ov13_02244F14[],ov13_02244F16[];
extern void *ov13_0224F4B0,*ov13_0224F4B4,*ov13_0224F4F8;
extern char ov13_0224F470[],ov13_02245B20[],ov13_02245D54[];
extern char *ov13_02245D04[];
extern u32 ov13_02244EF8[];
extern void *ov13_0224128C(int,int);
extern void ov13_022412B0(void *);
extern struct Sprite *ov13_02240184(int,int);
extern void *ov13_022401B0(int,int,int);
extern void ov13_02240008(void *,int,int);
extern void ov13_022400F0(int,int,void *);
extern void ov13_0223FF18(void *,int,int,int);
extern void ov13_02240068(void *,int,int *,int *);
extern void *ov13_02242528(int,void (*)(),int,int);
extern void ov13_022425C8(void *,void (*)());
extern void ov13_022425D0(int,void *);
extern void ov13_022425F0(int,void *);
extern void ov13_02241D98(void *);
extern void ov13_0223FD60(void *);
extern int ov13_0222B970(void),ov13_0222B980(void);
extern void *ov13_022408A0(int,int);
extern void ov13_02240B04(int);
extern void ov13_02240CC0(void *,int,int,int,int,int,u32,void *);
extern void ov13_02240B28(void *,int,int,int,int,void *);
extern void ov13_02240E2C(void *);
extern void *ov13_0222BAF4(void *,int,int,int),*ov13_0222BADC(void *,int);
extern void *ov13_0223F95C(char *,u32 *,int);
extern void ov13_0223FA50(void *);
extern void ov13_0222B90C(int *,int *);
extern void ov13_0223AC4C(int),ov13_0223B1E0(int);
extern u8 *ov13_02239FF0(void);
extern void ov13_0222936C(int),ov13_0223ABB8(int),ov13_022293F0(int,int);
extern void ov13_022290F0(int,int,int),ov13_022294B8(void);
extern void ov13_02228738(void),ov13_02228920(void),ov13_02228C68(void),ov13_02228EC0(void);
extern void ov13_02227064(void),ov13_02227390(void *),ov13_022273FC(void *),ov13_0222744C(void),ov13_022275D8(void *),ov13_0222763C(void *),ov13_022276B4(void *),ov13_02227FAC(void),ov13_022299CC(void),ov13_022284D8(void *),ov13_0222856C(void *),ov13_02228600(void *),ov13_02228694(void *),ov13_02228720(void),ov13_02227E70(void *);
void ov13_022272FC(int);void ov13_02227130(int);u32 ov13_02227B1C(void);void ov13_02227D18(void);char *ov13_022278C4(char *);void ov13_02227924(char *,void (*)(const void *,u32,u32));void ov13_02227DE4(int);

void ov13_02226F58(int type);
void ov13_02227028(void);
void ov13_02227228(void);
int ov13_02227258(void);
void ov13_0222726C(int a);
void ov13_0222728C(int a);
int ov13_022272A0(void);
void ov13_022272CC(void);
void ov13_022272E4(void);
void ov13_022277A4(void);
int ov13_022277B8(int a,int b,int c);
int ov13_0222788C(void);
void ov13_02227964(int a);
void ov13_022279B4(int a);
void ov13_022279EC(int a);
void ov13_02227A60(void *a,int b);
void ov13_02227B34(void);
void ov13_02227B54(void);
void ov13_02227B6C(int a,int b,int c,int y);
void ov13_02227C28(int a,int b,int c,int d);
void ov13_02227D58(void);
void ov13_02227DD0(void);
void ov13_02227E40(void);
void ov13_02227E58(void);
void ov13_02227EB0(int a);
void ov13_02227F68(void);
void ov13_022283B8(void);
int ov13_022283EC(void);
void ov13_02228400(int a);
void ov13_02228414(int a);
int ov13_02228428(void);


void ov13_02228444(void *task);
void ov13_02228C08(void);
void ov13_02228C38(void);
void ov13_02228E6C(int a);

extern u16 ov13_02244EB8[],ov13_02245958[],ov13_02244F68[],ov13_02244F64[];
extern u16 ov13_02244FA4[],ov13_02244F84[],ov13_02244F94[],ov13_02244F7C[],ov13_02244F9C[],ov13_02244F8C[];
extern int ov13_02245D80[];
extern void ov13_02240E74(void *,void *,void *);
extern int ov13_022417DC(void *),ov13_02241754(void *);
extern void *ov13_0223FDE0(void *,int);
void ov13_02228E6C(int);
void ov13_02227228(void) {ov13_0224F468->ending=1;ov13_022425C8(ov13_0224F468->task,ov13_022275D8);}
int ov13_02227258(void) {return ov13_0224F468->selected;}
void ov13_0222726C(int a) {if(ov13_0224F468->selected==-1)ov13_0224F468->selected=a;}
void ov13_0222728C(int a) {ov13_0224F468->selected=a;}
int ov13_022272A0(void) {if(!ov13_0224F468)return 1;if(ov13_0224F468->ending==0)return 1;return 0;}
void ov13_022272CC(void) {ov13_0224F468->flag=0;}
void ov13_022272E4(void) {ov13_0224F468->flag=1;}
void ov13_022272FC(int y) {int n=ov13_02244EE0[ov13_0224F468->type],i;ov13_0223FF18(ov13_0224F468->sprite[2],-1,0,y);for(i=0;i<n;i++){ov13_0223FF18(ov13_0224F468->sprite[i],-1,ov13_02244EBC[(ov13_02244ED2+ov13_0224F468->type*2)[i]*2],y+4);}}
void ov13_02227390(void *task) {int x,y;ov13_02240068(ov13_0224F468->sprite[2],0,&x,&y);y-=4;ov13_022272FC(y);if(y>168)return;ov13_022272FC(168);ov13_022425C8(task,ov13_022273FC);}
void ov13_022273FC(void *task) {ov13_0224F468->selected=-1;ov13_0224F468->tick++;if(ov13_0224F468->tick<4)return;ov13_0224F468->tick=0;ov13_022425C8(task,ov13_0222744C);}
void ov13_0222744C(void) {u16 pos[4];int n=ov13_02244EE0[ov13_0224F468->type],i;if(ov13_0224F468->flag==0){if(ov13_0224F468->selected!=-1)return;for(i=0;i<n;i++){ov13_02240E74(ov13_02244EBC+(ov13_02244ED2+ov13_0224F468->type*2)[i]*2,ov13_02244EB8,pos);if(ov13_022417DC(pos)!=0){int a;void *p;if(ov13_0224F468->task2)break;a=(ov13_02244EC4+ov13_0224F468->type*2)[i];p=ov13_0223FDE0(ov13_0224F468->sprite[i],0);ov13_022400F0(0,a+1,p);a=(ov13_02244ED2+ov13_0224F468->type*2)[i]*2;ov13_0223FF18(ov13_0224F468->sprite[i],-1,ov13_02244EBC[a],ov13_02244EBE[a]);ov13_02240008(ov13_0224F468->sprite[i],-1,1);ov13_0224F468->task2=ov13_02242528(0,ov13_022276B4,0,110);ov13_0224F468->selected=i;return;}}}ov13_0224F468->selected=-1;}
void ov13_022275D8(void *task) {int x,y;ov13_02240068(ov13_0224F468->sprite[2],0,&x,&y);y+=4;ov13_022272FC(y);if(y<192)return;ov13_022425C8(task,ov13_0222763C);}
void ov13_0222763C(void *task) {int i;ov13_022425D0(0,task);if(ov13_0224F468->task2)ov13_022425D0(0,ov13_0224F468->task2);for(i=0;i<2;i++)if(ov13_0224F468->sprite[i])ov13_0223FD60(ov13_0224F468->sprite[i]);ov13_0223FD60(ov13_0224F468->sprite[2]);ov13_022412B0(&ov13_0224F46C-4);}
void ov13_022276B4(void *task) {int n,i;ov13_0224F468->tick++;if(ov13_0224F468->tick<16)return;n=ov13_02244EE0[ov13_0224F468->type];for(i=0;i<n;i++){int a=(ov13_02244EC4+ov13_0224F468->type*2)[i];void *s=ov13_0223FDE0(ov13_0224F468->sprite[i],0);ov13_022400F0(0,a,s);ov13_02240008(ov13_0224F468->sprite[i],-1,1);}ov13_022272FC(168);ov13_0224F468->tick=0;ov13_0224F468->selected=-1;if(!ov13_0224F468->task2)return;ov13_0224F468->task2=0;ov13_022425D0(0,task);}
void ov13_022277A4(void) {ov13_0224F46C=0;}
int ov13_022277B8(int a,int b,int c) {void *w,*t;u32 f;if(ov13_0224F46C)return 0;ov13_02227924(ov13_02245B20,GXS_LoadBG0Scr);*(vu32 *)0x04001010=0x01920000;w=ov13_022408A0(1,0);t=ov13_0222BAF4(ov13_0224F4F8,a,b,c);f=ov13_02227B1C();ov13_02240CC0(w,20,0,216,64,2,f,t);ov13_02240E2C(w);ov13_0224F46C=1;return 1;}
int ov13_0222788C(void) {if(!ov13_0224F46C)return 0;ov13_02240B04(1);ov13_0224F46C=0;return 1;}
char *ov13_022278C4(char *s) {int a;strncpy(ov13_0224F470,s,63);if(((u8 *)s)[5]=='x')return ov13_0224F470;a=ov13_0222B970();if(((u8 *)s)[5]=='y'&&a!=0)return ov13_0224F470;ov13_0224F470[5]=ov13_02244F30[a];return ov13_0224F470;}
void ov13_02227924(char *s,void (*load)(const void *,u32,u32)) {u32 size;void *data;s=ov13_022278C4(s);data=ov13_0223F95C(s,&size,4);DC_FlushRange(data,size);load(data,0,size);ov13_0223FA50(data);}
void ov13_02227964(int a) {int type;ov13_0222B90C(&type,0);if(type==1)ov13_0223AC4C(a);else if(type==2)ov13_0223B1E0(a);}
void ov13_022279B4(int a) {int type;ov13_0222B90C(0,&type);if(type==1)ov13_02227130(a);}
void ov13_022279EC(int a) {void *w,*t;u32 f;w=ov13_022408A0(0,0);t=ov13_0222BADC(ov13_0224F4F8,a);f=ov13_02227B1C();ov13_02240CC0(w,13,40,230,112,2,f,t);ov13_02240E2C(w);}
void ov13_02227A60(void *a,int b) {void *w,*t;u32 f;int x,y;w=ov13_022408A0(0,0);t=ov13_0222BADC(ov13_0224F4F8,b);f=ov13_02227B1C();ov13_02240CC0(w,13,60,230,94,2,f,t);x=ov13_0222B970();y=ov13_0222B970();ov13_02240B28(w,ov13_02244F14[x*2],ov13_02244F16[y*2],2,521,a);ov13_02240E2C(w);}
u32 ov13_02227B1C(void) {return ov13_02244EF8[ov13_0222B970()];}
void ov13_02227B34(void) {ov13_0224F4B0=ov13_0224128C(16,4);}
void ov13_02227B54(void) {ov13_02227D18();ov13_022412B0(&ov13_0224F4B0);}
void ov13_02227B6C(int a,int b,int c,int y) {int i;u8 *p;ov13_02227D18();p=ov13_02244F38+a*2;for(i=0;i<2;i++){((void **)ov13_0224F4B0)[i]=ov13_022401B0(0,*p++,1);ov13_02240008(((void **)ov13_0224F4B0)[i],-1,1);}ov13_0223FF18(((void **)ov13_0224F4B0)[0],-1,b,y);ov13_0223FF18(((void **)ov13_0224F4B0)[1],-1,c,y);}
