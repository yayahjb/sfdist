//
//  rcpp_sfdist.cpp
//
//
//  Created by Herbert J. Bernstein on 12/27/16.
//
//
/* R function calculation of distance between two sets of
 structure factors as .mtz files
 
 The distance is computed from two sets of F's indexed on HKL
 Any pruning on resolution or other criteria should be done
 before processing with sfdist.  The input data consists of
 
 SF1 -- first set of structure factors
 SF2 -- second set of structure factors
 
 Needs:
 https://github.com/dials/ccp4io.git
 
 Derived in part from
 http://shelx.uni-ac.gwdg.de/~tg/research/programs/conv/mtz2x/mtz2sca/binaries/0.4/mtz2sca_v0.4.tar.gz
 
 License:  LGPL
 
 
 */


#include <Rcpp.h>
#include <RcppParallel.h>
using namespace RcppParallel;
#include "defines.h"

double SFDist_mtz(std::string SF1_name, std::string SF2_name);

extern "C" SEXP rcpp_sfdist ( SEXP SF1_name_, SEXP SF2_name_) {
    
    std::string SF1_name = Rcpp::as<std::string>(SF1_name_);
    std::string SF2_name = Rcpp::as<std::string>(SF2_name_);;
    double dist;

    dist = SFDist_mtz(SF1_name,SF2_name);

        if (dist < 0.) {
        if (-1.001 < dist && dist < 0.999) {
            Rcpp::stop("rcpp_sfdist: Unable to open first file \n");
        }
        if (-1.101 < dist && dist < -1.099 ) {
            Rcpp::stop("rcpp_sfdist: Unable to guess labels from firsy file\n");
        }
        if (-1.201 < dist && dist < -1.199 ) {
            Rcpp::stop("rcpp_sfdist: first file must have an mtz extension\n");
        }
        if (-2.001 < dist && dist < -1.999 ) {
            Rcpp::stop("rcpp_sfdist: Unable to open second file\n");
        }
        if (-2.101 < dist && dist < -2.099 ) {
            Rcpp::stop("rcpp_sfdist: Unable to guess labels from second file\n");
        }   
        if (-2.201 < dist && dist < -2.199 ) {
            Rcpp::stop("rcpp_sfdist: second file must have an mtz extension\n");
        }

        Rcpp::stop("rcpp_sfdist: error\n");

    }


    return Rcpp::wrap( dist );
}


