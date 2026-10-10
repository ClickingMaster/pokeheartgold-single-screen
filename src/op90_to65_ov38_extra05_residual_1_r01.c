#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Entry {
    struct Entry *last, *next;
    char *name, *value;
    int length, isFile;
} Entry;

typedef struct FormEntry {
    struct FormEntry *last, *next;
    char *name, *value;
    int length, isFile;
} FormEntry;

typedef struct Chunk {
    struct Chunk *next;
    char bytes[512];
} Chunk;

typedef struct Buffer {
    volatile int length;
    volatile int position;
    int success;
    char pad[0x10];
    Chunk *chunks;
    char bytes[1024];
} Buffer;

typedef struct Request {
    int reserved, cancel, secure, hostEnd, pathStart, contentLength;
    char pad[8];
    int secureState;
    char *url;
    int port;
    void *user;
    Buffer *buffer;
    void *headers;
    Entry *posts;
    void (*callback)(int, Buffer *, void *);
    int method;
    char boundary[20];
} Request;

typedef struct Job {
    int a, b, id;
    Request *request;
    int socket;
} Job;

typedef struct Poll {
    int fd;
    short events, revents;
} Poll;

extern int ov38_02226C10[2];
extern int ov38_02226224;
extern int ov38_02226214;
extern Job *ov38_02226220;
extern void (*ov38_0222621C)(void *);
extern void *(*ov38_02226218)(int, int);
extern char ov38_02226C58[];
extern char ov38_02226C61[];
extern const char ov38_02225EF4[];
extern const char ov38_02225EFC[];
extern const char ov38_02225F04[];
extern const char ov38_02225F0C[];
extern const char ov38_02225F10[];
extern const char ov38_02225F1C[];
extern const char ov38_02225F24[];
extern const char ov38_02225F28[];
extern const char ov38_02225F2C[];
extern const char ov38_02225F5C[];
extern const char ov38_02225F90[];
extern const char ov38_02224F8C[];
extern const char ov38_02225FA4[];
extern const char ov38_02224FB4[];
extern const char ov38_02225FA8[];
extern const char ov38_02225FB0[];
extern const char ov38_02225FB4[];
extern const char ov38_02225FB8[];
extern const char ov38_02225FC0[];
extern const char ov38_02225FD0[];
extern const char ov38_02225FDC[];
extern const char ov38_02225FE8[];
extern const char ov38_02225FFC[];
extern void ov38_02222038(void);
extern void ov38_0222204C(void);
extern void ov38_02222100(void);
extern void ov38_022230BC(void);
extern void ov38_02223640(void);
extern void ov38_02222B98(Request *);
extern void ov38_02223564(int);
extern Job *ov38_02222008(void);
extern int ov38_02222D6C(Request *);
extern int ov38_0222315C(Request *, int);
extern int ov38_022230DC(Request *);
extern int ov38_022231A8(Request *, int, int, int);
extern int SOC_Poll(Poll *, int, int, int);
extern void ov38_022235A0(int, int *);
extern int ov38_0222334C(const char *);
extern int ov38_02223424(Request *, int, int *, const void *, int);
extern int ov38_02223270(Request *, int, const void *, int, int);
extern Entry *ov38_02221C60(void *);
extern int ov38_022249A0(const char *);
extern int ov38_02224C18(char *, int);
extern int ov38_02224A08(char *, int);
extern int ov38_02223224(Request *, int, void *, int, int);
extern int ov38_02222420(Buffer *, void *, int, int);
extern int ov38_022232E4(const void *, const void *, int);
extern int ov38_02224B80(const char *, int);
extern int ov38_02222210(Buffer *, int, int, int *);
extern int ov38_02222E7C(Buffer *, const char *, int *);
extern int ov38_02222350(Buffer *, int, int, const char *, int);
extern int ov38_02222548(Request *, int, int, int, int);
extern int ov38_0222250C(Buffer *, int);
extern int ov38_022234D8(Request *, int);
extern int ov38_02224A98(const char *, int);
extern int ov38_02222520(Request *, int, int, int);
void ov38_022236A0(void);
#define SEND(p, n)                                           \
    do {                                                     \
        int sent = ov38_02223424(req, socket, &count, p, n); \
        if (sent < 0)                                        \
            goto complete;                                   \
        if (sent == 0)                                       \
            goto retry;                                      \
    } while (0)

void ov38_022236A0(void) {
    int socket = -1;
    int urlLength;
    int length;
    char temp[12];
    int security, count, parsed, status;
    Poll poll;
    char version[3];
    int invalid;
    int host;
    int lastHost;
    int lastPort;
    int multipart;
    int lastSecure;
    int connected;
    int error;
    int n;
    int offset;
    int remaining;
#define ring temp

    security = 0;
    ov38_02226C10[0] = 0;
    ov38_02226C10[1] = 0;
    multipart = 0;
    lastHost = socket;
    lastPort = socket;
    connected = 0;
    lastSecure = 0;
    Entry *entry;
    Request *req;
    Buffer *buffer;
    while (!ov38_02226224) {
        int jobId;

        ov38_02222038();
        {
            Job *job = ov38_02222008();
            if (!job) {
                jobId = -1;
            } else {
                jobId = job->id;
                req = job->request;
                ov38_02226220 = job;
            }
        }
        ov38_0222204C();
        if (jobId < 0) {
            ov38_02222100();
            continue;
        }
        buffer = req->buffer;
        if (req->cancel) {
            goto complete;
        }
        host = ov38_02222D6C(req);
        if (!host) {
            error = 4;
            goto complete;
        }
        invalid = -1;
        {
            int equal = 0;
            if (host == lastHost && req->port == lastPort && req->secure == lastSecure) {
                equal = 1;
            }
            lastPort = req->port;
            lastSecure = req->secure;
            lastHost = host;
            connected &= equal;
        }
retry:
        error = 0;
        if (socket >= 0) {
            poll.events = 9;
            poll.fd = socket;
            if (SOC_Poll(&poll, 1, 52365, 0) <= 0) {
                connected = 0;
            } else if (poll.revents & 0xe0) {
                connected = 0;
            }
        }
        if (!connected) {
            if (socket >= 0) {
                if (ov38_0222315C(req, socket) < 0) {
                    error = 10;
                }
                ov38_022235A0(socket, &security);
                socket = invalid;
                if (error) {
                    goto complete;
                }
            }
            socket = ov38_022230DC(req);
            if (socket < 0) {
                error = 3;
                goto complete;
            }
            if (req->secure) {
                security = req->secureState;
                req->secureState = 0;
            }
            ov38_02222038();
            ov38_02226220->socket = socket;
            ov38_0222204C();
            if (req->cancel) {
                goto complete;
            }
            connected = ov38_022231A8(req, socket, host, req->port) >= 0;
        } else {
            if (req->secure) {
                ov38_02223564(req->secureState);
                req->secureState = 0;
            }
            ov38_022230BC();
            ov38_02222038();
            ov38_02226220->socket = socket;
            ov38_0222204C();
        }
        if (req->cancel) {
            goto complete;
        }
        if (!connected) {
            error = 5;
            goto complete;
        }
        count = 0;
        connected = 0;
        urlLength = ov38_0222334C(req->url);
        error = 10;
        switch (req->method) {
        case 0:
            SEND(ov38_02225EF4, 4);
            break;
        case 1:
            SEND(ov38_02225EFC, 5);
            break;
        case 2:
            SEND(ov38_02225F04, 5);
            break;
        }
        offset = req->pathStart;
        if (urlLength > offset) {
            n = urlLength - offset;
            if (n) {
                SEND(req->url + offset, n);
            }
        } else {
            SEND(ov38_02225F0C, 1);
        }
        SEND(ov38_02225F10, 11);
        parsed = req->secure ? 8 : 7;
        SEND(ov38_02225F1C, 6);
        offset = parsed;
        n = req->hostEnd - offset;
        if (n) {
            SEND(req->url + offset, n);
        }
        SEND(ov38_02225F24, 2);
        entry = ov38_02221C60(&req->headers);
        while (entry) {
            n = ov38_0222334C(entry->name);
            if (n) {
                SEND(entry->name, n);
            }
            SEND(ov38_02225F28, 2);
            n = ov38_0222334C(entry->value);
            if (n) {
                SEND(entry->value, n);
            }
            SEND(ov38_02225F24, 2);
            ov38_0222621C(entry);
            entry = ov38_02221C60(&req->headers);
        }
        if (req->method == 1) {
            length = 0;
            multipart = 0;
            entry = req->posts;
            {
                Entry *p = entry;
                while (p) {
                    if (p->isFile) {
                        multipart = 1;
                        break;
                    }
                    if (p == req->posts->last) {
                        break;
                    }
                    p = p->next;
                }
            }
            if (multipart) {
                while (entry) {
                    length += 22;
                    length += ov38_0222334C(entry->name) + 41;
                    if (entry->isFile) {
                        length += 75;
                    }
                    length = length + 2;
                    length = length + entry->length;
                    length = length + 2;
                    if (entry == req->posts->last) {
                        break;
                    }
                    entry = entry->next;
                }
                length += 24;
                SEND(ov38_02225F2C, 44);
                SEND(req->boundary + 2, 18);
                SEND(ov38_02225F24, 2);
            } else {
                while (entry) {
                    length += ov38_022249A0(entry->name);
                    length++;
                    length += ov38_022249A0(entry->value);
                    if (entry == req->posts->last) {
                        break;
                    }
                    entry = entry->next;
                    length++;
                }
                SEND(ov38_02225F5C, 49);
            }
            SEND(ov38_02225F90, 16);
            n = ov38_02224C18(temp, length);
            parsed = n;
            if (n) {
                SEND(temp, n);
            }
            SEND(ov38_02225F24, 2);
        }
        SEND(ov38_02225F24, 2);
        if (req->method == 1) {
            if (multipart) {
                entry = req->posts;
                while (entry) {
                    SEND(req->boundary, 20);
                    SEND(ov38_02225F24, 2);
                    SEND(ov38_02224F8C, 38);
                    n = ov38_0222334C(entry->name);
                    if (n) {
                        SEND(entry->name, n);
                    }
                    SEND(ov38_02225FA4, 3);
                    if (entry->isFile) {
                        SEND(ov38_02224FB4, 75);
                    }
                    SEND(ov38_02225F24, 2);
                    n = entry->length;
                    if (n) {
                        SEND(entry->value, n);
                    }
                    SEND(ov38_02225F24, 2);
                    if (entry == req->posts->last) {
                        break;
                    }
                    entry = entry->next;
                }
                SEND(req->boundary, 20);
                SEND(ov38_02225FA8, 4);
            } else {
                int index;
                FormEntry *form = (FormEntry *)req->posts;
                while (form) {
                    for (index = 0; form->name[index]; index++) {
                        n = ov38_02224A08(temp, form->name[index]);
                        parsed = n;
                        if (n) {
                            SEND(temp, n);
                        }
                    }
                    SEND(ov38_02225FB0, 1);
                    for (index = 0; form->value[index]; index++) {
                        n = ov38_02224A08(temp, form->value[index]);
                        parsed = n;
                        if (n) {
                            SEND(temp, n);
                        }
                    }
                    if (form == (FormEntry *)req->posts->last) {
                        break;
                    }
                    SEND(ov38_02225FB4, 1);
                    form = form->next;
                }
            }
        }
        if (count > 0) {
            n = ov38_02223270(req, socket, ov38_02226C58, count, 0);
            if (n < 0) {
                goto complete;
            }
            if (!n) {
                goto retry;
            }
        }
        buffer->length = 0;
        ring[0] = 0;
        ring[1] = 0;
        ring[2] = 0;
        ring[3] = 0;
        {
            Chunk *chunk = buffer->chunks;
            error = 7;
            count = 0;
            do {
                if (req->cancel) {
                    goto complete;
                }
                if (count < 1024) {
                    n = ov38_02223224(req, socket, buffer->bytes + count, 1, 0);
                    ring[count & 3] = buffer->bytes[count];
                } else {
                    offset = count & 511;
                    if (!offset) {
                        if (chunk) {
                            chunk->next = ov38_02226218(516, 4);
                            chunk = chunk->next;
                        } else {
                            chunk = ov38_02226218(516, 4);
                            buffer->chunks = chunk;
                        }
                        if (!chunk) {
                            error = 1;
                            goto complete;
                        }
                        chunk->next = NULL;
                    }
                    n = ov38_02223224(req, socket, chunk->bytes + offset, 1, 0);
                    ring[count & 3] = chunk->bytes[offset];
                }
                if (n <= 0) {
                    error = 10;
                    goto complete;
                }
                count += n;
            } while (ring[(count - 4) & 3] != 13 || ring[(count - 3) & 3] != 10 || ring[(count - 2) & 3] != 13 || ring[(count - 1) & 3] != 10);
        }
        buffer->length = count;
        if (!buffer->length) {
            goto complete;
        }
        if (!ov38_02222420(buffer, ov38_02226C58, 0, 14)) {
            goto complete;
        }
        if (ov38_022232E4(ov38_02226C58, ov38_02225FB8, 5) || ov38_02226C58[8] != ' ') {
            goto complete;
        }
        if (ov38_02224B80(ov38_02226C61, 3) < 0) {
            goto complete;
        }
        if (ov38_02222210(buffer, 12, buffer->length, &status) < 0) {
            goto complete;
        }
        remaining = ov38_02222E7C(buffer, ov38_02225FC0, &parsed);
        if (!remaining) {
            error = 0;
            goto complete;
        }
        if (remaining > 1024) {
            goto complete;
        }
        if (remaining > 0) {
            if (!ov38_02222420(buffer, ov38_02226C58, parsed, remaining)) {
                goto complete;
            }
            remaining = ov38_02224B80(ov38_02226C58, remaining);
            if (remaining < 0) {
                goto complete;
            }
            req->contentLength = remaining;
        } else {
            req->contentLength = -1;
        }
        connected = ov38_02222E7C(buffer, ov38_02225FD0, &parsed);
        if (!connected) {
            goto complete;
        }
        if (connected < 0) {
            version[0] = ov38_02226C58[5];
            version[1] = ov38_02226C58[7];
            version[2] = 0;
            connected = ov38_02224B80(version, 2) >= 11;
        } else if (connected > 1024) {
            connected = 0;
        } else {
            connected = ov38_02222350(buffer, parsed, parsed + connected, ov38_02225FDC, 0) == 0;
        }
        n = ov38_02222E7C(buffer, ov38_02225FE8, &parsed);
        if (!n) {
            goto complete;
        }
        {
            int chunked;
            if (n > 1024) {
                chunked = 0;
            } else if (n > 0) {
                chunked = ov38_02222350(buffer, parsed, parsed + n, ov38_02225FFC, 59) == 0;
            } else {
                chunked = 0;
            }
            if (req->method == 2) {
                goto complete;
            }
            if (remaining >= 0) {
                while (remaining > 0 && !ov38_0222250C(buffer, buffer->position)) {
                    n = ov38_02222548(req, socket, buffer->position, remaining, 0);
                    if (n < 0) {
                        goto complete;
                    }
                    if (!n) {
                        break;
                    }
                    remaining -= n;
                    buffer->position += n;
                }
                if (remaining) {
                    error = ov38_0222250C(buffer, buffer->position) ? 6 : 10;
                    goto complete;
                }
                error = 0;
                goto complete;
            }
            error = 10;
            if (chunked) {
                int chunkRemaining;
                for (;;) {
                    ring[0] = 0;
                    ring[1] = 0;
                    count = 0;
                    while (count < 1024) {
                        n = ov38_02223224(req, socket, ov38_02226C58 + count, 1, 0);
                        if (n < 0) {
                            goto complete;
                        }
                        ring[count & 1] = ov38_02226C58[count];
                        if (ov38_02226C58[count] == ';' || (ov38_02226C58[count] == 10 && ring[(count - 1) & 1] == 13)) {
                            if (ov38_02226C58[count] == 10) {
                                parsed = count - 1;
                            } else {
                                parsed = count;
                                if (ov38_022234D8(req, socket) <= 0) {
                                    goto complete;
                                }
                            }
                            if (!parsed) {
                                goto complete;
                            }
                            chunkRemaining = ov38_02224A98(ov38_02226C58, parsed);
                            if (chunkRemaining < 0) {
                                goto complete;
                            }
                            break;
                        }
                        count++;
                    }
                    if (count == 1024) {
                        error = 7;
                        goto complete;
                    }
                    if (chunkRemaining > 0) {
                        while (chunkRemaining > 0) {
                            n = ov38_02222548(req, socket, buffer->position, chunkRemaining, 0);
                            if (n <= 0) {
                                goto complete;
                            }
                            buffer->position += n;
                            chunkRemaining -= n;
                            if (!chunkRemaining) {
                                if (ov38_02223224(req, socket, ov38_02226C58, 1, 0) <= 0) {
                                    goto complete;
                                }
                                if (ov38_02223224(req, socket, ov38_02226C58, 1, 0) <= 0) {
                                    goto complete;
                                }
                            }
                        }
                    } else {
                        ov38_022234D8(req, socket);
                        error = 0;
                        goto complete;
                    }
                }
            } else {
                if (ov38_0222250C(buffer, buffer->position)) {
                    goto complete;
                }
                do {
                    n = ov38_02222520(req, socket, buffer->position, 0);
                    if (n < 0) {
                        goto complete;
                    }
                    if (!n) {
                        error = 0;
                        goto complete;
                    }
                    buffer->position += n;
                    if (ov38_0222250C(buffer, buffer->position)) {
                        n = ov38_02223224(req, socket, ov38_02226C58, 1, 0);
                        if (n < 0) {
                            goto complete;
                        }
                        if (n) {
                            error = 6;
                            goto complete;
                        }
                    }
                } while (!ov38_0222250C(buffer, buffer->position));
            }
        }
complete:
        ov38_02222038();
        ov38_0222621C(ov38_02226220);
        ov38_02226220 = NULL;
        ov38_0222204C();
        if (req->cancel) {
            error = 8;
        }
        if (socket >= 0 && (!connected || error)) {
            if (ov38_0222315C(req, socket) < 0) {
                error = 10;
            }
            ov38_022235A0(socket, &security);
            socket = -1;
            connected = 0;
        }
        if (!error) {
            buffer->success = 1;
        } else {
            buffer->success = 0;
            ov38_02226214 = error;
        }
        {
            void *user = req->user;
            void (*callback)(int, Buffer *, void *) = req->callback;
            ov38_02222B98(req);
            callback(error, buffer, user);
        }
    }
    if (socket >= 0) {
        ov38_0222315C(req, socket);
        ov38_022235A0(socket, &security);
    }
    ov38_02223640();
}
