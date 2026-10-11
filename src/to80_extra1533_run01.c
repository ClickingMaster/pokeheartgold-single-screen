#pragma force_active on
#include "global.h"
#include "sound.h"
#include "sound_playback.h"
typedef struct MenuState {u8 group,index,choice,player,first,second;u16 pad6,other,busy;u8 match,padd[3];void*entry;u8 menu[0x24],animation[12];u16 update,key;u32 value,extra;}MenuState;
extern void *WiFiPlazaRoom_GetLobby(void*),*ov49_0225A010(void*),*ov49_0225EF84(void*),*ov49_0225EF40(void*,int),*WiFiPlazaRoom_GetObjManager(void*),*WiFiPlazaObj_GetOwnPlayer(void*),*WiFiPlazaObj_FindFacing(void*,void*),*WiFiPlazaWork_GetMyPlayer(void*),*WiFiPlazaWork_GetPlayer(void*,int),*ov49_02264C04(void*,int,int);
extern int ov45_0222B034(void*),ov45_0222B040(void*),ov45_0222B06C(void*),ov49_0225EF88(void*),WiFiPlazaObj_GetParam(void*,int),ov42_022282A4(int),ov49_02264CA8(MenuState*,void*,int),ov45_0222AE74(void*,int),ov45_0222B028(void*),ov49_0225A0DC(void*),ov49_02264E20(MenuState*,void*,void*),ov49_0225A1D4(void*),ov45_0222AAC8(void*),ov49_022652D0(void*),ov49_022652E0(void*),ov49_0225CB70(void*),String_GetLength(void*);
extern void ov49_0225EF8C(void*,int),ov49_0225A0CC(void*),ov49_0226526C(void*),ov45_0222AE64(void*),ov49_02259130(void*,int),WiFiPlazaObj_SetDirection(void*,int),ov49_0225EFF0(void*,int,int),ov45_0222A5E8(void*,int),ov49_0225A428(void*,int,int),ov45_0222B0E8(void*,int),IncrementGameStat47(void*),ov45_0222B118(void*,int),ov49_02264D4C(MenuState*,void*,void*,int,void*,void*),ov49_02264CFC(MenuState*,u8,int,void*,int),ov45_0222AED8(void*,u16),ov49_0225A0BC(void*),ov49_02264EC8(MenuState*,void*),ov49_0225A334(void*,int,int),WiFiPlazaRoom_PrintMessage(void*,void*),ov49_02264F9C(void*,void*,int,int,int),ov49_0225A174(void*,void*,int,int),ov49_02264F10(MenuState*),ov45_0222AB94(void*,u32,int),ov45_0222AF80(void*),ov49_02264F24(MenuState*,void*),ov49_0225A1E4(void*,int,int),ov49_02265260(void*,void*),ov49_02264F1C(MenuState*),ov49_02264E90(MenuState*,void*,void*,int,int),ov45_0222ABD0(void*,u32,int,int),ov45_0222A72C(void*,int),ov49_02264F78(MenuState*,void*),ov49_02265044(void*,void*,int,int,int,int),ov49_0225A1A4(void*,void*,int,int,int,int,int),ov49_02265274(void*,void*,u16,int),ov45_0222AFC4(void*),ov49_02264D14(MenuState*,void*),ov49_02264D30(MenuState*,void*,void*),ov45_0222A704(void*,u16,u32),WiFiPlazaRoom_CloseMessage(void*),WiFiPlazaObj_SetHandler(void *, void *, int),ov49_0225A4D0(void*),ov49_02264CF8(MenuState*),ov49_0225EF68(void*),ov49_02264F60(MenuState*),ov49_0226529C(void*,void*);
extern void*ov49_02264C50(void*,int,int);
int ov49_02262FB4(void*,void*,u32);
#define STATE(x) ov49_0225EF8C(machine,x)
#define MESSAGE(x) WiFiPlazaRoom_PrintMessage(ui,ov49_02264C04(ui,data->player,x))
#define CLEAR() ov49_0225A334(ui,data->player,0)
#define DELAY(x) ov49_02264CFC(data,128,x,machine,37)
#define RESET() ov49_0225A1E4(ui,0,0);ov49_02265260(data->menu,ui);ov49_02264F1C(data)
#define BAD() STATE(32);ov49_0225A0CC(ui)
int ov49_02262FB4(void*machine,void*ui,u32 context){int length;void*text;int option,response,compare,result,value,player;void*entry;void*listing;void*store;void*helper;int action;u32 selection;void*active;void*chosen;u32 current;void*owner=WiFiPlazaRoom_GetLobby(ui);MenuState*data;
 helper=ov49_0225A010(ui);data=ov49_0225EF84(machine);action=ov45_0222B034(owner);selection=ov45_0222B040(owner);
 if(ov45_0222B06C(owner)&&data&&data->busy){STATE(32);ov49_0225A0CC(ui);data->busy=0;}
 if(data){active=WiFiPlazaWork_GetMyPlayer(owner);if(data->entry)chosen=WiFiPlazaWork_GetPlayer(owner,data->player);else chosen=0;}
 if(ov49_0225EF88(machine)&&data->busy&&!chosen){STATE(32);ov49_0225A0CC(ui);data->busy=0;}
 switch(ov49_0225EF88(machine)){
 case 0:{
 data=ov49_0225EF40(machine,80);data->update=0;data->key=0;data->value=0;ov49_0226526C(data->animation);store=WiFiPlazaRoom_GetObjManager(ui);listing=WiFiPlazaObj_GetOwnPlayer(store);entry=WiFiPlazaObj_FindFacing(store,listing);if(!entry){data->busy=0;STATE(38);break;}player=WiFiPlazaObj_GetParam(entry,4);value=ov42_022282A4(WiFiPlazaObj_GetParam(listing,6));ov45_0222AE64(owner);result=ov49_02264CA8(data,owner,player);data->entry=entry;ov49_02259130(entry,0);WiFiPlazaObj_SetDirection(entry,value);ov49_0225EFF0(helper,player,0);PlaySE(1508);ov45_0222A5E8(owner,9);if(result==1){ov49_0225A428(ui,player,0);STATE(1);}else STATE(34);break;}
 case 1:if(ov45_0222AE74(owner,data->player)==1)STATE(2);else STATE(34);break;
 case 2:switch(action){case 2:ov45_0222B0E8(owner,data->player);STATE(3);break;case 0:STATE(34);break;case 1:break;case 3:case 4:default:STATE(32);break;}break;
 case 3:IncrementGameStat47(owner);ov45_0222B118(owner,7);ov49_02264D4C(data,owner,ui,data->first,active,chosen);ov49_02264CFC(data,128,4,machine,37);break;
 case 4:ov49_02264D4C(data,owner,ui,data->second,chosen,active);DELAY(5);ov45_0222AED8(owner,0);break;
 case 5:switch(action){case 2:ov49_0225A0CC(ui);if(ov45_0222B028(owner))STATE(6);else STATE(8);break;case 3:if(!ov49_0225A0DC(ui))ov49_0225A0BC(ui);ov49_02264EC8(data,ui);break;case 0:case 1:case 4:default:BAD();break;}break;
 case 6:if(ov49_02264E20(data,owner,ui)==1)DELAY(7);else STATE(7);break;
 case 7:CLEAR();MESSAGE(40);DELAY(9);break;
 case 8:CLEAR();ov49_0225A334(ui,data->other,1);WiFiPlazaRoom_PrintMessage(ui,ov49_02264C50(ui,data->player,data->other));DELAY(9);break;
 case 9:CLEAR();MESSAGE(46);DELAY(11);break;
 case 10:CLEAR();MESSAGE(54);DELAY(11);break;
 case 11:ov49_02264F9C(data->menu,ui,3,515,0);ov49_0225A174(ui,data->menu,0,0);ov49_02264F10(data);STATE(12);break;
 case 12:{int done;switch(ov49_0225A1D4(ui)){case 0:STATE(13);done=1;break;case 1:STATE(19);ov45_0222AB94(owner,context,WiFiPlazaObj_GetParam(data->entry,4));done=1;break;case 2:ov45_0222AF80(owner);STATE(33);done=1;break;default:ov49_02264F24(data,ui);done=0;break;}if(done==1){RESET();}break;}
 case 13:{compare=ov45_0222AAC8(active);if(compare==ov45_0222AAC8(chosen)){data->match=1;CLEAR();ov49_02264E90(data,owner,ui,1,0);MESSAGE(507);DELAY(18);ov45_0222AED8(owner,4);}else{data->match=0;CLEAR();ov49_02264E90(data,owner,ui,1,1);ov49_02264E90(data,owner,ui,2,0);MESSAGE(508);DELAY(14);}break;}
 case 14:ov49_02264F9C(data->menu,ui,2,518,1);ov49_0225A174(ui,data->menu,0,0);ov49_02264F10(data);STATE(15);break;
 case 15:{int done;switch(ov49_0225A1D4(ui)){case 0:{int tempPlayer;ov45_0222AED8(owner,3);STATE(16);tempPlayer=WiFiPlazaObj_GetParam(data->entry,4);ov45_0222ABD0(owner,context,tempPlayer,ov45_0222AAC8(chosen));done=1;break;}case 1:STATE(10);done=1;break;default:ov49_02264F24(data,ui);done=0;break;}if(done==1){RESET();}break;}
 case 16:CLEAR();ov49_02264E90(data,owner,ui,1,0);MESSAGE(510);DELAY(17);ov45_0222A72C(owner,ov45_0222AAC8(chosen));break;
 case 17:PlaySE(1471);STATE(18);break;
 case 18:switch(action){case 2:ov49_02264F78(data,chosen);if(data->match==1)STATE(33);else STATE(10);ov49_0225A0CC(ui);break;case 3:if(!ov49_0225A0DC(ui))ov49_0225A0BC(ui);ov49_02264EC8(data,ui);break;case 0:case 1:case 4:default:BAD();break;}break;
 case 19:ov45_0222AED8(owner,5);STATE(20);break;
 case 20:ov49_02265044(data->menu,ui,30,4,519,0);ov49_0225A1A4(ui,data->menu,0,0,8,3,23);STATE(21);ov49_02264F10(data);break;
 case 21:{response=ov49_0225A1D4(ui);switch(response){case -2:case -1:ov49_02264F24(data,ui);break;default:ov49_0225A1E4(ui,0,0);ov49_02265260(data->menu,ui);data->group=response;STATE(22);break;}break;}
 case 22:CLEAR();MESSAGE(477+data->group);DELAY(23);break;
 case 23:ov49_02264F9C(data->menu,ui,4,(data->group<<2)+115,0);ov49_0225A174(ui,data->menu,0,0);STATE(24);break;
 case 24:{int group;option=ov49_0225A1D4(ui);switch(option){case 0:case 1:case 2:case 3:RESET();group=data->group<<2;data->index=option+group;STATE(25);break;default:ov49_02264F24(data,ui);break;}break;}
 case 25:ov49_02265274(data->animation,owner,(u16)(data->index+6),2);CLEAR();MESSAGE(357+data->index);DELAY(26);break;
 case 26:CLEAR();MESSAGE(data->group+85);DELAY(27);break;
 case 27:if(ov49_022652D0(data->animation)==1){if(ov49_022652E0(data->animation)==2)STATE(28);else{BAD();}}else if(!ov49_0225A0DC(ui))ov49_0225A0BC(ui);break;
 case 28:switch(action){case 2:ov49_02264F78(data,chosen);STATE(29);ov49_0225A0CC(ui);break;case 3:if(!ov49_0225A0DC(ui))ov49_0225A0BC(ui);ov49_02264EC8(data,ui);break;case 0:case 1:case 4:default:BAD();break;}break;
 case 29:current=(u16)(selection-6);if(selection>=126||selection<6){STATE(35);break;}CLEAR();MESSAGE(current+236);if(current==data->index)DELAY(30);else DELAY(31);break;
 case 30:CLEAR();MESSAGE(51);DELAY(10);break;
 case 31:CLEAR();MESSAGE(50);DELAY(10);break;
 case 32:{u32 wait;ov49_0225A1E4(ui,0,0);ov49_02265260(data->menu,ui);text=ov49_02264C04(ui,data->player,45);WiFiPlazaRoom_PrintMessage(ui,text);data->busy=0;length=String_GetLength(text);wait=((u32)(ov49_0225CB70(ui)*length)>>1)+60;if(wait<128)wait=128;else if(wait>255)wait=255;ov49_02264CFC(data,(u8)wait,38,machine,36);ov45_0222AFC4(owner);break;}
 case 33:CLEAR();MESSAGE(43);data->busy=0;ov49_02264CFC(data,128,38,machine,36);break;
 case 34:MESSAGE(42);data->busy=0;ov49_02264CFC(data,128,38,machine,36);break;
 case 35:CLEAR();MESSAGE(356);data->busy=0;ov49_02264CFC(data,128,38,machine,36);break;
 case 36:ov49_02264D14(data,machine);break;
 case 37:ov49_02264D30(data,machine,ui);break;
 case 38:if(data->update==1)ov45_0222A704(owner,data->key,data->value);ov45_0222AE64(owner);WiFiPlazaRoom_CloseMessage(ui);ov45_0222A5E8(owner,1);{void*tempStore=WiFiPlazaRoom_GetObjManager(ui);WiFiPlazaObj_SetHandler(tempStore, (void*)(WiFiPlazaObj_GetOwnPlayer(tempStore)),1);}if(data->entry){ov49_02259130(data->entry,1);ov49_0225EFF0(helper,WiFiPlazaObj_GetParam(data->entry,4),1);ov49_0225A4D0(ui);}ov49_02264CF8(data);ov49_0225EF68(machine);return 1;
 default:break;
 }
 ov49_02264F60(data);ov49_0226529C(data->animation,owner);return 0;
}
