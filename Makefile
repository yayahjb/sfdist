PREFIX ?= $(CCP4)

RCPP ?= /usr/lib/R/site-library/Rcpp
RCPPARMA ?= /usr/lib/R/site-library/RcppArmadillo
RCPPPARA ?= /usr/local/lib/R/site-library/RcppParallel/
RPATH ?= /usr/share/R

bin:
	mkdir -p bin

lib:
	mkdir -p lib

build:
	mkdir -p build


all:	lib/librcpp_sfdist.so bin/sfdist

install:  all
	cp lib/librcpp_sfdist.so $(PREFIX)/lib/librcpp_sfdist.so
	chmod 755 $(PREFIX)/lib/librcpp_sfdist.so 
	cp bin/sfdist $(PREFIX)/bin/sfdist
	chmod 755 $(PREFIX)/bin/sfdist

clean:
	-@rm -f *.o

distclean:  clean
	-@rm -rf bin
	-@rm -rf lib
	-@rm -rf build

SFDist_refl.o: SFDist_refl.cpp sf.h
	g++ -g -O3 -fopenmp -fpic -O3 -fPIC   -I. -c SFDist_refl.cpp

SFDist_mtz.o: SFDist_mtz.cpp
	g++ -g -O3 -fopenmp -fpic -O3 -fPIC   -I. -c SFDist_mtz.cpp

rcpp_SFDist_mtz.o: rcpp_SFDist_mtz.cpp
	g++ -g -O3 -fopenmp -fpic -O3 -fPIC   -I. -c rcpp_SFDist_mtz.cpp

bin/sfdist:  bin SFDist_refl.o SFDist_mtz.o sfdist.cpp sf.cpp sf.h \
	sfmtzin.cpp sfmtzin.h label.cpp label.h utils.cpp utils.h
	g++ -g -O3 -fopenmp  -O3  -o bin/sfdist -I. SFDist_refl.o SFDist_mtz.o \
	sfdist.cpp  sf.cpp sfmtzin.cpp label.cpp utils.cpp -L $(PREFIX)/lib -lccp4c

lib/librcpp_sfdist.so: lib SFDist_refl.o rcpp_SFDist_mtz.o rcpp_sfdist.cpp sf.cpp sf.h sfmtzin.cpp \
                sfmtzin.h label.cpp label.h utils.cpp utils.h
	g++ -shared -g  -fopenmp -I$(RPATH)/include -DNDEBUG -fpic -O3 -fPIC  \
	-Wl,-rpath=$(PREFIX)/lib \
	-o lib/librcpp_sfdist.so -I.  -I$(RCPP)/include -I$(RCPPPARA)/include \
	-I$(RCPPARMA)/include -I$(RPATH)/include SFDist_refl.o rcpp_SFDist_mtz.o rcpp_sfdist.cpp \
	sf.cpp sfmtzin.cpp label.cpp utils.cpp \
	-L $(PREFIX)/lib  -lccp4c -lR -lblas -llapack -lpthread


