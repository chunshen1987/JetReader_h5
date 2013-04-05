#ifndef Testjet_H
#define Testjet_H

#include<fstream>
#include<sstream>
#include<string>

#include "Hydroinfo_h5.h"
using namespace std;

class Testjet
{
   private:
      HydroinfoH5* hydroInfoptr;

   public:
      Testjet(HydroinfoH5* hydroInfo_in);
      ~Testjet();
      
      double getJetlengthMax(double x_o, double y_o, double phi, double cutT, double step);

};
#endif
