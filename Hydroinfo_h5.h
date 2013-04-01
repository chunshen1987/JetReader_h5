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
      int Visflag;  // flag to determine whether to read evolutions for viscous variables

      int Buffersize;
      hid_t H5file_id, H5groupEventid;

      int grid_XL, grid_XH, grid_YL, grid_YH;
      hsize_t grid_Framenum;
      double grid_Tau0, grid_dTau, grid_dx, grid_dy;
      double grid_Taumax;

      int dimensionX, dimensionY;
      double ***ed, ***sd, ***vx, ***vy, ***Temperature, ***Pressure;
      double ***pi00, ***pi01, ***pi02, ***pi03, ***pi11, ***pi12, ***pi13, ***pi22, ***pi23, ***pi33;
      double ***BulkPi;

   public:
      HydroinfoH5(string file);
      ~HydroinfoH5();
      
      void readHydrogridInfo();
      void printHydrogridInfo();
      int readH5Attribute_int(hid_t id, string attributeName);
      double readH5Attribute_double(hid_t id, string attributeName);

      void readHydroinfoBuffered(int loopNum);
      void readHydroinfoSingleframe(int frameIdx);
      void readH5Dataset_double(hid_t id, string datasetName, double** dset_data);

};

#endif
