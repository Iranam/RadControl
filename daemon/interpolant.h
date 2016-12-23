#ifndef Interpolant_H
#define Interpolant_H
namespace interpolant{

typedef float real;
typedef unsigned int uint;
class Interpolant{
public:
  Interpolant(real*x,real*y,uint N);
  Interpolant(const char* filename);
  //Interpolant(char*string);
	~Interpolant();
	real operator()(real x);
private:
  uint N;
	real*px,*y,*a,*b,*c;
	real al,ar;//coefficients for leftmost and rightmost half-intervals(linear functions)
	void construct();
};
}
#endif
