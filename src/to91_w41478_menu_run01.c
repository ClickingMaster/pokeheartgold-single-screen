#pragma force_active on
#include "global.h"
#include "system.h"
#include "sound.h"
extern BOOL PlaySE(u16);
typedef struct {s16 page,oldpage,selection,previous;u8 body[0xec];int timer;} Menu;
typedef struct {u32 clear;u8 gap[10];u8 enabled;} Input;
extern int ov43_0222BC78(u8 *,u8 *,void *,int);
extern void ov43_0222C844(void *,void *,s32),ov43_0222AD00(u8 *,int);
int ov43_0222BD18(void *,void *,void *);
#pragma force_active on
int ov43_0222BD18(void *menu,void *input,void *user){
 Menu *p=menu; Input *in=input;
 u16 old;BOOL changed,page,right;
 if(gSystem.newKeys&0xcf3)in->clear=0;
 if(gSystem.newKeys&1)return ov43_0222BC78((u8*)p,(u8*)in,user,5);
 if(gSystem.newKeys&2){p->selection=8;ov43_0222C844(p->body,user,p->selection);return ov43_0222BC78((u8*)p,(u8*)in,user,0);}
 old=p->selection;changed=FALSE;page=FALSE;
 if(gSystem.newAndRepeatedKeys&64){
 if(p->selection==8){p->selection=p->previous;changed=TRUE;ov43_0222AD00((u8*)user,1);}
 else if(p->selection<4){if(p->selection>0){p->selection--;changed=TRUE;}}
 else if(p->selection>4){p->selection--;changed=TRUE;}
 }else if(gSystem.newAndRepeatedKeys&128){
 if(p->selection==3||p->selection==7){p->selection=8;changed=TRUE;}
 else if(p->selection<4){if(p->selection<3){p->selection++;changed=TRUE;}}
 else if(p->selection<7){p->selection++;changed=TRUE;}
 }else if(gSystem.newAndRepeatedKeys&32){if(p->selection!=8){if(p->selection<4){p->selection+=4;changed=TRUE;page=TRUE;right=FALSE;}else{p->selection-=4;changed=TRUE;}}}
 else if(gSystem.newAndRepeatedKeys&16){if(p->selection!=8){if(p->selection<4){p->selection+=4;changed=TRUE;}else{p->selection-=4;changed=TRUE;page=TRUE;right=TRUE;}}}
 else if(in->enabled){if(in->enabled&&(gSystem.newAndRepeatedKeys&512)){page=TRUE;right=FALSE;}else if(in->enabled&&(gSystem.newAndRepeatedKeys&256)){page=TRUE;right=TRUE;}}
 if(page==TRUE){if(right){p->oldpage=p->page;p->page=(p->page+1)%4;return ov43_0222BC78((u8*)p,(u8*)in,user,2);}else{p->oldpage=p->page;p->page--;if(p->page<0)p->page+=4;return ov43_0222BC78((u8*)p,(u8*)in,user,1);}}
 if(changed==TRUE){PlaySE(1509);ov43_0222C844(p->body,user,p->selection);p->previous=old;}
 return 0;
}
