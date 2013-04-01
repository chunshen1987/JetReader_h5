#include<cmath>
#include<iostream>

#include "arsenal.h"
using namespace std;

double cubeInterp(double x, double y, double z, double A000, double A100, double A010, double A110, double A001, double A101, double A011, double A111)
// Perform a 3d interpolation. The known data are A### located at the 8 corners,
// labels using the xyz order. Therefore A000 is value at the origin and A010
// is the value at (x=0,y=1,z=0). Note that the coordinate (x,y,z) must be
// constrained to the unit cube. Axyz is the return value.
{
   double Axyz = A000*(1-x)*(1-y)*(1-z) + A100*x*(1-y)*(1-z) 
                 + A010*(1-x)*y*(1-z) + A001*(1-x)*(1-y)*z 
                 + A101*x*(1-y)*z + A011*(1-x)*y*z + A110*x*y*(1-z) 
                 + A111*x*y*z;
   return(Axyz);
}

