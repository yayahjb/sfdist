######################################################################
#  Makefile - makefile to create sfdist                              #
#                                                                    #
# Version 1.0.0 07 January 2016                                      #
#                                                                    #
#         Herbert J. Bernstein (yayahjb@gmail.com)                   #
#         Lawrence C Andrews                                         #
#                                                                    #
# (C) Copyright 2016-2017 Herbert J. Bernstein, Lawrence C. Andrews  #
#                                                                    #
######################################################################

######################################################################
#                                                                    #
# YOU MAY REDISTRIBUTE THE sfdist PACKAGE UNDER THE TERMS OF THE GPL #
#                                                                    #
# ALTERNATIVELY YOU MAY REDISTRIBUTE THE sfdist API UNDER THE TERMS  #
# OF THE sfdist                                                      #
#                                                                    #
######################################################################

########################### GPL NOTICES ##############################
#                                                                    #
# This program is free software; you can redistribute it and/or      #
# modify it under the terms of the GNU General Public License as     #
# published by the Free Software Foundation; either version 2 of     #
# (the License, or (at your option) any later version.               #
#                                                                    #
# This program is distributed in the hope that it will be useful,    #
# but WITHOUT ANY WARRANTY; without even the implied warranty of     #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      #
# GNU General Public License for more details.                       #
#                                                                    #
# You should have received a copy of the GNU General Public License  #
# along with this program; if not, write to the Free Software        #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA           #
# 02111-1307  USA                                                    #
#                                                                    #
######################################################################

######################### LGPL NOTICES ###############################
#                                                                    #
# This library is free software; you can redistribute it and/or      #
# modify it under the terms of the GNU Lesser General Public         #
# License as published by the Free Software Foundation; either       #
# version 2.1 of the License, or (at your option) any later version. #
#                                                                    #
# This library is distributed in the hope that it will be useful,    #
# but WITHOUT ANY WARRANTY; without even the implied warranty of     #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  #
# Lesser General Public License for more details.                    #
#                                                                    #
# You should have received a copy of the GNU Lesser General Public   #
# License along with this library; if not, write to the Free         #
# Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,    #
# MA  02110-1301  USA                                                #
#                                                                    #
######################################################################

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
	g++ -g -O3 -fopenmp -fpic -fPIC   -I. -c SFDist_refl.cpp

SFDist_mtz.o: SFDist_mtz.cpp
	g++ -g -O3 -fopenmp -fpic -fPIC   -I. -c SFDist_mtz.cpp

bin/sfdist:  bin SFDist_refl.o SFDist_mtz.o sfdist.cpp sf.cpp sf.h \
	sfmtzin.cpp sfmtzin.h label.cpp label.h utils.cpp utils.h
	g++ -g -O3 -fopenmp   -o bin/sfdist -I. SFDist_refl.o SFDist_mtz.o \
	sfdist.cpp  sf.cpp sfmtzin.cpp label.cpp utils.cpp -L $(PREFIX)/lib -lccp4c

lib/librcpp_sfdist.so: lib SFDist_refl.o SFDist_mtz.o rcpp_sfdist.cpp sf.cpp sf.h sfmtzin.cpp \
                sfmtzin.h label.cpp label.h utils.cpp utils.h
	g++ -shared -g  -fopenmp -I$(RPATH)/include -DNDEBUG -fpic -O3 -fPIC  \
	-Wl,-rpath=$(PREFIX)/lib \
	-o lib/librcpp_sfdist.so -I.  -I$(RCPP)/include -I$(RCPPPARA)/include \
	-I$(RCPPARMA)/include -I$(RPATH)/include SFDist_refl.o SFDist_mtz.o rcpp_sfdist.cpp \
	sf.cpp sfmtzin.cpp label.cpp utils.cpp \
	-L $(PREFIX)/lib  -lccp4c -lR -lblas -llapack -lpthread


