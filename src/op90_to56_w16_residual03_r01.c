#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct {u32 len;u8 name[32];u8 pad[4];u8 address[6];u16 mode;} Entry;
typedef struct {u32 count;Entry entries[];} List;
extern char ov13_02245AC4[];
static inline int NameEqual(const char *name,const char *wanted){return strlen(name)==strlen(wanted)&&!memcmp(name,wanted,strlen(wanted));}
static inline char *CopyName(char *name,const Entry *entry){memcpy(name,entry->name,32);name[entry->len]=0;return name;}
int ov13_02224164(const List*,const List*,u32*);
// @fn ov13_02224164
int ov13_02224164(const List*a,const List*b,u32*out){int changed=0,result=0,newHas,oldHas;u32 count,i,j;Entry const *left=a->entries,*right=b->entries;i=changed;for(;i<a->count;i++){char newName[34]={0};memcpy(newName,left->name,32);newName[left->len]=0;count=b->count;j=0;if(count>j){u32 length=left->len;do{if(length==0||length>32)break;if(length==1&&(left->name[0]==0||left->name[0]==' '))break;if(!memcmp(newName,right->name,strlen(newName))&&!memcmp(left->address,right->address,6)&&left->mode!=right->mode&&left->mode==0){changed=1;break;}j++;right++;}while(j<count);}if(changed)break;right=b->entries;left++;}if(!changed){char oldName[34]={0};newHas=0;oldHas=0;left=a->entries;right=b->entries;for(j=0;j<b->count;j++,right++){memcpy(oldName,right->name,32);oldName[right->len]=0;if(!memcmp(oldName,ov13_02245AC4,strlen(ov13_02245AC4))&&right->mode==0){oldHas=1;break;}}for(i=0;i<a->count;i++,left++){if(NameEqual(CopyName(oldName,left),ov13_02245AC4)&&left->mode==0){newHas=1;break;}}if(newHas&&!oldHas)changed=1;}if(changed){*out=i;result=1;}return result;}
