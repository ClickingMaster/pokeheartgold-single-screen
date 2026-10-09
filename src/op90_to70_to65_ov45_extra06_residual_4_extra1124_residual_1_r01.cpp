#include <nitro/types.h>
#include <nitro/code32.h>
struct Vec;
extern "C" Vec *ov45_022379D4(Vec *);
struct Vec {
 char *ptr;u32 size,cap;
 Vec():ptr(0),size(0),cap(0){}
 void Destroy(){ov45_022379D4(this);}
};
struct Date {u32 year,month,day,week;};
struct Time {u32 hour,minute,second;};
struct Stamp {s64 v;};
struct DateTime {Date date;Time time;};
struct Work {u8 pad[0x134];u8 list[0x148-0x134];int valid;Stamp stamp;
 void SetStamp(const Stamp &t){valid=1;stamp=t;}
};
extern "C" Work *_022577C4;
extern "C" char ov45_022550E4[];
extern "C" int ov45_02233FE4(const char *,Vec *);
extern "C" void ov00_021ECB94(Date *,Time *);
extern "C" s64 RTC_ConvertDateTimeToSecond(const Date *,const Time *);
extern "C" void ov45_02233434(int,int,char *,Stamp *,int);
extern "C" char *ov45_02231C78(Vec *);
extern "C" void ov45_02232D8C(void *,int *);
extern "C" void ov45_0223683C(int,int,int,int,const char *);
#pragma force_active on
// @fn ov45_0223683C
extern "C" void ov45_0223683C(int result,int unused1,int unused2,int unused3,const char *value){
 if(result==0)return;
 Vec data;
 if(ov45_02233FE4(value,&data)==0||data.size!=8){
  Work *w=_022577C4;
  DateTime now;
  ov00_021ECB94(&now.date,&now.time);
  w->stamp.v=RTC_ConvertDateTimeToSecond(&now.date,&now.time);
  Stamp s;
  s=w->stamp;
  ov45_02233434(-1,0,ov45_022550E4,&s,8);
  data.Destroy();
  return;
 }
 {
  Stamp s=*(Stamp *)ov45_02231C78(&data);
  Work *w=_022577C4;
  w->valid=1;
  w->stamp=s;
  int state=5;
  ov45_02232D8C(_022577C4->list,&state);
 }
 data.Destroy();
}
