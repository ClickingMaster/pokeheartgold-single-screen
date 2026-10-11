#pragma force_active on
#include "global.h"
#pragma force_active on
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define PTR(p,o) (*(u8**)((u8*)(p)+(o)))
#define U8(p,o) (*(u8*)((u8*)(p)+(o)))
#define U16(p,o) (*(u16*)((u8*)(p)+(o)))
#define U64(p,o) (*(u64*)((u8*)(p)+(o)))
#define D(o) (s+U32(s,12)*320+dst+(o))
extern u32 VsRecorder_GetAvatarFileId(u32,int);
void ov40_02238FF4(u8*);
void ov40_02238FF4(u8*w){u8*s=PTR(w,0x860);int i,dst,src;PTR(s,0x714)=s+0xe0;PTR(s,0x718)=s+0x638;
switch(U32(s,20)){
case 0:for(i=0,dst=0,src=0;i<16;i++,dst+=16,src+=8){if(U32(s,28)==0){U32(D(0),0x74c)=10000;U32(D(0),0x750)=VsRecorder_GetAvatarFileId(U8(PTR(s,0x718)+U32(s,12)*72+i,4),4);U64(D(0),0x754)=(s64)i;}else{U32(D(0),0x74c)=20000;U32(D(0),0x750)=VsRecorder_GetAvatarFileId(U8(PTR(s,0x714)+U32(s,12)*456+i,4),4);U64(D(0),0x754)=U64(PTR(s,0x714)+U32(s,12)*456+src,20);if(U64(D(0),0x754)>=0x8ac7230489e7ffffULL)U64(D(0),0x754)=0x8ac7230489e7ffffULL;}}break;
case 1:{int dst,i,src;for(i=0,dst=0,src=0;i<12;i++,dst+=16,src+=8){if(U32(s,28)==0){U32(D(0),0x74c)=30000;U32(D(0),0x750)=U8(PTR(s,0x718)+U32(s,12)*72+i,20);U64(D(0),0x754)=(s64)i;}else{U32(D(0),0x74c)=40000;U32(D(0),0x750)=U8(PTR(s,0x714)+U32(s,12)*456+i,148);U64(D(0),0x754)=U64(PTR(s,0x714)+U32(s,12)*456+src,160);if(U64(D(0),0x754)>=0x8ac7230489e7ffffULL)U64(D(0),0x754)=0x8ac7230489e7ffffULL;}}break;
}case 2:{int dst,off,src;int i;for(i=0,dst=0,off=0,src=0;i<20;dst+=16,off+=2,src+=8,i++){if(U32(s,28)==0){U32(D(0),0x74c)=50000;U32(D(0),0x750)=U16(PTR(s,0x718)+U32(s,12)*72+off,32);U64(D(0),0x754)=(s64)i;}else{U32(D(0),0x74c)=60000;U32(D(0),0x750)=U16(PTR(s,0x714)+U32(s,12)*456+off,256);U64(D(0),0x754)=U64(PTR(s,0x714)+U32(s,12)*456+src,296);if(U64(D(0),0x754)>=0x8ac7230489e7ffffULL)U64(D(0),0x754)=0x8ac7230489e7ffffULL;}}break;
}}}
