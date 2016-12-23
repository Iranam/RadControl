#include"interpolant.h"
#include"string.h"
#include<stdio.h>
#include<iostream>
using std::clog;
using std::cerr;
using std::endl;
#ifndef _NOWARNINGS
#include<math.h>
#endif

namespace interpolant{
Interpolant::Interpolant(real*nx,real*ny,uint nN):
  N{nN}{
	px=new real[N];
	y=new real[N];
  memcpy(px,nx,N*sizeof(real));
  memcpy(y,ny,N*sizeof(real));
	construct();
}
Interpolant::Interpolant(const char* filename){
	FILE*file=fopen(filename,"r");
  if(file==NULL){
		throw -1;
  }
	real val1,val2;
  char junk[256];
  int r;
	N=0;
  while((r=fscanf(file,"%f %f\n",&val1,&val2))!=EOF){
    if(r==2)N++;
    else fgets(junk,sizeof(junk),file);
  }
	rewind(file);
	px=new real[N];
	y=new real[N];
  uint i=0,line=0;;
  while((r=fscanf(file,"%f %f\n",&val1,&val2))!=EOF){
    line++;
    if(r!=2){
      fgets(junk,sizeof(junk),file);
      #ifndef _NOWARNINGS
      clog<<"WARNING:Bad data in file \""<<filename<<"\" at line "<<line<<endl;
      #endif
      continue;
    }
    px[i]=val1;
    y[i]=val2;
    i++;
  }
  fclose(file);
	construct();
}
void Interpolant::construct(){
#ifdef INTERPOLANT_DEBUG
	clog<<"N="<<N<<endl;
	for(uint i=0;i<N;i++)clog<<"x["<<i<<"]="<<px[i]<<endl;
	for(uint i=0;i<N;i++)clog<<"y["<<i<<"]="<<y[i]<<endl;
#endif
	real*der=new real[N];
	der[0]=(y[1]-y[0])/(px[1]-px[0]);
	/*for(uint i=1;i<N-1;i++){
		real x0=px[i-1];
		real x1=px[i];
		real x2=px[i+1];
		real slp1=(y[i]-y[i-1])/(x1-x0);
		real slp2=(y[i+1]-y[i])/(x2-x1);
		real res=2*x1*(slp2-slp1)+slp1*(x2+x1)-slp2*(x1+x0);
    res/=x2-x0;
		der[i]=res;}*/
	for(uint i=1;i<N-1;i++)der[i]=(y[i+1]-y[i-1])/(px[i+1]-px[i-1]);
  /*We are using a less-than-usually orthodox way of estimating derivatives, but it usually
   *makes the interpolant monotonous*/
  der[N-1]=(y[N-1]-y[N-2])/(px[N-1]-px[N-2]);
#ifdef INTERPOLANT_DEBUG
	for(uint i=0;i<N;i++)clog<<"der["<<i<<"]="<<der[i]<<endl;
#endif
	a=new real[N-1];
	b=new real[N-1];
	c=new real[N-1];
	for(uint i=0;i<N-1;i++){
		real h=y[i+1]-y[i];//height of interval i
		real w=px[i+1]-px[i];//width of interval i
		real w2=w*w;
		real dl=der[i];//derivative at left end of the interval
		real dr=der[i+1];//derviative at the right end
	  a[i]=(dl+dr-2*h/w)/w2;
		b[i]=(3*h/w-2*dl-dr)/w;
		c[i]=dl;
		/*basis polynoms:
		 *         a      b      c
		 *height  :-2h/w3  3h/w2 0
		 *leftder : dl/w2 -2dl/w dl
		 *rightder: dr/w2 - dr/w 0
		 * */
#ifndef _NOWARNINGS
    //TODO:doesn't work yet
    real A=a[i],B=b[i],C=c[i],maxx;
    real D=B*B-3*A*C;
    if(D>0&&A!=0&&(maxx=(-B+sqrt(D))/(3*A))>0&&maxx<w){
      clog<<"WARNING:Interpolant does not increase monotonically on the interval ["<<
      px[i]<<':'<<px[i+1]<<"]"<<endl<<"Mimimum at "<<maxx+px[i]<<";"<<endl;}
#endif
	}
  al=der[0];
  ar=der[N-1];
	delete[] der;
}
real Interpolant::operator()(real x){
	//px=[0,2,3,6],N=4
	//y=[0.5,1,2,3]
	//x=4
  uint i=0;
	for(;i<N&&x>px[i];i++);//i=3
	if(i==0)return y[0]+al*(x-px[0]);//x is far left
	if(i==N)return y[N-1]+ar*(x-px[N-1]);//x is far right
	i--;//i=2
	x-=px[i];//x=1
	return ((a[i]*x+b[i])*x+c[i])*x+y[i];
}
Interpolant::~Interpolant(){
	delete[]px;
	delete[]y;
	delete[]a;
	delete[]b;
	delete[]c;
}
}
