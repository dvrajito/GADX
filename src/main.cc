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
#include "General.h"
#include "RunInfo.h"
#include "EvalInfo.h"
#include "GenInfo.h"
#include "GenOper.h"
#include "GenEval.h"
#include "EvalBool.h"
#include "Individual.h"
#include "Generation.h"
#include "SettingFile.h"

ofstream resFile, histFile;
unsigned int randSeed = 201;
GenInfo *aGenInfo;
RunInfo *aRunInfo;
EvalInfo *anEvalInfo;
Generation *initPop;

///////////////////////// Prototypes //////////////////////
void SaveData(); 
void ReadData();
void InitSaveResults();
void ExecuteTrialSet();
void ExecuteGA();
void FreeAll();

// Main function, entry point to the program.
int main()
{
    time_t sTime, eTime;

    // Read the settings from a dictionary and initialize the
    // settings storing objects from it.
    InitGA();

    // load problem data from files if needed
    if ((anEvalInfo->aFileName != NULL) &&
        (anEvalInfo->aFileName[0] != '\0') &&
        (anEvalInfo->aFileName[0] != ' '))
        if (anEvalInfo->funcNumber < 20)
            ReadSATexpr(anEvalInfo->aFileName);
    // else: add other functions to load data from a file here.

    // apply runtime settings
    randSeed = aRunInfo->rseed;
    srand(randSeed);

    // Initialize the population based on the type of fitness function
    if (anEvalInfo->realEval || anEvalInfo->funcNumber < 20)
        initPop = new Generation(aRunInfo->genSize, aGenInfo, binRand);
    else
        initPop = new Generation(aRunInfo->genSize, aGenInfo, limRand);

    // Uncomment to see what the initial population looks like
    //initPop->Print("old");

    // Start out the file storing results
    InitSaveResults();

    // Run the GA and time it
    time(&sTime);
    ExecuteGA();
    FreeAll();
    time(&eTime);
    cout << "total time:" << eTime - sTime << endl;

    return 0;
}

// Deprecated: Read the data from the console
void ReadData()
{
    int dirty = 0;
    FILE *gaFile;
    gaFile = fopen(settingFile, "r");
    aGenInfo = new GenInfo(gaFile);
    dirty += aGenInfo->AskInfo();
    aRunInfo = new RunInfo(gaFile);
    dirty += aRunInfo->AskRunInfo();
    anEvalInfo = new EvalInfo(gaFile);
    dirty += anEvalInfo->AskEvalInfo();
    fclose(gaFile);
    if (dirty)
        SaveData();
    ReadSATexpr(anEvalInfo->aFileName);
}

// Deprecated: read the settings from a file
void SaveData()
{
    FILE *gaFile;
    gaFile = fopen(settingFile, "w");
    aGenInfo->FPrint(gaFile);
    aRunInfo->FPrint(gaFile);
    anEvalInfo->FPrint(gaFile);
    fclose(gaFile);
}

// Open the files storing the results and the history.
// Result files record the results at the end of each trial.
// History files also record intermediary results.
void InitSaveResults()
{
    char *resName;

    resName = aRunInfo->resFileName;
    // Open the result file and write current settings into it
    if (resName[0] != '0') {
        resFile.open(resName);
        resFile << "random seed\tpopulation size\tindividual size\t";
        resFile << "probCross\tprobMut\tgeneration number\n";
        resFile << randSeed << "\t\t" << aRunInfo->genSize << "\t\t"
            << aGenInfo->indSize << "\t\t" << aGenInfo->probCross << "\t"
            << aGenInfo->probMut << "\t" << aRunInfo->genNumber << endl;
    }
    // open the history file if it's turned on and write current settings into it
    if (aRunInfo->history) {
        if ((aRunInfo->hFileName == NULL) ||
            (aRunInfo->hFileName[0] == '\0') ||
            (aRunInfo->hFileName[0] == ' ')) {
            cout << "history filename" << endl;
            cin >> resName;
            histFile.open(resName, ios::app);
        }
        else
            histFile.open(aRunInfo->hFileName);
       histFile << "random seed\tpopulation size\tindividual size\t";
       histFile << "probCross\tprobMut\tgeneration number" << endl;
       histFile << randSeed << "\t\t" << aRunInfo->genSize << "\t\t"
           << aGenInfo->indSize << "\t\t" << aGenInfo->probCross << "\t"
           << aGenInfo->probMut << "\t" << aRunInfo->genNumber << endl;
    }
}

// Execute a GA trial for a choice of the parameters.
// It runs a number of trials with a different random seed
// based on the setting.
void ExecuteTrialSet()
{
    int j, nTrials;

    aRunInfo->theCross->Print();
    randSeed = aRunInfo->rseed;
    nTrials = aRunInfo->trialNr;
    resFile << endl << "crossover form = " << aRunInfo->theCross->MethodString()
        << endl << "Best fitness:";
    if (aRunInfo->history)
       histFile << endl << "crossover form = " << aRunInfo->theCross->MethodString() << endl;

    // Run the GA for the given number of trials
    for (j = 0; j < nTrials; j++) {
        // randomize the population again for the new trial
        srand(randSeed);
        if (anEvalInfo->realEval || anEvalInfo->funcNumber < 20)
            initPop->ReInit(binRand);
        else
            initPop->ReInit(limRand);
        EvalGen(initPop, anEvalInfo);
        initPop->Sort();
        resFile << endl << initPop->fitValues[initPop->popSize - 1] << "\t";

        // Increment the random seed by randInc for the next trial
        randSeed = randSeed + randInc;
        if (aRunInfo->history)
           histFile << endl << "Trial number " << j << endl;

        // run one trial
        cout << "Trial number " << j << endl;
        initPop->GARun(aRunInfo, anEvalInfo);
        resFile.close();
        resFile.open(aRunInfo->resFileName, ios::app);
    }
}

// Run the genetic algorithm based on the settings
void ExecuteGA()
{
    int i, k;

    // Initialize the fitness array
    InitFitArray(aRunInfo->genSize);

    // execute a GA trial set based on the choice of crossover operation
    if (aRunInfo->theCross->theCrossover == allCrs)
        for (i = onePointCrs; i < allCrs; i++) {
            aRunInfo->theCross->theCrossover = CrossForm(i);
            if ((i == dissocCrs) && (aRunInfo->theCross->dissForm == allOp))
                for (k = zeroOp; k < allOp; k++) {
                    aRunInfo->theCross->dissForm = DissocForm(k);
                    ExecuteTrialSet();
                }
            else if ((i == combineCrs) && (aRunInfo->theCross->combForm == allCmb))
                for (k = perfCmb; k < allCmb; k++) {
                    aRunInfo->theCross->theCrossover = combineCrs;
                    aRunInfo->theCross->combForm = CombineForm(k);
                    ExecuteTrialSet();
                }
            else
                ExecuteTrialSet();
        }
    else if ((aRunInfo->theCross->theCrossover == dissocCrs) &&
             (aRunInfo->theCross->dissForm == allOp))
        for (k = zeroOp; k < allOp; k++) {
            aRunInfo->theCross->dissForm = DissocForm(k);
            ExecuteTrialSet();
        }
    else if ((aRunInfo->theCross->theCrossover == combineCrs) &&
             (aRunInfo->theCross->combForm == allCmb))
        for (k = perfCmb; k < allCmb; k++) {
            aRunInfo->theCross->theCrossover = combineCrs;
            aRunInfo->theCross->combForm = CombineForm(k);
            ExecuteTrialSet();
        }
    else
        ExecuteTrialSet();
    resFile << endl << endl;
    resFile.close();
    if (aRunInfo->history) {
       histFile << endl << endl;
       histFile.close();
    }
}


// Deallocate all the data structures used.
void FreeAll()
{
    delete initPop;
    delete aGenInfo;
    delete aRunInfo;
    delete anEvalInfo;
    deleteGlobalSAT();
}











