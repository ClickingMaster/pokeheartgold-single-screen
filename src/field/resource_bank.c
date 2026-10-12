#include "field/resource_bank.h"

#include "global.h"

typedef struct ResourceBankEntry {
    void *data; // NULL while the slot is free
    u32 id;
} ResourceBankEntry;

struct ResourceBank {
    enum HeapID parentHeapID;
    enum HeapID heapID;
    u32 capacity;
    u32 heapSize;
    u32 entriesSize;
    ResourceBankEntry *entries; // stored right after the bank itself
};

void *ResourceBank_AllocBlock(ResourceBank *bank, u32 size, BOOL atEnd);
void ResourceBank_FreeBlock(void *block);
ResourceBankEntry *ResourceBank_FindEntry(ResourceBank *bank, u32 id);
ResourceBankEntry *ResourceBank_FindFreeEntry(ResourceBank *bank);
void ResourceBankEntry_Set(ResourceBankEntry *entry, u32 id, void *data);
void ResourceBankEntry_Clear(ResourceBankEntry *entry);

ResourceBank *ResourceBank_Create(enum HeapID parentHeapID, enum HeapID heapID, u32 heapSize, u32 capacity) {
    ResourceBank *bank;
    u32 entriesSize = capacity * sizeof(ResourceBankEntry);
    u32 totalSize = heapSize + entriesSize + sizeof(ResourceBank);

    if (Heap_Create(parentHeapID, heapID, totalSize) != TRUE) {
        GF_AssertFail();
    }
    heapSize = totalSize - heapSize;
    bank = Heap_Alloc(heapID, heapSize);
    if (bank == NULL) {
        GF_AssertFail();
    }
    memset(bank, 0, heapSize);
    bank->parentHeapID = parentHeapID;
    bank->heapID = heapID;
    bank->capacity = capacity;
    bank->heapSize = totalSize;
    bank->entriesSize = entriesSize;
    bank->entries = (ResourceBankEntry *)(bank + 1);
    return bank;
}

void ResourceBank_Destroy(ResourceBank *bank) {
    u32 count = bank->capacity;
    ResourceBankEntry *entry = bank->entries;

    while (count != 0) {
        if (entry->data != NULL) {
            ResourceBank_Free(bank, entry->id);
        }
        count--;
        entry++;
    }
    {
        enum HeapID heapID = bank->heapID;
        Heap_Free(bank);
        Heap_Destroy(heapID);
    }
}

void *ResourceBank_Alloc(ResourceBank *bank, u32 id, u32 size, BOOL atEnd) {
    ResourceBankEntry *entry = ResourceBank_FindFreeEntry(bank);
    void *data;

    if (entry == NULL) {
        GF_AssertFail();
    }
    data = ResourceBank_AllocBlock(bank, size, atEnd);
    ResourceBankEntry_Set(entry, id, data);
    return data;
}

void ResourceBank_Free(ResourceBank *bank, u32 id) {
    ResourceBankEntry *entry = ResourceBank_FindEntry(bank, id);
    if (entry == NULL) {
        GF_AssertFail();
    }
    ResourceBank_FreeBlock(entry->data);
    ResourceBankEntry_Clear(entry);
}

void *ResourceBank_Get(ResourceBank *bank, u32 id) {
    ResourceBankEntry *entry = ResourceBank_FindEntry(bank, id);
    if (entry == NULL) {
        GF_AssertFail();
    }
    return entry->data;
}

BOOL ResourceBank_Contains(ResourceBank *bank, u32 id) {
    if (ResourceBank_FindEntry(bank, id) != NULL) {
        return TRUE;
    }
    return FALSE;
}

void *ResourceBank_LoadFromNarc(ResourceBank *bank, u32 id, NARC *narc, u32 fileId, BOOL atEnd) {
    u32 size = NARC_GetMemberSize(narc, fileId);
    void *data = ResourceBank_Alloc(bank, id, size, atEnd);
    NARC_ReadWholeMember(narc, fileId, data);
    return data;
}

void *ResourceBank_AllocBlock(ResourceBank *bank, u32 size, BOOL atEnd) {
    void *data;
    if (atEnd == FALSE) {
        data = Heap_Alloc(bank->heapID, size);
    } else {
        data = Heap_AllocAtEnd(bank->heapID, size);
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data;
}

void ResourceBank_FreeBlock(void *block) {
    Heap_Free(block);
}

ResourceBankEntry *ResourceBank_FindEntry(ResourceBank *bank, u32 id) {
    u32 count = bank->capacity;
    ResourceBankEntry *entry = bank->entries;

    while (count != 0) {
        if (entry->data != NULL && entry->id == id) {
            return entry;
        }
        entry++;
        count--;
    }
    return NULL;
}

ResourceBankEntry *ResourceBank_FindFreeEntry(ResourceBank *bank) {
    u32 count = bank->capacity;
    ResourceBankEntry *entry = bank->entries;

    while (count != 0) {
        if (entry->data == NULL) {
            return entry;
        }
        entry++;
        count--;
    }
    return NULL;
}

void ResourceBankEntry_Set(ResourceBankEntry *entry, u32 id, void *data) {
    entry->data = data;
    entry->id = id;
}

void ResourceBankEntry_Clear(ResourceBankEntry *entry) {
    entry->data = NULL;
    entry->id = 0;
}
