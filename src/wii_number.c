#include "wii_number.h"

#include "global.h"

#include "constants/charcode.h"

#include "heap.h"
#include "pm_string.h"

// A Wii number is 16 digits; its Wii message address is "w<16 digits>@wii.com".
typedef struct WiiNumberDigitChar {
    u16 charCode;
    s8 digit;
} WiiNumberDigitChar;

extern s8 sWiiAddressPrefix[];
extern s8 sWiiAddressDomain[];
extern WiiNumberDigitChar sWiiNumberDigitChars[];

#pragma force_active on

void WiiNumber_ToWiiAddress(String *wiiNumber, char *address, enum HeapID heapID) {
    int len = 0;
    int i = 0;
    u16 *chars;
    u32 j;

    do {
        address[len++] = sWiiAddressPrefix[i++];
    } while (sWiiAddressPrefix[i] != 0);
    chars = Heap_Alloc(heapID, 200);
    CopyStringToU16Array(wiiNumber, chars, 100);
    for (i = 0;; i++) {
        if (chars[i] == EOS) {
            address[len] = 0;
            break;
        }
        for (j = 0; j < 10; j++) {
            if (sWiiNumberDigitChars[j].charCode == chars[i]) {
                address[len] = sWiiNumberDigitChars[j].digit;
                break;
            }
        }
        if (j >= 10) {
            GF_AssertFail();
        }
        len++;
    }
    i = 0;
    do {
        address[len++] = sWiiAddressDomain[i++];
    } while (sWiiAddressDomain[i] != 0);
    address[len] = 0;
    Heap_Free(chars);
}

// TRUE if the string is empty or only "0"s.
BOOL WiiNumber_IsZero(String *wiiNumber, enum HeapID heapID) {
    u16 *chars;
    u16 *c;
    BOOL zero = TRUE;

    chars = Heap_Alloc(heapID, 200);
    CopyStringToU16Array(wiiNumber, chars, 100);
    if (*chars != EOS) {
        c = chars;
        do {
            if (*c != CHAR_0) {
                zero = FALSE;
                break;
            }
            c++;
        } while (*c != EOS);
    }
    Heap_Free(chars);
    return zero;
}

// Reads the 4 groups of 4 digits of the Wii number from its Wii message address.
void WiiNumber_ParseDigitGroups(char *address, u32 *groups, enum HeapID unused) {
    int i;
    int j;
    int pos = 0;
    char *c;

    for (i = 0; i < 4; i++) {
        *groups = 0;
        for (j = 0; j < 4; j++) {
            while ((c = address + pos, *c < '0') || *c > '9') {
                if (*c == 0) {
                    GF_ASSERT(*c != 0);
                    break;
                }
                pos++;
            }
            *groups *= 10;
            *groups += *c - '0';
            pos++;
        }
        groups++;
    }
}

#pragma force_active reset
