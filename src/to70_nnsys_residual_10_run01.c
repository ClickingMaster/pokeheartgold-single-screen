#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#pragma force_active on
typedef struct Node {u32 start,size;struct Node *prev,*next;} Node;
BOOL TryToMergeBlockRegion_(Node **,Node **,u32 *);
static inline void Remove(Node **head,Node *n){Node *p=n->prev,*q=n->next;if(p)p->next=q;else *head=q;if(q)q->prev=p;}
static inline void Push(Node **head,Node *n){if(*head)(*head)->prev=n;n->next=*head;n->prev=NULL;*head=n;}
BOOL TryToMergeBlockRegion_(Node **free,Node **unused,u32 *region){Node *n=*free,*next;BOOL done=FALSE;while(n){next=n->next;if(n->start==region[1]){region[1]=n->start+n->size;done|=TRUE;Remove(free,n);Push(unused,n);}if(n->start+n->size==region[0]){region[0]=n->start;Remove(free,n);done|=TRUE;Push(unused,n);}n=next;}return done;}
