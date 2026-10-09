// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
const double bp[2]={1.0,1.5};
const double dp_h[2]={0.0,0.5849624872207642};
const double dp_l[2]={0.0,1.350039202129749e-08};
extern float __float_nan;extern int errno;
extern double fabs(double),scalbn(double,int),_dsqrt(double);
#define HI(v) (((s32*)&(v))[1])
#define LO(v) (((u32*)&(v))[0])
double __ieee754_pow(double,double);
static const double lg2=0.6931471805599453,lg2_l=-1.904654299957768e-09;
// @fn __ieee754_pow
double __ieee754_pow(double x,double y){
 double z,ax,z_h,z_l,p_h,p_l,y1,t1,t2,r,s,t,u,v,w;
 s32 i,j,k,yisint,n,negativeflag;s32 hx,hy,ix,iy;u32 lx,ly;
 hx=HI(x);lx=LO(x);hy=HI(y);ly=LO(y);ix=hx&0x7fffffff;iy=hy&0x7fffffff;
 if((iy|ly)==0)return 1.0;
 if(ix>0x7ff00000||(ix==0x7ff00000&&lx!=0)||iy>0x7ff00000||(iy==0x7ff00000&&ly!=0))return x+y;
 yisint=0;
 if(hx<0){
  if(iy>=0x43400000)yisint=2;
  else if(iy>=0x3ff00000){k=(iy>>20)-1023;if(k>20){j=ly>>(52-k);if((u32)(j<<(52-k))==ly)yisint=2-(j&1);}else if(ly==0){j=iy>>(20-k);if((j<<(20-k))==iy)yisint=2-(j&1);}}
 }
 if(ly==0){
  if(iy==0x7ff00000){if(((ix-0x3ff00000)|lx)==0)return y-y;else if(ix>=0x3ff00000)return hy>=0?y:0.0;else return hy<0?0.0-y:0.0;}
  if(iy==0x3ff00000)return hy>=0?x:1.0/x;
  if(hy==0x40000000)return x*x;
  if(hy==0x3fe00000&&hx>=0)return _dsqrt(x);
 }
 ax=fabs(x);
 if(lx==0){if(ix==0x7ff00000||ix==0||ix==0x3ff00000){z=ax;if(hy<0)z=1.0/z;if(hx<0){if(((ix-0x3ff00000)|yisint)==0)z=(z-z)/(z-z);else if(yisint==1)z=0.0-z;}return z;}}
 negativeflag=(hx>>31)+1;
 if((negativeflag|yisint)==0){errno=33;return (double)__float_nan;}
 if(iy>0x41e00000){
  if(iy>0x43f00000){if(ix<=0x3fefffff)return hy<0?(1.0e300*1.0e300):0.0;if(ix>=0x3ff00000)return hy>0?(1.0e300*1.0e300):0.0;}
  if(ix<0x3fefffff)return hy<0?(1.0e300*1.0e300):0.0;
  if(ix>0x3ff00000)return hy>0?(1.0e300*1.0e300):0.0;
  t=x-1.0;w=t*t*(0.5-t*(0.3333333333333333-t*0.25));
  u=1.4426950216293335*t;
  v=t*1.9259629911266175e-08-w*1.4426950408889634;
  t1=u+v;LO(t1)=0;t2=v-(t1-u);
 }else{
  double s2,s_h,s_l,t_h,t_l;
  n=0;if(ix<0x00100000){ax*=9007199254740992.0;n-=53;ix=HI(ax);}
  n+=(ix>>20)-1023;j=ix&0x000fffff;ix=j|0x3ff00000;
  if(j<=0x3988e)k=0;else if(j<0xbb67a)k=1;else{k=0;n++;ix-=0x100000;}
  HI(ax)=ix;u=ax-bp[k];v=1.0/(ax+bp[k]);s=u*v;s_h=s;LO(s_h)=0;
  t_h=0.0;HI(t_h)=((ix>>1)|0x20000000)+0x00080000+(k<<18);
  t_l=ax-(t_h-bp[k]);s_l=v*((u-s_h*t_h)-s_h*t_l);
  s2=s*s;r=s2*s2*(0.5999999999999946+s2*(0.4285714285785502+s2*(0.33333332981837743+s2*(0.272728123808534+s2*(0.23066074577556175+s2*0.20697501780033842)))));
  r+=s_l*(s_h+s);s2=s_h*s_h;t_h=3.0+s2+r;LO(t_h)=0;t_l=r-((t_h-3.0)-s2);
  u=s_h*t_h;v=s_l*t_h+t_l*s;p_h=u+v;LO(p_h)=0;p_l=v-(p_h-u);
  z_h=0.9617967009544373*p_h;
  z_l=dp_l[k]+(-7.028461650952758e-09*p_h+p_l*0.9617966939259756);
  t=(double)n;t1=((z_h+z_l)+dp_h[k])+t;LO(t1)=0;t2=z_l-(((t1-t)-dp_h[k])-z_h);
 }
 s=1.0;if((negativeflag|(yisint-1))==0)s=-1.0;
 y1=y;LO(y1)=0;p_l=(y-y1)*t1+y*t2;p_h=y1*t1;z=p_l+p_h;j=HI(z);i=LO(z);
 if(j>=0x40900000){if(((j-0x40900000)|i)!=0)return s*1e+300*1e+300;if(p_l+8.008566259537294e-17>z-p_h)return s*1e+300*1e+300;}
 else if((j&0x7fffffff)>=0x4090cc00){if(((j+0x3f6f3400)|i)!=0)return s*1e-300*1e-300;if(p_l<=z-p_h)return s*1e-300*1e-300;}
 i=j&0x7fffffff;k=(i>>20)-1023;n=0;
 if(i>0x3fe00000){n=j+(0x100000>>(k+1));k=((n&0x7fffffff)>>20)-1023;t=0.0;HI(t)=n&~(0x000fffff>>k);n=((n&0x000fffff)|0x00100000)>>(20-k);if(j<0)n=-n;p_h-=t;}
 t=p_l+p_h;LO(t)=0;u=t*0.6931471824645996;v=(p_l-(t-p_h))*lg2+t*lg2_l;
 z=u+v;w=v-(z-u);t=z*z;
 t1=z-t*(0.16666666666666602+t*(-0.0027777777777015593+t*(6.613756321437934e-05+t*(-1.6533902205465252e-06+t*4.1381367970572385e-08))));
 r=(z*t1)/(t1-2.0)-(w+z*w);z=1.0-(r-z);j=HI(z);j+=n<<20;
 if((j>>20)<=0)z=scalbn(z,n);else HI(z)+=n<<20;
 return s*z;
}
