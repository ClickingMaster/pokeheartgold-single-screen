#ifndef TO90_OV45_INPUT_PRIVATE_H
#define TO90_OV45_INPUT_PRIVATE_H
#include "global.h"
#include <stddef.h>
typedef struct Ov45Input48 {u32 words[12];} Ov45Input48;
typedef struct Ov45Payload84 {Ov45Input48 input;u32 flag;char text[32];} Ov45Payload84;
typedef struct Ov45Key12 {u32 words[3];} Ov45Key12;
typedef char Ov45InputExtent[(sizeof(Ov45Input48)==48)?1:-1];
typedef char Ov45PayloadExtent[(sizeof(Ov45Payload84)==84)?1:-1];
typedef char Ov45PayloadFlag[(offsetof(Ov45Payload84,flag)==48)?1:-1];
typedef char Ov45PayloadText[(offsetof(Ov45Payload84,text)==52)?1:-1];
typedef char Ov45KeyExtent[(sizeof(Ov45Key12)==12)?1:-1];
void ov45_02247B94(void *,const char *,const Ov45Input48 *);
u32 ov45_0224A874(void *,void *,int,int,const Ov45Input48 *,const char *);
void ov45_02250688(void *,char *,char *,const Ov45Input48 *,int,int,int);
u8 *ov45_022496AC(u8 *,int,u32 *);
#endif
