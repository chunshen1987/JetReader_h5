#include<iostream>
#include<sstream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<string>

#include "hdf5.h"
#include "Hydroinfo_h5.h"

using namespace std;

HydroinfoH5::HydroinfoH5(string filename)
{
   herr_t status;
   const char *fileptr = (char*) filename.c_str();
   H5file_id = H5Fopen(fileptr, H5F_ACC_RDWR, H5P_DEFAULT);
   H5groupEventid = H5Gopen(H5file_id, "/Event", H5P_DEFAULT);

   readHydrogridInfo();
   printHydrogridInfo();

   Buffersize = 200;
   dimensionX = grid_XH - grid_XL + 1;
   dimensionY = grid_YH - grid_YL + 1;
   
   //initialize all matrices
   ed = new double** [Buffersize];
   sd = new double** [Buffersize];
   vx = new double** [Buffersize];
   vy = new double** [Buffersize];
   Temperature = new double** [Buffersize];
   Pressure = new double** [Buffersize];
   pi00 = new double** [Buffersize];
   pi01 = new double** [Buffersize];
   pi02 = new double** [Buffersize];
   pi03 = new double** [Buffersize];
   pi11 = new double** [Buffersize];
   pi12 = new double** [Buffersize];
   pi13 = new double** [Buffersize];
   pi22 = new double** [Buffersize];
   pi23 = new double** [Buffersize];
   pi33 = new double** [Buffersize];
   BulkPi = new double** [Buffersize];
   for(int i=0; i<Buffersize; i++)
   {
      ed[i] = new double* [dimensionX];
      sd[i] = new double* [dimensionX];
      vx[i] = new double* [dimensionX];
      vy[i] = new double* [dimensionX];
      Temperature[i] = new double* [dimensionX];
      Pressure[i] = new double* [dimensionX];
      pi00[i] = new double* [dimensionX];
      pi01[i] = new double* [dimensionX];
      pi02[i] = new double* [dimensionX];
      pi03[i] = new double* [dimensionX];
      pi11[i] = new double* [dimensionX];
      pi12[i] = new double* [dimensionX];
      pi13[i] = new double* [dimensionX];
      pi22[i] = new double* [dimensionX];
      pi23[i] = new double* [dimensionX];
      pi33[i] = new double* [dimensionX];
      BulkPi[i] = new double* [dimensionX];
      for(int j=0; j<dimensionX; j++)
      {
         ed[i][j] = new double [dimensionY];
         sd[i][j] = new double [dimensionY];
         vx[i][j] = new double [dimensionY];
         vy[i][j] = new double [dimensionY];
         Temperature[i][j] = new double [dimensionY];
         Pressure[i][j] = new double [dimensionY];
         pi00[i][j] = new double [dimensionY];
         pi01[i][j] = new double [dimensionY];
         pi02[i][j] = new double [dimensionY];
         pi03[i][j] = new double [dimensionY];
         pi11[i][j] = new double [dimensionY];
         pi12[i][j] = new double [dimensionY];
         pi13[i][j] = new double [dimensionY];
         pi22[i][j] = new double [dimensionY];
         pi23[i][j] = new double [dimensionY];
         pi33[i][j] = new double [dimensionY];
         BulkPi[i][j] = new double [dimensionY];
      }
   }
   
   readHydroinfoBuffered(); 

   status = H5Gclose(H5groupEventid);
   status = H5Fclose(H5file_id);
}

HydroinfoH5::~HydroinfoH5()
{
   for(int i=0; i<Buffersize; i++)
   {
      for(int j=0; j<dimensionX; j++)
      {
         delete[] ed[i][j];
         delete[] sd[i][j];
         delete[] vx[i][j];
         delete[] vy[i][j];
         delete[] Temperature[i][j];
         delete[] Pressure[i][j];
         delete[] pi00[i][j];
         delete[] pi01[i][j];
         delete[] pi02[i][j];
         delete[] pi03[i][j];
         delete[] pi11[i][j];
         delete[] pi12[i][j];
         delete[] pi13[i][j];
         delete[] pi22[i][j];
         delete[] pi23[i][j];
         delete[] pi33[i][j];
         delete[] BulkPi[i][j];
      }
      delete[] ed[i];
      delete[] sd[i];
      delete[] vx[i];
      delete[] vy[i];
      delete[] Temperature[i];
      delete[] Pressure[i];
      delete[] pi00[i];
      delete[] pi01[i];
      delete[] pi02[i];
      delete[] pi03[i];
      delete[] pi11[i];
      delete[] pi12[i];
      delete[] pi13[i];
      delete[] pi22[i];
      delete[] pi23[i];
      delete[] pi33[i];
      delete[] BulkPi[i];
   }
   delete[] ed;
   delete[] sd;
   delete[] vx;
   delete[] vy;
   delete[] Temperature;
   delete[] Pressure;
   delete[] pi00;
   delete[] pi01;
   delete[] pi02;
   delete[] pi03;
   delete[] pi11;
   delete[] pi12;
   delete[] pi13;
   delete[] pi22;
   delete[] pi23;
   delete[] pi33;
   delete[] BulkPi;
}

void HydroinfoH5::readHydrogridInfo()
{
   herr_t status;

   grid_XL = readH5Attribute_int(H5groupEventid, "XL");
   grid_XH = readH5Attribute_int(H5groupEventid, "XH");
   grid_YL = readH5Attribute_int(H5groupEventid, "YL");
   grid_YH = readH5Attribute_int(H5groupEventid, "YH");
   grid_Tau0 = readH5Attribute_double(H5groupEventid, "Tau0");
   grid_dTau = readH5Attribute_double(H5groupEventid, "dTau");
   grid_dx = readH5Attribute_double(H5groupEventid, "DX");
   grid_dy = readH5Attribute_double(H5groupEventid, "DY");
   
   status = H5Gget_num_objs(H5groupEventid, &grid_Framenum);
}

void HydroinfoH5::printHydrogridInfo()
{
   cout << "-----------------------------------------" << endl;
   cout << "-----------hydro grid info---------------" << endl;
   cout << "-----------------------------------------" << endl;
   cout << "XL = " << grid_XL << endl;
   cout << "XH = " << grid_XH << endl;
   cout << "DX = " << grid_dx << " fm" << endl;
   cout << "YL = " << grid_YL << endl;
   cout << "YH = " << grid_YH << endl;
   cout << "DY = " << grid_dy << " fm" << endl;
   cout << "Tau0 = " << grid_Tau0 << " fm" << endl;
   cout << "dTau = " << grid_dTau << " fm" << endl;
   cout << "-----------------------------------------" << endl;
}

int HydroinfoH5::readH5Attribute_int(hid_t id, string attributeName)
{
   int attributeValue;
   hid_t attr;
   herr_t ret;
   attr = H5Aopen_name(id, attributeName.c_str());
   ret  = H5Aread(attr, H5T_NATIVE_INT, &attributeValue);
   ret =  H5Aclose(attr);
   return(attributeValue);
}

double HydroinfoH5::readH5Attribute_double(hid_t id, string attributeName)
{
   double attributeValue;
   hid_t attr;
   herr_t ret;
   attr = H5Aopen_name(id, attributeName.c_str());
   ret  = H5Aread(attr, H5T_NATIVE_DOUBLE, &attributeValue);
   ret =  H5Aclose(attr);
   return(attributeValue);
}

void HydroinfoH5::readHydroinfoBuffered()
{
   hid_t group_id;
   herr_t status;

   for(int i=0; i<Buffersize; i++)
   {
      if(i < (int) grid_Framenum)
      {
         stringstream frameName;
         frameName << "Frame_" <<  setw(4) << setfill('0') << i;
         group_id = H5Gopen(H5groupEventid, frameName.str().c_str(), H5P_DEFAULT);

         readH5Dataset_double(group_id, "e", ed[i]);
         readH5Dataset_double(group_id, "s", sd[i]);
         readH5Dataset_double(group_id, "Vx", vx[i]);
         readH5Dataset_double(group_id, "Vy", vy[i]);
         readH5Dataset_double(group_id, "Temp", Temperature[i]);
         readH5Dataset_double(group_id, "P", Pressure[i]);
         readH5Dataset_double(group_id, "Pi00", pi00[i]);
         readH5Dataset_double(group_id, "Pi01", pi01[i]);
         readH5Dataset_double(group_id, "Pi02", pi02[i]);
         readH5Dataset_double(group_id, "Pi03", pi03[i]);
         readH5Dataset_double(group_id, "Pi11", pi11[i]);
         readH5Dataset_double(group_id, "Pi12", pi12[i]);
         readH5Dataset_double(group_id, "Pi13", pi13[i]);
         readH5Dataset_double(group_id, "Pi22", pi22[i]);
         readH5Dataset_double(group_id, "Pi23", pi23[i]);
         readH5Dataset_double(group_id, "Pi33", pi33[i]);
         readH5Dataset_double(group_id, "BulkPi", BulkPi[i]);
   
         status = H5Gclose(group_id);
      }
      else
         break;
   }
}

void HydroinfoH5::readHydroinfoSingleframe(int frameIdx)
{
   hid_t group_id;
   herr_t status;

   if(frameIdx < (int) grid_Framenum)
   {
      stringstream frameName;
      frameName << "Frame_" <<  setw(4) << setfill('0') << frameIdx;
      group_id = H5Gopen(H5groupEventid, frameName.str().c_str(), H5P_DEFAULT);

      readH5Dataset_double(group_id, "e", ed[frameIdx]);
      readH5Dataset_double(group_id, "s", sd[frameIdx]);
      readH5Dataset_double(group_id, "Vx", vx[frameIdx]);
      readH5Dataset_double(group_id, "Vy", vy[frameIdx]);
      readH5Dataset_double(group_id, "Temp", Temperature[frameIdx]);
      readH5Dataset_double(group_id, "P", Pressure[frameIdx]);
      readH5Dataset_double(group_id, "Pi00", pi00[frameIdx]);
      readH5Dataset_double(group_id, "Pi01", pi01[frameIdx]);
      readH5Dataset_double(group_id, "Pi02", pi02[frameIdx]);
      readH5Dataset_double(group_id, "Pi03", pi03[frameIdx]);
      readH5Dataset_double(group_id, "Pi11", pi11[frameIdx]);
      readH5Dataset_double(group_id, "Pi12", pi12[frameIdx]);
      readH5Dataset_double(group_id, "Pi13", pi13[frameIdx]);
      readH5Dataset_double(group_id, "Pi22", pi22[frameIdx]);
      readH5Dataset_double(group_id, "Pi23", pi23[frameIdx]);
      readH5Dataset_double(group_id, "Pi33", pi33[frameIdx]);
      readH5Dataset_double(group_id, "BulkPi", BulkPi[frameIdx]);
   
      status = H5Gclose(group_id);
   }
   else
   {
      cout << "Error: readHydroinfoSingleframe :: frameIdx exceed maximum frame number from hydro" << endl;
      cout << "frameIdx = " << frameIdx << endl;
      exit(1);
   }
}

void HydroinfoH5::readH5Dataset_double(hid_t id, string datasetName, double** dset_data)
{
   herr_t status;
   hid_t dataset_id;
   
   double temp_data[dimensionX][dimensionY];
   dataset_id = H5Dopen(id, datasetName.c_str(), H5P_DEFAULT);
   status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, temp_data);
   for(int i=0; i<dimensionX; i++)
      for(int j=0; j<dimensionY; j++)
         dset_data[i][j] = temp_data[i][j];
   status = H5Dclose(dataset_id);
}
