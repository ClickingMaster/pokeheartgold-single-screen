#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Link {void *prev,*next;} Link;
typedef struct List {void *head,*tail;u16 count,offset;} List;
void NNS_FndAppendListObject(List*,void*);void NNS_FndPrependListObject(List*,void*);void NNS_FndInsertListObject(List*,void*,void*);
void NNS_FndInsertListObject(List *list,void *next,void *object){if(next==0){NNS_FndAppendListObject(list,object);}else if(next==list->head){NNS_FndPrependListObject(list,object);}else{u32 offset=list->offset;Link *ln=(Link*)((u8*)next+offset);Link *lo=(Link*)((u8*)object+offset);void *previous=ln->prev;lo->prev=previous;lo->next=next;((Link*)((u8*)previous+offset))->next=object;((Link*)((u8*)next+list->offset))->prev=object;list->count++;}}
