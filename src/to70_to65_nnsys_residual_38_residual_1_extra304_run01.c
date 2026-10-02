#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Link {u8 pad[16];struct Link *next;} Link;
int removeLink_(Link **,Link *);
int removeLink_(Link **head,Link *entry){Link *next;Link *current=*head;if(!current)return 0;if(current==entry){*head=(*head)->next;entry->next=0;return 1;}next=current->next;while(next){if(next==entry){current->next=next->next;next->next=0;return 1;}current=next;next=next->next;}return 0;}
