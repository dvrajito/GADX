/***************************************************************

   Project: GADX, a C++ implementation of genetic algorithms
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    main.cc
   Update:  March 2019

   The main function for the genetic algorithms and some helping
   functions.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;
#include <ctime>
#include "GAManager.h"


// Main function, entry point to the program.
int main(int argc, char **argv)
{
    time_t sTime, eTime;
    GAManager GAM;

    // init files and settings
    GAM.GAInit();
    GAM.InitialPopulation();

    // Run the GA and time it
    time(&sTime);
    GAM.ExecuteGA();
    time(&eTime);
    cout << "total time:" << eTime - sTime << endl;

    return 0;
}











