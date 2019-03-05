/********************************************************

   Project: GAD trajectory
   Author:  Dana Vrajitoru
   License: Creative Commons, Attribution
   File:    SettingFile.cc
   Updated: March 2019

   Reading the information from the setting file.

*********************************************************/

//#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;
#include "GenTypes.h"
#include "GenInfo.h"
#include "RunInfo.h"
#include "EvalInfo.h"
#include "General.h"
#include "EvalReal.h"
#include "GenEval.h"
#include "EvalBool.h"
#include "SettingFile.h"

extern GenInfo *aGenInfo;
extern RunInfo *aRunInfo;
extern EvalInfo *anEvalInfo;

// Read the file "GAPrefs.txt" and return it as a "dictionary"
// which is an array of strings, explanation followed by value.
char **ReadDictionary()
{
    const int lineLen = 80;
    char **result, rline[lineLen]="";
    ifstream finput(settingFile);
    int i, j = 0;

    // check if the file was properly opened
    if (!finput.good()) {
        cout << "Could not read the settings file" << endl;
        return NULL;
    }

    // allocate the dictionary
    result = new char *[2 * OPT_NR];

    // read 2 lines for each options, assuming that they are stored
    // as "explanation \n value \n".
    for (i = 0; i < 2 * OPT_NR; i++) {
        do {
            finput.getline(rline, lineLen);
            DeleteComment(rline);   // if there are other comments on the line, delete them
        } while (rline[0] == '\0'); // comment lines start with #
        result[i] = new char[strlen(rline) + 1];
        strcpy(result[i], rline);
        rline[0] = '\0';
    }
    finput.close();
    cout << "dictionary read" << endl;
    return result;
}

// Intialize the settings of the genetic algorithm from 
// the settings file created from the Python interface.
void InitGA()
{
    char **theDict = ReadDictionary();
    aGenInfo = new GenInfo(theDict);
    aGenInfo->Print();
    aRunInfo = new RunInfo(theDict);
    aRunInfo->Print();
    anEvalInfo = new EvalInfo(theDict);
    anEvalInfo->Print();
}
