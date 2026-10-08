#pragma force_active on
#include "global.h"
typedef struct {u8 pad[9];u8 state;u16 actions;u8 base[2];u8 mood;u8 values[5];} W40082State;
extern const s8 _020F68DE[][5];
extern void sub_020324F4(void *,s8 *);
void sub_02032354(W40082State *);
// @fn sub_02032354
void sub_02032354(W40082State *s){
 s8 values[5],order[5];int n;u8 total,action,max,index;u16 actions=s->actions;int i,j; u8 reduce;
 sub_020324F4(s->base,values);
 for(n=0;n<5;n++){
  reduce=0;action=actions&7;if(action==0)break;
  total=0;index=0;max=0;action--;
  for(i=0;i<5;i++)order[i]=i;
  for(i=0;i<2;i++)for(j=i+1;j<5;j++){
   s8 tmp;
   if(values[order[i]]>values[order[j]]||(values[order[i]]==values[order[j]]&&order[i]<order[j]))continue;
   tmp=order[i];order[i]=order[j];order[j]=tmp;
  }
  {
   s8 value;
   const s8 *row;
   s8 *v;
   s8 change;
   s8 old;
   row=_020F68DE[action];
   v=&values[0];
   for(i=0;i<5;i++){
    change=*row;
    old=*v;
    value=old+change;
    if(change>0&&values[order[0]]!=0&&order[0]!=i&&values[order[1]]!=0&&order[1]!=i)reduce=1;
    if(value>=63)value=63;else if(value<0)value=0;
    if((action==6||value<=old)&&value>max){max=value;index=i;}
    *v=value;total+=value;
    row++;v++;
   }
  }
  actions>>=3;
  if(reduce){if(s->mood<10)s->mood=0;else s->mood-=10;}
  if(action!=5&&total>100)values[index]-=total-100;
 }
 for(i=0;i<5;i++)s->values[i]=values[i];
 s->state=3;s->actions=0;
}
