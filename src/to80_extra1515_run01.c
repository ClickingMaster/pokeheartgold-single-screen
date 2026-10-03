#pragma force_active on
#include "global.h"
#define P(d,n) ((char*)(d)+(n))
#define W(d,n) (*(void**)P(d,n))
#define I(d,n) (*(int*)P(d,n))
#define B(d,n) (*(u8*)P(d,n))
extern u8 gSystem[],ov15_02200640[],ov15_02200641[],ov15_02200642[],ov15_02200643[];
int ov15_021FA73C(void*,int,u8*,int,int,int);int ov15_021FA6C0(void*,int,int);void ov15_021FFECC(void*,int);void PlaySE(int);
void ov15_021FA0E4(void*,int);void ov15_021FA170(void*);int ov15_021FAC2C(void*);int ov15_021FA104(void*,int);int ov15_021FA68C(void*,int);void ov15_021FD774(void*,int);int ov15_021FD810(void*,int,int,int);
int ov15_021FA1BC(void*);
int ov15_021FA1BC(void*d){int changed=0,result,target;u8 flags[5];
 if(I(gSystem,0x4c)&0x40){I(d,0x644)=ov15_02200640[I(d,0x644)*4];changed++;}
 else if(I(gSystem,0x4c)&0x80){I(d,0x644)=ov15_02200641[I(d,0x644)*4];changed++;}
 else if(I(gSystem,0x4c)&0x20){target=ov15_02200642[I(d,0x644)*4];
 if(target==14)return ov15_021FA73C(d,14,&flags[4],1,2,0);
 if(I(d,0x644)!=16){if(I(d,0x644)>=0&&I(d,0x644)<8){result=ov15_021FA6C0(d,I(d,0x644),-1);if(I(d,0x644)!=result){I(d,0x644)=result;changed++;}}
 else{I(d,0x644)=target;changed++;}}
 }
 else if(I(gSystem,0x4c)&0x10){target=ov15_02200643[I(d,0x644)*4];
 if(target==15)return ov15_021FA73C(d,15,&flags[3],1,2,0);
 if(I(d,0x644)!=16){if(I(d,0x644)>=0&&I(d,0x644)<8){result=ov15_021FA6C0(d,I(d,0x644),1);if(I(d,0x644)!=result){I(d,0x644)=result;changed++;}}
 else{I(d,0x644)=target;changed++;}}
 }
 else if(I(gSystem,0x48)&0x200){
 if(I(d,0x644)>=0&&I(d,0x644)<8){I(d,0x644)=ov15_021FA6C0(d,B(W(d,0x234),0x64),-1);ov15_021FFECC(d,I(d,0x644));return ov15_021FA73C(d,I(d,0x644),&flags[2],1,2,changed);}
 else return ov15_021FA73C(d,ov15_021FA6C0(d,B(W(d,0x234),0x64),-1),&flags[2],1,2,0);
 }
 else if(I(gSystem,0x48)&0x100){
 if(I(d,0x644)>=0&&I(d,0x644)<8){I(d,0x644)=ov15_021FA6C0(d,B(W(d,0x234),0x64),1);ov15_021FFECC(d,I(d,0x644));return ov15_021FA73C(d,I(d,0x644),&flags[1],1,2,changed);}
 else return ov15_021FA73C(d,ov15_021FA6C0(d,B(W(d,0x234),0x64),1),&flags[1],1,2,0);
 }
 if(I(d,0x644)==17)I(d,0x644)=B(W(d,0x234),0x64);
 if(changed){PlaySE(0x5dc);ov15_021FFECC(d,I(d,0x644));ov15_021FA0E4(d,I(d,0x644));ov15_021FA170(d);}
 flags[0]=0;result=ov15_021FAC2C(d);
 if(result!=-1){
 if(ov15_021FA104(d,result)){
 if((u32)result<8){if(ov15_021FA68C(d,result)!=-1){I(d,0x644)=result;ov15_021FFECC(d,I(d,0x644));}}
 else{I(d,0x644)=result;ov15_021FFECC(d,I(d,0x644));if(I(d,0x644)>=8&&I(d,0x644)<=13)ov15_021FA0E4(d,I(d,0x644));}
 }
 result=ov15_021FA73C(d,result,&flags[0],1,2,1);if(result!=1)return result;
 }else if(I(gSystem,0x48)&1){result=ov15_021FA73C(d,I(d,0x644),&flags[0],1,2,0);if(I(d,0x644)>=8&&I(d,0x644)<=13)ov15_021FA0E4(d,I(d,0x644));if(result!=1)return result;}
 else if(I(gSystem,0x48)&2){result=ov15_021FA73C(d,16,&flags[0],1,2,0);ov15_021FD774(d,0);if(I(d,0x644)>=8&&I(d,0x644)<=13)ov15_021FA0E4(d,I(d,0x644));if(result!=1)return result;}
 if(flags[0]==1)return ov15_021FD810(d,20,41,27);return 1;
}
