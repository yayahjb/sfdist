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

/* Compute the lower triangle of distances among a set of mtz files
 , not including the diagonal */

extern "C" SEXP rcpp_sfdists ( SEXP SF_names_) {
    
    std::string SF1_name;
    std::string SF2_name;
    std::vector< std::string > SF_names = Rcpp::as<std::vector <std::string> >(SF_names_);
    std::map< std::pair<int,int>,  double > distances;
    std::vector< std::pair< double, std::pair<int,int> > > retvals;
    std::vector<double> dists;
    int i, j, k;
    double dist;
    double retval;

    if (SF_names.size() > 1) {
        retval = 0.;
#pragma omp parallel for private(SF1_name) schedule(dynamic)
        for (i=0; i < SF_names.size(); i++) {
            SF1_name = std::string(SF_names[i]);
#pragma omp parallel for private(SF2_name, dist) schedule(dynamic)
            for (j=i+1; j < SF_names.size(); j++) {
                SF2_name = std::string(SF_names[j]);
                dist = SFDist_mtz(SF1_name,SF2_name);
                Rcpp::Rcout << SF1_name <<" "<<SF2_name<<" "<<dist<<std::endl;
#pragma omp criticalsection(savedistance)
                {
                    distances[std::make_pair(i,j)] = dist;
                }
                if (dist < 0.) {
                    retvals.push_back(std::make_pair(dist,std::make_pair(i,j)));
                }
            }
        }
     } else { 
         retvals.push_back(std::make_pair(-3.,std::make_pair(-1,-1)));
     }
     if (retvals.size() > 0) {
         for (k=0; k < retvals.size(); k++) {
             retval = retvals[k].first;
             i = retvals[k].second.first;
             j = retvals[k].second.second;
             SF1_name = "";
             SF2_name = "";
             if (i >= 0) SF1_name = std::string(SF_names[i]);
             if (j >= 0) SF2_name = std::string(SF_names[j]);
             if (retval <  0.) {
                 if (-1.001 < retval && retval < 0.999) {
                     Rcpp::Rcerr << "rcpp_sfdist: Unable to open "<< SF1_name << std::endl; 
                 } else if (-1.101 < retval && retval < -1.099 ) {
                     Rcpp::Rcerr << "rcpp_sfdist: Unable to guess labels from" << SF1_name << std::endl;
                 } else if (-1.201 < retval && retval < -1.199 ) {
                     Rcpp::Rcerr << "rcpp_sfdist: " << SF1_name << " must have an mtz extension" << std::endl;
                 } else if (-2.001 < retval && retval < -1.999 ) {
                     Rcpp::Rcerr << "rcpp_sfdist: Unable to open "<< SF2_name << std::endl; 
                 } else if (-2.101 < retval && retval < -2.099 ) {
                     Rcpp::Rcerr << "rcpp_sfdist: Unable to guess labels from" << SF2_name << std::endl;
                 } else if (-2.201 < retval && retval < -2.199 ) {
                     Rcpp::Rcerr << "rcpp_sfdist: " << SF2_name << " must have an mtz extension" << std::endl;
                 }
            }
        }
        Rcpp::stop("rcpp_sfdist: error\n");
    }

    for (i=0; i < SF_names.size(); i++) {
        for (j=i+1; j < SF_names.size(); j++) {
           dists.push_back(distances[std::make_pair(i,j)]);
        }
    }

    return Rcpp::wrap( dists );
}
/* Compute the one distance between two mtz files */

extern "C" SEXP rcpp_sfdist ( SEXP SF1_name_, SEXP SF2_name_) {
    
    std::string SF1_name = Rcpp::as<std::string>(SF1_name_);
    std::string SF2_name = Rcpp::as<std::string>(SF2_name_);;
    double dist;

    dist = SFDist_mtz(SF1_name,SF2_name);
    /*  Rcpp::Rcout << SF1_name <<" "<<SF2_name<<" "<<dist<<std::endl; */
    if (dist <  0.) {
        if (-1.001 < dist && dist < 0.999) {
            Rcpp::Rcerr << "rcpp_sfdist: Unable to open "<< SF1_name << std::endl; 
        } else if (-1.101 < dist && dist < -1.099 ) {
            Rcpp::Rcerr << "rcpp_sfdist: Unable to guess labels from" << SF1_name << std::endl;
        } else if (-1.201 < dist && dist < -1.199 ) {
            Rcpp::Rcerr << "rcpp_sfdist: " << SF1_name << " must have an mtz extension" << std::endl;
        } else if (-2.001 < dist && dist < -1.999 ) {
            Rcpp::Rcerr << "rcpp_sfdist: Unable to open "<< SF2_name << std::endl; 
        } else if (-2.101 < dist && dist < -2.099 ) {
            Rcpp::Rcerr << "rcpp_sfdist: Unable to guess labels from" << SF2_name << std::endl;
        } else if (-2.201 < dist && dist < -2.199 ) {
            Rcpp::Rcerr << "rcpp_sfdist: " << SF2_name << " must have an mtz extension" << std::endl;
        }
        Rcpp::stop("rcpp_sfdist: error\n");
    }

    return Rcpp::wrap( dist );
}



