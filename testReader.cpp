#include<iostream>
#include<sstream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<string>

#include "Hydroinfo_h5.h"
#include "Testjet.h"

using namespace std;

int main()
{
   // test output function
   //HydroinfoH5 testoutput;
   //testoutput.setHydroFiles(-130, 130, 0.1, 5, -130, 130, 0.1, 6, 0.6, 0.02, 5, 1, "H5data_test.h5");
   //HydroinfoH5 testoutput(-130, 130, 0.1, 5, -130, 130, 0.1, 6, 0.6, 0.02, 5, 1, "H5data_test.h5");

   // test read in function
   //HydroinfoH5 testreadin;
   //testreadin.readHydroinfoH5("JetData.h5", 500, 1);
   HydroinfoH5 testreadin("JetData.h5", 500, 1);
   fluidCell* fluidCellptr = new fluidCell();
   testreadin.getHydroinfo(3.4, 2.5, -4.34, fluidCellptr);
   cout << fluidCellptr->temperature << endl;
   Testjet jettest(&testreadin);
   double jetlength = jettest.getJetlengthMax(0.0, 0.0, 0.0, 0.2, 0.05);
   cout << jetlength << endl;
   cout << "Haha" << endl;
   return 0;

}





