typedef unsigned char u8;
typedef unsigned short u16;

typedef struct List {
    void *head;
    void *tail;
    u16 count;
    u16 offset;
} List;

typedef struct Link {
    void *prev;
    void *next;
} Link;

#define GetLink(list, object) ((Link *)((u8 *)(object) + (list)->offset))

void NNS_FndInitList(List *list, u16 offset);
void SetFirstObject(List *list, void *object);
void NNS_FndAppendListObject(List *list, void *object);
void NNS_FndPrependListObject(List *list, void *object);
void NNS_FndInsertListObject(List *list, void *target, void *object);
void NNS_FndRemoveListObject(List *list, void *object);
void *NNS_FndGetNextListObject(List *list, void *object);
void *NNS_FndGetPrevListObject(List *list, void *object);

void NNS_FndInitList(List *list, u16 offset) {
    list->head = 0;
    list->tail = 0;
    list->count = 0;
    list->offset = offset;
}

void SetFirstObject(List *list, void *object) {
    Link *link = GetLink(list, object);
    link->next = 0;
    link->prev = 0;
    list->head = object;
    list->tail = object;
    list->count++;
}

void NNS_FndAppendListObject(List *list, void *object) {
    Link *link;
    if (!list->head) {
        SetFirstObject(list, object);
        return;
    }
    link = GetLink(list, object);
    link->prev = list->tail;
    link->next = 0;
    GetLink(list, list->tail)->next = object;
    list->tail = object;
    list->count++;
}

void NNS_FndPrependListObject(List *list, void *object) {
    Link *link;
    if (!list->head) {
        SetFirstObject(list, object);
        return;
    }
    link = GetLink(list, object);
    link->prev = 0;
    link->next = list->head;
    GetLink(list, list->head)->prev = object;
    list->head = object;
    list->count++;
}
