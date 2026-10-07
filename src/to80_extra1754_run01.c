#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include "renderer_joint_family_private.h"
extern void getTransData_(s32*,s32,const void*,const void*),getTransDataEx_(s32*,s32,const void*,const void*),getRotData_(s32*,s32,const void*,const void*),getRotDataEx_(s32*,s32,const void*,const void*),getScaleData_(s32*,s32,const void*,const void*),getScaleDataEx_(s32*,s32,const void*,const void*);
extern int getRotDataByIdx_(s32*,const void*,const void*,u32);
void getJntSRTAnmResult_(u8*anim,u32 index,s32 frame,MdlJointResult*out){s32 scales[6],x[2],y[2],z[2];u32 flags;u8*p=anim+((u16*)(anim+0x14))[index];flags=*(u32*)p;int interp;
 if(flags&1){out->flags=7;goto done;}
 p+=4;if((frame&0xfff)&&(*(u32*)(anim+8)&1))interp=1;else interp=0;out->flags=0;
 if(!(flags&6)){
 if(!(flags&8)){if(interp)getTransDataEx_(&out->trans[0],frame,p,anim);else getTransData_(&out->trans[0],frame,p,anim);p+=8;}else{out->trans[0]=*(s32*)p;p+=4;}
 if(!(flags&0x10)){if(interp)getTransDataEx_(&out->trans[1],frame,p,anim);else getTransData_(&out->trans[1],frame,p,anim);p+=8;}else{out->trans[1]=*(s32*)p;p+=4;}
 if(!(flags&0x20)){if(interp)getTransDataEx_(&out->trans[2],frame,p,anim);else getTransData_(&out->trans[2],frame,p,anim);p+=8;}else{out->trans[2]=*(s32*)p;p+=4;}
 }else if(flags&2)out->flags|=4;else getMdlTrans_(out);
 if(!(flags&0xc0)){
 if(!(flags&0x100)){if(interp)getRotDataEx_(out->rot,frame,p,anim);else getRotData_(out->rot,frame,p,anim);p+=8;}else{if(getRotDataByIdx_(out->rot,anim+*(u32*)(anim+0xc),anim+*(u32*)(anim+0x10),*(u32*)p)){s32 f=out->rot[5],a=out->rot[0],d=out->rot[3],c=out->rot[2],b=out->rot[1],e=out->rot[4];out->rot[6]=(b*f-c*e)>>12;out->rot[7]=(c*d-a*f)>>12;out->rot[8]=(a*e-b*d)>>12;}p+=4;}
 }else if(flags&0x40)out->flags|=2;else getMdlRot_(out);
 if(!(flags&0x600)){
 if(!(flags&0x800)){if(interp)getScaleDataEx_(x,frame,p,anim);else getScaleData_(x,frame,p,anim);scales[0]=x[0];scales[3]=x[1];}else{scales[0]=((s32*)p)[0];scales[3]=((s32*)p)[1];}
 if(!(flags&0x1000)){if(interp)getScaleDataEx_(y,frame,p+8,anim);else getScaleData_(y,frame,p+8,anim);scales[1]=y[0];scales[4]=y[1];}else{scales[1]=((s32*)p)[2];scales[4]=((s32*)p)[3];}
 if(!(flags&0x2000)){if(interp)getScaleDataEx_(z,frame,p+16,anim);else getScaleData_(z,frame,p+16,anim);scales[2]=z[0];scales[5]=z[1];}else{scales[2]=((s32*)p)[4];scales[5]=((s32*)p)[5];}
 }else if(flags&0x200)out->flags|=1;else{getMdlScale_(out);return;}
 done:{int mode=(out->flags&1)?4:0;MdlRenderState*ctx=_021DA700;ctx->scale(out,scales,ctx->command,mode);}
}
