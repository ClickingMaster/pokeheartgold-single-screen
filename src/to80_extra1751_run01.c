#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Cert {
    int length;
    void *bytes;
} Cert;

typedef struct SSL {
    u8 p0[0x30];
    u8 session, resumed;
    u16 cipher;
    u8 p34[0x20];
    u8 random[32], sessionId[32];
    u8 p94[0x3c1];
    u8 error;
    u8 p456[0x3ca];
    Cert *cert;
} SSL;

typedef struct Conn {
    u8 pad[12];
    SSL *ssl;
} Conn;

extern void *(*_021D4634)(int);
extern void (*_021D4660)(void *);

extern struct {
    u32 first, count;
} _021D5E60;

extern u32 date2sec(void);
extern void set_random(void *, int);
extern void update_digest(SSL *, void *, int);
extern int CPSi_TcpWrite2Raw(void *, int, int, int, Conn *);
int send_server_hello(Conn *);

int send_server_hello(Conn *connection) {
    SSL *ssl = connection->ssl;
    u8 *buffer, *cursor;
    Cert *cert = ssl->cert;
    int length = cert ? cert->length : 0;
    u32 time = date2sec();
    ssl->random[0] = time >> 24;
    ssl->random[1] = time >> 16;
    ssl->random[2] = time >> 8;
    ssl->random[3] = time;
    set_random(ssl->random + 4, 28);
    buffer = _021D4634(length + 157);
    if (!buffer) {
        ssl->error = 9;
        return 1;
    }
    buffer[5] = 2;
    buffer[6] = 0;
    buffer[7] = 0;
    buffer[8] = 70;
    buffer[9] = 3;
    buffer[10] = 0;
    MI_CpuCopy8(ssl->random, buffer + 11, 32);
    buffer[43] = 32;
    if (ssl->session) {
        MI_CpuCopy8(ssl->sessionId, buffer + 44, 32);
        ssl->resumed = 1;
        cursor = buffer + 76;
    } else {
        u32 count;
        set_random(buffer + 44, 28);
        cursor = buffer + 76;
        count = _021D5E60.count;
        buffer[72] = count >> 24;
        buffer[73] = count >> 16;
        buffer[74] = count >> 8;
        buffer[75] = count;
        MI_CpuCopy8(cursor - 32, ssl->sessionId, 32);
        count = _021D5E60.count;
        ssl->resumed = 0;
        _021D5E60.count = count + 1;
    }
    cursor[0] = ssl->cipher >> 8;
    cursor[1] = ssl->cipher;
    cursor[2] = 0;
    cursor += 3;
    if (!ssl->resumed) {
        if (length) {
            int packetLength = length + 6;
            int chainLength = length + 3;
            cursor[0] = 11;
            cursor[1] = packetLength >> 16;
            cursor[2] = packetLength >> 8;
            cursor[3] = packetLength;
            cursor[4] = chainLength >> 16;
            cursor[5] = chainLength >> 8;
            cursor[6] = chainLength;
            cursor[7] = length >> 16;
            cursor[8] = length >> 8;
            cursor[9] = length;
            cursor += 10;
            MI_CpuCopy8(cert->bytes, cursor, length);
            cursor += length;
        }
        cursor[0] = 14;
        cursor[1] = 0;
        cursor[2] = 0;
        cursor[3] = 0;
        cursor += 4;
    }
    length = cursor - buffer - 5;
    buffer[0] = 22;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = length >> 8;
    buffer[4] = length;
    update_digest(ssl, buffer + 5, length);
    CPSi_TcpWrite2Raw(buffer, length + 5, 0, 0, connection);
    _021D4660(buffer);
    return ssl->resumed;
}
