################################################################
## Project GADX, a C++ implementation of genetic algorithms   ##
## License: Creative Commons, Attribution                     ##
## Author: Dana Vrajitoru                                     ##
## Updated: March 2019                                        ##
## Makefile                                                   ##
################################################################

#folders being used
CSPATH = ./src
BINPATH = ./bin
LIBPATH = ./lib

# the source files 

types_sources = General.cc \
				CrossMethod.cc \
				RunInfo.cc \
				Individual.cc \
				Generation.cc \
				GenInfo.cc \
				EvalInfo.cc \
				SATclause.cc \
				SATexpr.cc


evaluation_sources = GenEval.cc	\
					EvalBool.cc \
					EvalReal.cc \
					SettingFile.cc

operation_sources = GenOper.cc GAManager.cc

raw_sources = $(types_sources) $(evaluation_sources) $(operation_sources) 

sources=$(addprefix src/,$(raw_sources)) 

# objects are the same as the source files with .cc replaced with .o

objects = $(addprefix lib/,$(raw_sources:.cc=.o))

# headers are the same as the source files with .cc replaced with .h

headers = $(rawsources:.c=.h)

# and we have a few more

headers += GenTypes.h

# actually, we don't use $headers...

# we will want to recompile if the Makefile (this file) changes...

#others = Makefile


AR          = ar
ARFLAGS     = rvs
CC          = gcc
CLINKER     = gcc
CXX         = g++
CXXFLAGS    = -O
CCFLAGS     = -O
LIB_DIRS    =  
LIBS        = 

default: all

# make all will run commands regardless of whether there is a file "all"
all:	GAD

GAD:	lib/main.o $(objects)
	$(CXX) $(OPTFLAGS) -o bin/GAD lib/main.o $(objects) $(LIB_DIRS) $(LIBS)

# make clean will run commands regardless of whether there is a file "clean"
clean:
	rm -rf lib/*.o
	rm -f bin/GAD

lib/%.o : src/%.c
	$(CC) $(CCFLAGS) $(CFLAGS) -c $< $(OUTPUT_OPTION)

lib/%.o : src/%.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE_DIRS) $(TARGET_ARCH) -c $< -o $@








