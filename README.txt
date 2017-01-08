                     sfdist -- Structure Factor Distance utilities
                     Herbert J. Bernstein and Lawrence C. Andrews
                               Version 1.0.0 07 Jan 2016

                                  yayahjb@gmail.com

sfdist is a package of two utilities built on build on a variant of the Pearson
Correlation coefficient commonly used in crystallography to compare unscaled
sets of reflections, to allow for sets of reflections with very different
completeness.  Reflections that do not have matching [h,k,l] in both sets are
given a penalty distance.  The reflections are weighted by the reciprocals of
the sigmas to reduce swamping of results by strong refection, and the correlation,
which is a similarity, rather than a distance is converted to a distance.

A shell command line program sfdist is provided as well as a shared library 
rcpp_sfdist (librcpp_sfdist.so)  for use of ncdist from R.

#####################################################################
#                                                                    #
# YOU MAY REDISTRIBUTE THE sfdist PACKAGE UNDER THE TERMS OF THE GPL #
#                                                                    #
# ALTERNATIVELY YOU MAY REDISTRIBUTE THE sfdist API UNDER THE TERMS  #
# OF THE LGPL                                                        #
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

THE COMMAND LINE PROGRAM SFDIST

 Created by Herbert J. Bernstein on 12/27/16

 Command-line calculation of distance between two sets of
 structure factors as .mtz files
 
 The distance is computed from two sets of F's indexed on HKL
 Any pruning on resolution or other criteria should be done
 before processing with SFDist.  The input data consists of
 
 SF1 -- first set of structure factors
 SF2 -- second set of structure factors
 
 Derived in part from
 http://shelx.uni-ac.gwdg.de/~tg/research/programs/conv/mtz2x/mtz2sca/binaries/0.4/mtz2sca_v0.4.tar.gz

 If SF1 is file1.mtz and SF2 is file2.mtz, the call is

 sfdist file1.mtz file2.mtz

 As of this writing, only .mtz files are accepted.   Other formats, such as
 .HKL should be converted externally.  The data may be anomalous or non-anomalous.
 
THE RCPP INTERFACE TO SFDIST

The library rcpp_ncdist is provided as an interface between R and sfdist.  Depending on
the type of system used, the name of the file may be a .dylib, a .dll, a .so and may or may
not be prefixed by lib.  Under linux the name is librcpp_ncdist.so.  Under OS X the name
is librcpp_ncdist.dylib.

Assuming the library is named /usr/local/lib/librcpp_sfdist.so, and the two files to be
compared are "file1.mtz", "file2.mtz") in order to compute a cell distance in R, 
the commands are:

> library("Rcpp")
> dyn.load("/usr/local/lib/librcpp_sfdist.so")
> .Call("rcpp_sfdist", "file1.mtz", "file2.mtz")

