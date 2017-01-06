//
//  SFDist.cpp
//
//
//  Created by Herbert J. Bernstein on 12/27/16.
//
//
/* Command-line calculation of distance between two sets of
 structure factors as .mtz files
 
 The distance is computed from two sets of F's indexed on HKL
 Any pruning on resolution or other criteria should be done
 before processing with SFDist.  The input data consists of
 
 SF1 -- first set of structure factors
 SF2 -- second set of structure factors
 
 Needs:
 https://github.com/dials/ccp4io.git
 
 Derived in part from
 http://shelx.uni-ac.gwdg.de/~tg/research/programs/conv/mtz2x/mtz2sca/binaries/0.4/mtz2sca_v0.4.tar.gz
 
 License:  LGPL
 
 
 */


#include <string>
#include <iostream>
#include "defines.h"

double SFDist_mtz(std::string SF1_name, std::string SF2_name);

int main(int argc, char ** argv) {
    
    std::string SF1_name;
    std::string SF2_name;
    double dist;

    dist = -3.;    

    if (argc > 2) {

        SF1_name = std::string(argv[1]);
        SF2_name = std::string(argv[2]);

        dist = SFDist_mtz(SF1_name,SF2_name);

    }

    if (dist < 0.) {
        if (-1.001 < dist && dist < 0.999) {
            std::cerr << "Unable to open file " << SF1_name << ".\n";
            return Defs::ERROR_FILE_OPEN;
        }
        if (-1.101 < dist && dist < -1.099 ) {
            std::cerr << "Unable to guess labels from " << SF1_name << "\n";
            return Defs::NEED_LABELS;   
        }
        if (-1.201 < dist && dist < -1.199 ) {
            std::cerr << SF1_name << " must have an mtz extension"  << std::endl;
            return -1;
        }
        if (-2.001 < dist && dist < -1.999 ) {
            std::cerr << "Unable to open file " << SF2_name << ".\n";
            return Defs::ERROR_FILE_OPEN;
        }
        if (-2.101 < dist && dist < -2.099 ) {
            std::cerr << "Unable to guess labels from " << SF2_name << "\n";
            return Defs::NEED_LABELS;
        }   
        if (-2.201 < dist && dist < -2.199 ) {
            std::cerr << SF2_name << " must have an mtz extension"  << std::endl;
            return -1;
        }

        std::cerr
        << "Usage: SFDist SF1 SF2"
        << std::endl;
        return -1;

    }

    std::cout << dist << std::endl;

    return 0;

}
