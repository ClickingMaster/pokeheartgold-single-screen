#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    s32 length;
    u8 *bytes;
} KeyPart;

typedef struct {
    u8 unused[0x458];
    u32 signatureType, keyType;
    u8 *encodedBegin, *encodedEnd;
    u8 gap[0x1c];
    KeyPart modulusRef, exponentRef;
    u8 modulus[256];
    u32 modulusLength;
    u8 exponent[8];
    u32 exponentLength;
    u8 *signature;
    u32 signatureLength;
    u8 subject, keyEncoding, nameType, valid;
    char issuer[256], subjectName[256], commonName[80];
    u8 gapTime[12];
    u32 now;
} Certificate;

extern const char *_021106B8[6];
extern int cert_item_len(u8 **);
extern void make_dn(char *, char *, int);
extern u32 parse_time(const u8 *, int);
int cert_item(Certificate *, u8 **, int, int, int);
typedef char modulus_offset[offsetof(Certificate, modulus) == 0x494 ? 1 : -1];
typedef char exponent_offset[offsetof(Certificate, exponent) == 0x598 ? 1 : -1];
typedef char name_offset[offsetof(Certificate, commonName) == 0x7b0 ? 1 : -1];
typedef char timestamp_offset[offsetof(Certificate, now) == 0x80c ? 1 : -1];

int cert_item(Certificate *certificate, u8 **input, int nesting, int ordinal, int mode) {
    u8 *payload = *input;
    int tag = *payload++;
    int length = cert_item_len(&payload);
    if (length < 0 || length > 2000) {
        return 1;
    }
    switch (tag & 31) {
    case 2:
        if (certificate->keyEncoding) {
            if (ordinal == 0) {
                while (*payload == 0) {
                    payload++;
                    length--;
                }
                switch (mode) {
                case 0:
                    if (length <= 256) {
                        MI_CpuCopy8(payload, certificate->modulus, length);
                        certificate->modulusLength = length;
                    }
                    break;
                case 2:
                    certificate->modulusRef.length = length;
                    certificate->modulusRef.bytes = payload;
                    break;
                }
            } else if (ordinal == 1) {
                while (*payload == 0) {
                    payload++;
                    length--;
                }
                switch (mode) {
                case 0:
                    if (length <= 8) {
                        MI_CpuCopy8(payload, certificate->exponent, length);
                        certificate->exponentLength = length;
                    }
                    break;
                case 2:
                    certificate->exponentRef.length = length;
                    certificate->exponentRef.bytes = payload;
                    break;
                }
            }
        }
        payload += length;
        break;
    case 3:
        if (nesting == 1 && mode != 2) {
            certificate->signature = payload + 1;
            certificate->signatureLength = (u32)length - 1;
        }
        if (certificate->keyEncoding) {
            payload++;
            if (cert_item(certificate, &payload, nesting, 0, mode)) {
                return 1;
            }
            certificate->keyEncoding = 0;
        } else {
            payload += length;
        }
        break;
    case 6: {
        int identifier = 0;
        u8 *encoded = payload;
        for (; identifier < 6; identifier++) {
            const char *name = _021106B8[identifier];
            if (memcmp(encoded, name, strlen(name)) == 0) {
                switch (identifier) {
                case 1:
                case 2:
                    if (mode == 0) {
                        certificate->keyType = identifier;
                    }
                    certificate->keyEncoding = identifier;
                    break;
                case 3:
                case 4:
                    if (mode != 2) {
                        certificate->signatureType = identifier;
                    }
                    break;
                case 5:
                    if (mode != 2) {
                        certificate->nameType = identifier;
                    }
                    break;
                }
                break;
            }
        }
        payload += length;
        break;
    }
    case 12:
    case 19:
    case 20:
    case 22:
        if (mode != 2) {
            if (certificate->subject) {
                make_dn(certificate->subjectName, (char *)payload, length);
                if (certificate->nameType == 5 && length <= 79) {
                    MI_CpuCopy8(payload, certificate->commonName, length);
                    certificate->commonName[length] = 0;
                }
            } else {
                make_dn(certificate->issuer, (char *)payload, length);
            }
        }
        certificate->nameType = 0;
        payload += length;
        break;
    case 23:
    case 24:
        if (mode != 2) {
            u32 timestamp = parse_time(payload, tag & 31);
            if (ordinal == 0) {
                if (certificate->now >= timestamp) {
                    certificate->valid = 1;
                }
            } else if (certificate->now > timestamp) {
                certificate->valid = 0;
            }
        }
        payload += length;
        certificate->subject = 1;
        break;
    case 16: {
        u8 *end;
        int child;
        if (nesting == 0 && ordinal == 0 && mode != 2) {
            certificate->encodedBegin = payload;
        }
        end = payload + length;
        child = 0;
        while (payload < end) {
            int result = cert_item(certificate, &payload, nesting + 1, child, mode);
            child++;
            if (result) {
                return 1;
            }
        }
        if (nesting == 1 && ordinal == 0 && mode != 2) {
            certificate->encodedEnd = payload;
        }
        break;
    }
    case 17: {
        u8 *end = payload + length;
        while (payload < end) {
            if (cert_item(certificate, &payload, nesting + 1, 0, mode)) {
                return 1;
            }
        }
        break;
    }
    default:
        if (tag == 0xa0) {
            u8 *end = payload + length;
            while (payload < end) {
                if (cert_item(certificate, &payload, nesting + 1, 0, mode)) {
                    return 1;
                }
            }
        } else {
            payload += length;
        }
        break;
    }
    *input = payload;
    return 0;
}
