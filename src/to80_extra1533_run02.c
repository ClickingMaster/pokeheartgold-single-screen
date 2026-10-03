#pragma force_active on
#include "global.h"
typedef struct {
 u8 group,choice,index,active,b4,b5;u16 pad6,field8,flagA;u8 padC[8];u8 menu[48];u16 special,specialValue;void *specialData;u32 flag;
} State;
extern void *ov49_02259FE8(void*),*ov49_02259FF0(void*);
extern State *ov49_0225EF84(void*),*ov49_0225EF40(void*,int);
extern u32 ov45_0222B034(void*),ov45_0222B040(void*),ov45_0222B06C(void*),ov45_0222B020(void*),ov45_0222B028(void*),ov45_0222B0B0(void*);
extern void *ov45_0222A5C0(void*),*ov45_0222A578(void*,int),*ov45_0222AAC8(void*);
extern u32 ov49_0225EF88(void*),ov49_0225A0DC(void*),ov49_0225A1D4(void*),ov49_0225CB70(void*),ov49_02258DAC(void*);
extern void ov49_0225EF8C(void*,int),ov49_0225A0CC(void*),ov49_0225A0BC(void*),ov49_0225A0EC(void*),ov49_0225A4D0(void*),ov49_0225EF68(void*);
extern void ov45_0222A5E8(void*,int),ov45_0222B0E8(void*,u32),ov45_0222B118(void*,int),ov45_0222AED8(void*,u16),ov45_0222B0BC(void*),ov45_0222AFC4(void*),ov45_0222AE64(void*),ov45_0222B0D8(void*,int),ov45_0222A704(void*,u16,void*);
extern void ov49_0225A428(void*,u32,int),ov49_0225A334(void*,u16,int),ov49_0225A08C(void*,void*),ov49_0225A478(void*,void*),ov49_0225A174(void*,void*,int,int),ov49_0225A1E4(void*,int,int),ov49_02258EEC(void*,u32,int);
extern void *ov49_02264C04(void*,u16,int),*ov49_02264C50(void*,u16,u16);
extern int ov49_02264CA8(State*,void*,u32),ov49_02264E20(State*,void*,void*);
extern void ov49_02264CFC(State*,u8,int,void*,int),ov49_02264D4C(State*,void*,void*,int,void*,void*),ov49_02264EC8(State*,void*),ov49_02264E90(State*,void*,void*,int,int);
extern void ov49_02264F78(State*,void*),ov49_02264F10(State*),ov49_02264F1C(State*),ov49_02264F24(State*,void*),ov49_02264F60(State*),ov49_02264CF8(State*),ov49_02264D14(State*,void*),ov49_02264D30(State*,void*,void*),ov49_02264F9C(void*,void*,int,int,int),ov49_02265260(void*,void*);
extern void IncrementGameStat47(void*),PlaySE(int);
extern u32 String_GetLength(const void*);
int ov49_02263B74(void*,void*);
int ov49_02263B74(void *task,void *ctx){
 void *game=ov49_02259FE8(ctx);State*s=ov49_0225EF84(task);
 u32 status=ov45_0222B034(game),selection=ov45_0222B040(game);void *b,*a;
 if(ov45_0222B06C(game) && s && s->flagA==1){ov49_0225EF8C(task,26);ov49_0225A0CC(ctx);s->flagA=0;}
 if(s){b=ov45_0222A5C0(game);a=ov45_0222A578(game,s->active);}
 if(ov49_0225EF88(task)!=0 && s->flagA==1 && !a){ov49_0225EF8C(task,26);ov49_0225A0CC(ctx);s->flagA=0;}
 switch(ov49_0225EF88(task)){
 case 0:{u32 value;int result;s=ov49_0225EF40(task,80);s->special=0;s->specialValue=0;s->specialData=0;value=ov45_0222B020(game);result=ov49_02264CA8(s,game,value);PlaySE(1508);ov45_0222A5E8(game,9);if(result==1){ov49_0225A428(ctx,value,0);ov45_0222B0E8(game,value);ov49_0225EF8C(task,1);}else ov49_0225EF8C(task,26);break;}
 case 1:if(!ov45_0222B0B0(game))break;ov49_02264D4C(s,game,ctx,s->b5,a,b);ov49_02264CFC(s,128,2,task,30);break;
 case 2:IncrementGameStat47(game);ov45_0222B118(game,7);ov49_02264D4C(s,game,ctx,s->b4,b,a);ov49_02264CFC(s,128,3,task,30);break;
 case 3:switch(status){case 3:ov49_0225A0CC(ctx);ov45_0222AED8(game,0);if(ov45_0222B028(game))ov49_0225EF8C(task,4);else ov49_0225EF8C(task,6);break;case 2:if(!ov49_0225A0DC(ctx))ov49_0225A0BC(ctx);ov49_02264EC8(s,ctx);break;case 0:case 1:case 4:default:ov49_0225EF8C(task,26);ov49_0225A0CC(ctx);break;}break;
 case 4:if(ov49_02264E20(s,game,ctx)==1)ov49_02264CFC(s,128,5,task,30);else ov49_0225EF8C(task,5);break;
 case 5:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,40));ov49_02264CFC(s,128,7,task,30);break;
 case 6:ov49_0225A334(ctx,s->active,0);ov49_0225A334(ctx,s->field8,1);ov49_0225A08C(ctx,ov49_02264C50(ctx,s->active,s->field8));ov49_02264CFC(s,128,7,task,30);break;
 case 7:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,47));ov49_02264CFC(s,128,9,task,30);break;
 case 8:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,53));ov49_02264CFC(s,128,9,task,30);break;
 case 9:switch(status){case 3:switch(selection){case 3:ov49_0225EF8C(task,10);ov45_0222B0BC(game);s->flag=1;ov49_02264F78(s,a);break;case 4:ov49_0225EF8C(task,14);ov45_0222B0BC(game);s->flag=1;ov49_02264F78(s,a);break;case 5:ov45_0222AED8(game,1);ov49_0225EF8C(task,15);break;default:ov49_0225EF8C(task,26);ov49_0225A0CC(ctx);break;}ov49_0225A0CC(ctx);break;case 2:if(!ov49_0225A0DC(ctx))ov49_0225A0BC(ctx);ov49_02264EC8(s,ctx);break;case 0:ov49_0225EF8C(task,27);ov49_0225A0CC(ctx);break;case 1:case 4:default:ov49_0225EF8C(task,26);ov49_0225A0CC(ctx);break;}break;
 case 10:ov49_0225A334(ctx,s->active,0);ov49_02264E90(s,game,ctx,1,1);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,511));ov49_02264CFC(s,128,11,task,30);break;
 case 11:ov49_0225A334(ctx,s->active,0);ov49_02264E90(s,game,ctx,1,1);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,512));ov49_02264CFC(s,128,12,task,30);break;
 case 12:ov49_0225A334(ctx,s->active,0);ov49_02264E90(s,game,ctx,1,1);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,513));ov49_02264CFC(s,128,13,task,30);break;
 case 13:PlaySE(1471);ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,514));ov49_02264CFC(s,128,8,task,30);ov45_0222AED8(game,2);ov49_0225A478(ctx,ov45_0222AAC8(ov45_0222A5C0(game)));break;
 case 14:ov49_0225A334(ctx,s->active,0);ov49_02264E90(s,game,ctx,1,1);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,507));ov49_02264CFC(s,128,27,task,30);ov45_0222AED8(game,2);break;
 case 15:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,687));ov49_02264CFC(s,128,16,task,30);break;
 case 16:switch(status){case 3:ov49_0225EF8C(task,17);ov49_0225A0CC(ctx);ov45_0222B0BC(game);s->flag=1;break;case 2:if(!ov49_0225A0DC(ctx))ov49_0225A0BC(ctx);ov49_02264EC8(s,ctx);break;case 0:case 1:case 4:default:ov49_0225EF8C(task,26);ov49_0225A0CC(ctx);break;}break;
 case 17:if(selection<6 || selection>=126){ov49_0225EF8C(task,26);break;}s->index=selection-6;s->group=(u32)s->index>>2;ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,s->group+55));ov49_02264CFC(s,128,18,task,30);break;
 case 18:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,s->group+85));ov49_02264CFC(s,128,19,task,30);break;
 case 19:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,s->index+236));ov49_02264CFC(s,128,20,task,30);break;
 case 20:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,s->group+477));ov49_02264CFC(s,128,21,task,30);break;
 case 21:ov49_02264F9C(s->menu,ctx,5,s->group*4+115,2);ov49_0225A174(ctx,s->menu,0,0);ov49_02264F10(s);ov49_0225EF8C(task,22);break;
 case 22:{u32 value=ov49_0225A1D4(ctx);int ok;switch(value){case 0:case 1:case 2:case 3:s->choice=value+s->group*4;ov45_0222AED8(game,s->choice+6);ov49_0225EF8C(task,23);ok=1;break;case 4:s->choice=126;ov45_0222AED8(game,s->choice);ov49_0225EF8C(task,28);ok=1;break;default:ov49_02264F24(s,ctx);ok=0;break;}if(ok){ov49_0225A1E4(ctx,0,0);ov49_02265260(s->menu,ctx);ov49_02264F1C(s);ov49_02264F78(s,a);}break;}
 case 23:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,s->choice+357));if(s->index==s->choice)ov49_02264CFC(s,128,24,task,30);else ov49_02264CFC(s,128,25,task,30);break;
 case 24:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,51));ov49_02264CFC(s,128,25,task,30);break;
 case 25:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,52));ov49_02264CFC(s,128,8,task,30);break;
 case 26:{void *string;u32 length,duration;ov49_0225A1E4(ctx,0,0);ov49_02265260(s->menu,ctx);string=ov49_02264C04(ctx,s->active,45);ov49_0225A08C(ctx,string);s->flagA=0;length=String_GetLength(string);duration=(ov49_0225CB70(ctx)*length>>1)+60;if(duration<128)duration=128;else if(duration>255)duration=255;ov49_02264CFC(s,(u8)duration,31,task,29);ov45_0222AFC4(game);break;}
 case 27:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,43));s->flagA=0;ov49_02264CFC(s,128,31,task,29);break;
 case 28:ov49_0225A334(ctx,s->active,0);ov49_0225A08C(ctx,ov49_02264C04(ctx,s->active,49));ov49_02264CFC(s,128,27,task,30);break;
 case 29:ov49_02264D14(s,task);break;
 case 30:ov49_02264D30(s,task,ctx);break;
 case 31:if(s->special==1)ov45_0222A704(game,s->specialValue,s->specialData);if(!s->flag)ov45_0222B0D8(game,s->active);ov45_0222AE64(game);ov49_0225A0EC(ctx);ov49_02264CF8(s);ov49_0225EF68(task);ov45_0222A5E8(game,1);{void *x=ov49_02259FF0(ctx);ov49_02258EEC(x,ov49_02258DAC(x),1);}ov49_0225A4D0(ctx);return 1;
 }
 ov49_02264F60(s);return 0;
}
