
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs) 

CXX = g++
CXXFLAGS = -g -O -fPIC -Wall -Wno-deprecated
LD = g++

SOFLAGS = 

CXXFLAGS     += $(ROOTCFLAGS) 

LIBS         = ${ROOTGLIBS} ${ROOTLIBS}

all : ../test/runTrackDUTanalyzer

clean:
	rm *.o
	rm test*

objects=TrackDUTanalyzer.o

../test/runTrackDUTanalyzer: $(objects)
	$(CXX) -o ../test/runTrackDUTanalyzer $(objects) ${LIBS}  $(CXXFLAGS) ${SOFLAGS}

TrackDUTanalyzer.o: TrackDUTanalyzer.C ReadInputTrees.h DUTanalysis.h TrackAnalysis.h
	$(CXX) ${LIBS} ${CXXFLAGS} -c TrackDUTanalyzer.C
