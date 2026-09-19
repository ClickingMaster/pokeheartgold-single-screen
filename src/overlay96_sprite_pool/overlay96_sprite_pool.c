#include "global.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void *ov96_021EB3E4(void *, int, int, int, int);
void ov96_021EB564(void *, int);
void ov96_021EB630(void *, int);
void ov96_022050F8(void *, void *);

void ov96_022050F8(void *data, void *gfx)
{
    u8 *cursor;
    int i;
    u8 j;
    u8 *node;

    i = 0;
    cursor = data;
    do {
        PTR(cursor, 0x380) = ov96_021EB3E4(gfx, 2, 1, 0x68, 6);
        ov96_021EB564(PTR(cursor, 0x380), 1);
        i++;
        cursor += 0xC;
    } while (i < 20);

    cursor = data;
    for (i = 0; i < 10; i++) {
        PTR(cursor, 0x470) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0x11);
        ov96_021EB564(PTR(cursor, 0x470), 6);
        PTR(cursor, 0x474) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0x12);
        ov96_021EB564(PTR(cursor, 0x474), 5);
        cursor += 0x10;
    }

    cursor = data;
    for (i = 0; i < 4; i++) {
        PTR(cursor, 0x6C) = ov96_021EB3E4(gfx, 2, 1, 0x68, 9);
        ov96_021EB564(PTR(cursor, 0x6C), 2);
        for (j = 0; j < 2; j++) {
            node = cursor + j * 4;
            PTR(node, 0x70) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0x17);
            ov96_021EB564(PTR(node, 0x70), 3);
            ov96_021EB630(PTR(node, 0x70), 2);
        }
        PTR(cursor, 0x78) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0x1B);
        ov96_021EB564(PTR(cursor, 0x78), 8);
        cursor += 0xB8;
    }

    cursor = data;
    for (i = 0; i < 4; i++) {
        PTR(cursor, 0x34C) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0xE);
        ov96_021EB564(PTR(cursor, 0x34C), 7);
        ov96_021EB630(PTR(cursor, 0x34C), 4);
        cursor += 4;
    }

    cursor = data;
    for (i = 0; i < 4; i++) {
        PTR(cursor, 0x35C) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0x19);
        ov96_021EB564(PTR(cursor, 0x35C), 4);
        cursor += 4;
    }

    cursor = data;
    for (i = 0; i < 5; i++) {
        PTR(cursor, 0x56C) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0x1A);
        PTR(cursor, 0x568) = ov96_021EB3E4(gfx, 2, 1, 0x68, 0x10);
        ov96_021EB564(PTR(cursor, 0x568), 0);
        ov96_021EB564(PTR(cursor, 0x56C), 4);
        cursor += 0x14;
    }
}
