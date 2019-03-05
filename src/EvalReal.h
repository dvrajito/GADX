/********************************************************************

   Project: GADX, a C++ implementation of genetic algorithms
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    EvalReal.h
   Update:  March 2019

   Evaluation (fitness) functions for the genetic algorithm with
   any kind of problems involving real numbers.

*********************************************************************/

#ifndef EvalReal_h
#define EvalReal_h

#include "Individual.h"
#include "EvalInfo.h"

// Converts a chromosome into a real value between the lower and upper limits a, b.
// x = a + sum(c[i]*(b-a)/2^(i+1), i=0,length)
double IndToReal(Individual *anInd);

// Converts a chromosome to an array of real numbers.
// Result stored in realVals. nLim is the number of variables.
// limits shows the partitionning of the chromosome into variables.
// It contains the last position in the chromosome for each variable.
void IndToReals(Individual *anInd,
                double realVals[],
                int limits[],
                int nLim);

// Converts the chromosome to an array of real numbers and then
// calls the appropriate fitness function and transformation to evaluate them.
double EvalReal(Individual *anInd, EvalInfo *anEval);

#endif
