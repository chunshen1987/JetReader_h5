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
   const char *fileptr = (char*) filename.c_str();
   H5file_id = H5Fopen(fileptr, H5F_ACC_RDWR, H5P_DEFAULT);
   H5groupEventid = H5Gopen(H5file_id, "/Event", H5P_DEFAULT);

   readHydrogridInfo();
   printHydrogridInfo();
   
   status = H5Gclose(H5groupEventid);
   status = H5Fclose(H5file_id);
}

HydroinfoH5::~HydroinfoH5()
{
}

void HydroinfoH5::readHydrogridInfo()
{
   grid_XL = readH5Attribute_int(H5groupEventid, "XL");
   grid_XH = readH5Attribute_int(H5groupEventid, "XH");
   grid_YL = readH5Attribute_int(H5groupEventid, "YL");
   grid_YH = readH5Attribute_int(H5groupEventid, "YH");
   grid_Tau0 = readH5Attribute_double(H5groupEventid, "Tau0");
   grid_dTau = readH5Attribute_double(H5groupEventid, "dTau");
   grid_dx = readH5Attribute_double(H5groupEventid, "DX");
   grid_dy = readH5Attribute_double(H5groupEventid, "DY");
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
