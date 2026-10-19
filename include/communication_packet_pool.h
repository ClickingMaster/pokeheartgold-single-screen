#ifndef POKEHEARTGOLD_COMMUNICATION_PACKET_POOL_H
#define POKEHEARTGOLD_COMMUNICATION_PACKET_POOL_H

#include "global.h"

// A ring of bytes: data is written at write, made readable up to limit by ByteRing_Commit, read from read.
typedef struct ByteRing {
    u8 *data;
    s16 read;
    volatile s16 limit;
    volatile s16 write;
    s16 capacity;
} ByteRing;

typedef struct PacketNode {
    u8 *data;
    struct PacketNode *prev;
    struct PacketNode *next;
    u16 size;
    u8 command; // 0: free node

    union {
        u8 raw;

        struct {
            u8 headerWritten : 1;
            u8 inRing : 1; // the data was copied into the pool's ring
            u8 rest : 6;
        } b;
    } flags;
} PacketNode;

typedef struct PacketQueue {
    PacketNode *head;
    PacketNode *tail;
} PacketQueue;

typedef struct PacketPool {
    PacketQueue queue;
    PacketQueue queue2;
    PacketNode *current; // partly written into the last frame
    ByteRing *ring;
    PacketNode *nodes;
    int count;
} PacketPool;

typedef struct ByteWriter {
    u8 *dest;
    int remaining;
} ByteWriter;

#endif // POKEHEARTGOLD_COMMUNICATION_PACKET_POOL_H
