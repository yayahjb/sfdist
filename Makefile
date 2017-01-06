PREFIX ?= $(HOME)
CCP4IO_URL ?= https://github.com/dials/ccp4io.git
MMDB ?= mmdb-1.25.6.1
MMDB_URL ?= https://launchpad.net/mmdb/1.25/1.25.6.1/+download/$(MMDB).tar.gz


#RCPP ?= /usr/lib/R/site-library/Rcpp
RCPP ?=/usr/lib64/R/library/Rcpp/
#RCPPPARA ?= /usr/local/lib/R/site-library/RcppParallel/
RCPPPARA ?= /usr/lib64/R/library/RcppParallel/
RCPPARMA ?= /usr/lib64/R/library/RcppArmadillo/
#RPATH ?= /usr/share/R/
RPATH ?= /usr/lib64/R/

ccp4io:
	git clone $(CCP4IO_URL)
	
libccp4:	ccp4io mmdb
	(cd ccp4io/libccp4; \
	make distclean; \
	LDFLAGS = "$(LDFLAGS) -L $(PREFIX)/lib"; \
	export $(LDFLAGS); \
	FFLAGS = "$(FFLAGS) $(LDFLAGS)"; export $(FFLAGS); \
	bash ./configure --prefix=$(PREFIX) --enable-shared; \
	make install)
	ln -s ccp4io/libccp4/ccp4 ccp4

mmdb:
	curl -O -L $(MMDB_URL)
	gunzip < $(MMDB).tar.gz | tar xvf -
	(cd $(MMDB); \
	make distclean; \
	LDFLAGS = "$(LDFLAGS) -L $(PREFIX)/lib"; \
	 export $(LDFLAGS); \
	bash ./configure --prefix=$(PREFIX) --enable-shared; \
	make install)



SFDist_refl.o: SFDist_refl.cpp sf.h
	g++ -g -O3 -fopenmp -fpic -O3 -fPIC   -I. -c SFDist_refl.cpp

SFDist_mtz.o: SFDist_mtz.cpp
	g++ -g -O3 -fopenmp -fpic -O3 -fPIC   -I. -c SFDist_mtz.cpp

rcpp_SFDist_mtz.o: rcpp_SFDist_mtz.cpp
	g++ -g -O3 -fopenmp -fpic -O3 -fPIC   -I. -c rcpp_SFDist_mtz.cpp

sfdist:	SFDist_refl.o SFDist_mtz.o sfdist.cpp sfdist.hpp sf.cpp sf.h sfmtzin.cpp sfmtzin.h label.cpp label.h utils.cpp utils.h
	g++ -g -O3 -fopenmp  -O3  -static -o sfdist -I. SFDist_refl.o SFDist_mtz.o sfdist.cpp  sf.cpp sfmtzin.cpp label.cpp utils.cpp -L $(HOME)/lib -lccp4c

rcpp_sfdist.so:	SFDist_refl.o rcpp_SFDist_mtz.o \
                rcpp_sfdist.cpp sfdist.hpp sf.cpp sf.h sfmtzin.cpp \
                sfmtzin.h label.cpp label.h utils.cpp utils.h
	g++ -shared -g  -fopenmp -I$(RPATH)/include -DNDEBUG -fpic -O3 -fPIC  -o rcpp_sfdist.so -I.  -I$(RCPP)/include -I$(RCPPPARA)/include \
 	-I$(RCPPARMA)/include SFDist_refl.o rcpp_SFDist_mtz.o rcpp_sfdist.cpp  sf.cpp sfmtzin.cpp label.cpp utils.cpp \
	-L $(HOME)/lib -L/usr/lib/R/lib  -lccp4c -lR -lblas -llapack -lpthread


