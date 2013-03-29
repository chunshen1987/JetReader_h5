#ifndef Hydroinfo_h5_H
#define Hydroinfo_h5_H

#include<fstream>
#include<sstream>
#include<string>

#include "hdf5.h"

using namespace std;

class HydroinfoH5
{
   private:
      int Buffersize;
      hid_t H5file_id, H5groupEventid;
      herr_t status;

      int grid_XL, grid_XH, grid_YL, grid_YH;
      double grid_Tau0, grid_dTau, grid_dx, grid_dy;

   public:
      HydroinfoH5(string file);
      ~HydroinfoH5();
      
      void readHydrogridInfo();
      void printHydrogridInfo();
      int readH5Attribute_int(hid_t id, string attributeName);
      double readH5Attribute_double(hid_t id, string attributeName);

};

#endif
