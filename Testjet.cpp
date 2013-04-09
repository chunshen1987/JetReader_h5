#include<iostream>
#include<sstream>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<string>

#include "Testjet.h"

using namespace std;

Testjet::Testjet(HydroinfoH5* hydroInfo_in)
{
   hydroInfoptr = hydroInfo_in;
}

Testjet::~Testjet()
{

}

double Testjet::getJetlengthMax(double x_o, double y_o, double phi, double cutT, double step)
{
   double tauMax;
   double vx = cos(phi);
   double vy = sin(phi);

   double grid_Xmax = hydroInfoptr->getHydrogridXmax();
   double grid_Ymax = hydroInfoptr->getHydrogridYmax();
   double grid_X0 = hydroInfoptr->getHydrogridX0();
   double grid_Y0 = hydroInfoptr->getHydrogridY0();
   double grid_Taumax = hydroInfoptr->getHydrogridTaumax();
   double grid_Tau0 = hydroInfoptr->getHydrogridTau0();

   if(vx >= 0)
   {
      if(vy >= 0)
      {
         if(vy*(grid_Xmax - x_o) > vx*(grid_Ymax - y_o))
            tauMax = (grid_Ymax - y_o)/vy;
         else
            tauMax = (grid_Xmax - x_o)/vx;
      }
      else
      {
         if(-vy*(grid_Xmax - x_o) > vx*(y_o - grid_Y0))
            tauMax = (y_o - grid_Y0)/(-vy);
         else
            tauMax = (grid_Xmax - x_o)/vx;
      }
   }
   else
   {
      if(vy >= 0)
      {
         if(vy*(x_o - grid_X0) > (-vx)*(grid_Ymax - y_o))
            tauMax = (grid_Ymax - y_o)/vy;
         else
            tauMax = (x_o - grid_X0)/(-vx);
      }
      else
      {
         if((-vy)*(x_o - grid_X0) > (-vx)*(y_o - grid_Y0))
            tauMax = (y_o - grid_Y0)/(-vy);
         else
            tauMax = (x_o - grid_X0)/(-vx);
      }
   }
   tauMax = min(tauMax, grid_Taumax - grid_Tau0);
   
   double jetLength = tauMax;
   fluidCell* fluidCellptr = new fluidCell();
   while(1)
   {
      if(jetLength < 0)
      {
         tauMax = 0;
         break;
      }
      hydroInfoptr->getHydroinfo(jetLength+grid_Tau0, x_o+jetLength*vx, y_o+jetLength*vy, fluidCellptr);
      if(fluidCellptr->temperature >= cutT)
      {
         tauMax = jetLength;
         break;
      }
      else
         jetLength = jetLength - step;
   }
   delete fluidCellptr;
   return(tauMax);
}
