===============================================================================
||  Project: GADX                                                            ||
||  Author: Dana Vrajitoru                                                   ||
||  Creative Commons, Attribution                                            ||
||  Updated: March 2021                                                      ||
||  User instructions, compilation and execution guide.                      ||
===============================================================================

This is a C++ implementation of the genetic algorithm with some examples
of simple application problems. It can be run entirely from command-line.

Four forms of crossover are available: 1-point, n-point, uniform, and 
dissociated. A fifth option allows a running a set of trials for each of
these. 

The chromosomes have a sex type as an attribute and several sex types can be
used, as well as several reproduction schemes. These options are described 
in more detail in the section Settings File below.

The code is written to be used both with binary genes, and with genes from
a larger domain, such as [0...limit). A real encoding for several variables
is also included. 

Folder structure:
src - C++ source files
lib - object files for Makefile compilation
bin - executable for Makefile compilation
Problems - examples of problem files for SAT and Hamiltonian Circuit problems.
Results - where statistics and result files will be stored

The executable is called GAD, or projectName.exe under Visual Studio. 

===============================================================================
||                            Compiling                                      ||
===============================================================================

1. Assuming that you upzipped the files into a local folder called GADV. Edit 
the file src/General.h and replace the value of the constant FILE_ROOT with
the absolute path to the folder containing the application files. 

2. If using the Makefile, edit it and add whatever library path isn't 
recognized by g++ by default. If you use another compiler than g++, change the
value of the variable CXX in the Makefile. This project doesn't have any other
special dependencies.

3. If you compile on Linux, comment out the constant M_PI in General.h.

4. If you compile in Visual Studio, add the macro _CRT_SECURE_NO_WARNINGS to 
the project properties under C/C++ - Preprocessor - Definitions. 

5. After this, you should be able to simply compile the project the usual way.
If you use the Makefile, run the command make from the project folder. The
object files will be created in the folder lib and the executable, called GAD,
will be found in the folder bin of the project folder.

===============================================================================
||                           Running the Demo                                ||
===============================================================================

Edit the file GAPrefs.txt in the project folder and replace the path to the
project file in all the file names with the path to where you installed the
project. 

The settings file is setup to run 5 trials with the fitness function 
F(x1, x2) = x1^2 + x2^2
where x1 and x2 are in the range [10, 500). The result is in the file 
Results/testF1.txt in the project folder.

On Linux, if you used the Makefile, after running make from the project folder, 
run the GAD executable from the bin folder. If you use an IDE such as Visual 
Studio, run the executable from wherever the IDE creates it (such as, from the
Debug folder in your project folder).

===============================================================================
||                        Adding Your Own Problem                            ||
===============================================================================

1. Decide whether the genes for your problem will have binary values or if 
your problem is real-valued. The binary representation can easily be extended
to accommodate genes taking values from a finite set. 

2. If the genes are binary or taking values from a finite set, add the fitness
function to the file EvalBool.cc and .h. Then add a number for it and call it
from the function ChoseBFunct in EvalBool.cc. These functions typically receive 
the entire chromosome as a parameter.

3. If the problem is real-valued, then add the fitness function in the file
EvalReal.cc and .h. Assign it a number and call it from the function 
ChoseFunct in EvalReal.cc. Note that the parameter x is an array of real
numbers converted from the chromosome and nLim is the number of variables
stored in this array.

4. In the preference file GAPrefs.txt, set the function number to the one you
assigned to your function and the evaluation form to 0 if you're using binary
or finite number of values genes, or 1 if the problem is real-valued.

5. If your problem is a minimization one, then you need to set the 
transformation number to 3 and add a case for your function number in the 
function MaxVal. Your fitness will be computed as MaxVal - F(..), which turns 
it into a maximization problem. 

6. If your problem needs external data (such as a graph), use the problem file
setting to store the path and name of this file. Typically, problem data files
are stored in the folder Problems in the project folder.

===============================================================================
||                          Settings file                                    ||
===============================================================================

Every line starting with # is a comment and will be ignored. Also, part of a 
line starting with # until the end of a line will be considered a comment and
ignored.

Here are the options to be set in the file:

** individual size **
Integer, number of genes in a chromosome or individual

** population size **
Integer, number of chromosomes in a generation.

** generation number **
Integer, number of generations to run by trial

** crossover form **
Integer, type of crossover to use. Possible values:
0 = 1-Point crossover
1 = n-Point crossover
2 = Uniform crossover
3 = Dissociated crossover
4 = Combined

** crossover sites **
Integer, number of crossover sites if the crossover is n-Point

** diss form  **
Integer, the form for the dissociated crossover. Possible values:
0 = Using 0 for the intermediary bits
1 = Using bitwise and
2 = Using bitwise xor
3 = Using a random choice of parents' bits

** combined form **
Integer, choice of combination form if the crossover is combined. 
Possible values:
0 = Performance-based; the average fitness achieved by each of the 4 crossovers
    is kept and a fitness-proportionate selection chooses the form in each
	operation.
1 = Balanced; giving each crossover form an equal probability to be chosen
2 = Round-robin, changing the crossover form every 50 generations
3 = Round-robin, changing the crossover form every 20 generations
4 = Adaptive, where the probability of the 1-Point and n-Point crossovers (the
    conservative ones) is increased when the population is very diverse, and 
	the probability of the other 2 (the exploratory ones) is increased when
	the population becomes too homogeneous.
  
** swap probability **
Real number between 0 and 1, probability of swapping the parent genes for the
uniform crossover.

** encode **
Integer, 0 for no encoding and 1 for encoding. If the encoding is turned on, 
assuming that the genes are binary, the chromosomes are encoding by storing 
8 genes per index of the chromosome array and using the binary representation
of the short integer.

** probability of crossover **
Real number between 0 and 1, representing the probability of crossover being 
performed for each selected pair of parents. If the crossover roll is not
successful, the parents are cloned into the children.

** probability of mutation **
Real number between 0 and 1, representing the probability of mutation as 
applied to each gene of the newly children. 

** reproduction form **
Integer, representing the reproduction form from one generation to the next.
Possible values:
0 = Simple reproduction, no measure to conserve good chromosomes into the
    next generation beside the fitness-proportionate selection itself.
1 = Elitist. The best chromosome from the old generation is automatically 
    added to the new generation, whether it's better or not. This adds an
	extra chromosome to the number specified as population size.
2 = Monotone. The best chromosome from the old generation is replaces the
    worst chromosome from the new generation if the old one is better than
	the best new chromosome. This does not add anything to the population
	size.
3 = Sexuate. Monotone plus the parents chosen for crossover are chosen to
	match their reproduction scheme. A male individual can reproduce with a 
	female, a selfMate, and a mixMate as long as it's not itself. A female
	can reproduce with a male, a selfMate, and a mixMate as long as it's not 
	itself. A selfMate can reproduce with anyone, including itself. A mixMate
	can reproduce with anyone other than itself. The reproduction schemes are
	randomly assigned at birth. In the sexuate scheme, children inherit 
	randomly one of their parents' sexes. 
4 = Sex Binary. In this reproduction scheme, chromosomes are initialized 
    randomly as male or female, then the rules for sexuate apply. After 25
    unsuccessful mating attempts were made, the sex of the first parent is
    changed to make mating possible. The children are randomly assigned male or 
    female regardless of their parents' sex.
5 = Hermaphrodite. All the children are defined as mixMate, meaning that they 
    can mate with anyone other than themselves. However, a population radius 
	can be defined so that chromosomes have to search for partners within that
	radius based on the fitness value. 
6 = Diverse. Same as above, but a parent chromosome looks for a partner that is 
    genetically from itself. A number of potential mates (5) are considered,
	selected the usual way, and the one with the most different genetic
    material from the first parent is chosen as a second parent. 

** evaluation form **
Integer, 0 for binary, 1 for real-valued.

** function number **
Integer, number identifying the fitness function.

** var number **
Integer, number of variables represented in the chromosome in case of real-
valued problem representation.

** transformation number ** 
Integer, identifying the transformation number. Possible values:
0 = no transformation
1 = T1(f, xi, i=0,n-1) = Sum(f(xi), i=0, n-1)
2 = T2(f, xi, i=0,n-1) = Sum(fNr(xi, x(i+1)mod 3), i = 0,n-1)
3 = T3(f, x) = Max - F(x); Turns a maximization problem into a minimization one
    or the other way around.

** problem file ** 
String, path to a data file in case the fitness function requires data stored in
a file, such as SAT functions or Hamiltonian Circuit problems where the file 
contains the graph.

** superior limit **
Real number, upper bound for the variables stored in the chromosome.

** inferior limit **
Real number, lower bound for the variables stored in the chromosome.

** history **
Integer, 0 for not keeping the history, 1 for keeping it. More debugging info
than the result file.

** result file **
String, path to the file file storing the results. The file will have a header 
containing some of the settings, followed by the best fitness in each new
generation reported every x generations, where 
x = generation number / number of reports

** history file **
String, path and name of the history file if it is kept.

** random seed **
Integer, seed for the random number generator. The same seed should lead to the
same results. If multiple trials are done, then the random seed is incremented 
by 25 in each trial and re-fed to the generator. This allows for the initial
population to be the same when running trial sets with different parameters.

** number of trials **
Integer, number of trials. In each trial, the population is randomized and the
set number of generations is built again.

** number of reports ** 
Integer, number of intermediary reports of the best fitness in the result file.