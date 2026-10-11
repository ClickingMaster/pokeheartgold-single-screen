#include <string.h>

#include "global.h"

#include "wifi_setup_internal.h"

extern char ov13_02245AC4[]; // SSID of the setup access point

static inline int NameEqual(const char *name, const char *wanted) {
    return strlen(name) == strlen(wanted) && !memcmp(name, wanted, strlen(wanted));
}

static inline char *CopyName(char *name, const WifiSetupApEntry *entry) {
    memcpy(name, entry->ssid, 32);
    name[entry->ssidLength] = 0;
    return name;
}

#pragma force_active on

// Compares two scan results. Picks the first access point of current that is also in previous
// (same SSID and BSSID) but has just dropped privacy, or else the open setup access point
// (ov13_02245AC4) when it is new in current. Returns 1 and its index in *selected if one was found.
int ov13_02224164(const WifiSetupApEntryList *current, const WifiSetupApEntryList *previous, u32 *selected) {
    int changed = 0;
    int result = 0;
    int newHas;
    int oldHas;
    u32 count;
    u32 i;
    u32 j;
    const WifiSetupApEntry *left = current->entries;
    const WifiSetupApEntry *right = previous->entries;

    i = changed;
    for (; i < current->count; i++) {
        char newName[34] = { 0 };
        memcpy(newName, left->ssid, 32);
        newName[left->ssidLength] = 0;
        count = previous->count;
        j = 0;
        if (count > j) {
            u32 length = left->ssidLength;
            do {
                if (length == 0 || length > 32) {
                    break;
                }
                // a one-character SSID of NUL or space is hidden (compared unsigned)
                if (length == 1 && (((const u8 *)left->ssid)[0] == 0 || ((const u8 *)left->ssid)[0] == ' ')) {
                    break;
                }
                if (!memcmp(newName, right->ssid, strlen(newName)) && !memcmp(left->bssid, right->bssid, 6) && left->privacy != right->privacy && left->privacy == 0) {
                    changed = 1;
                    break;
                }
                j++;
                right++;
            } while (j < count);
        }
        if (changed) {
            break;
        }
        right = previous->entries;
        left++;
    }
    if (!changed) {
        char oldName[34] = { 0 };
        newHas = 0;
        oldHas = 0;
        left = current->entries;
        right = previous->entries;
        for (j = 0; j < previous->count; j++, right++) {
            memcpy(oldName, right->ssid, 32);
            oldName[right->ssidLength] = 0;
            if (!memcmp(oldName, ov13_02245AC4, strlen(ov13_02245AC4)) && right->privacy == 0) {
                oldHas = 1;
                break;
            }
        }
        for (i = 0; i < current->count; i++, left++) {
            if (NameEqual(CopyName(oldName, left), ov13_02245AC4) && left->privacy == 0) {
                newHas = 1;
                break;
            }
        }
        if (newHas && !oldHas) {
            changed = 1;
        }
    }
    if (changed) {
        *selected = i;
        result = 1;
    }
    return result;
}

#pragma force_active reset
