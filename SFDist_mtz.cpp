//
//  SFDist_mtz.cpp
//
//
//  Created by Herbert J. Bernstein on 12/27/16.
//
//
/* Function calculatin of distance between two sets of
 structure factors as .mtz files

  Uses SFDist_refl.cpp for the actual distance calculation

  Returns negative values for errors

     -1. for unable to read first file
     -1.1 for unable to get labels from first file 
     -1.2 for needs .mtz extension for first file
     -2. for unable to read second file
     -2.1 for unable to get labels from second file
     -2.2 for needs .mtz extsnions fro second file
     -3. for general error

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


#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <ccp4/cmtzlib.h>
#include <ccp4/mtzdata.h>
#include "label.h"
#include "sfmtzin.h"
#include "sf.h"
#include "utils.h"
#include "defines.h"
#include "rsymop.h"
#include "rsymop_setup.h"

extern "C" double SFDist_refl(std::vector<SF>& refl_list1,std::vector<SF>& refl_list2);

double SFDist_mtz(std::string SF1_name, std::string SF2_name) {  

    std::string SF1_name_ext;
    std::string SF2_name_ext;
    std::string::size_type idx1;
    std::string::size_type idx2;
    CMtz::MTZ *mtz_file1;
    CMtz::MTZ *mtz_file2;
    std::string labels1[Defs::NUM_LABELS];;
    std::string labels2[Defs::NUM_LABELS];;
    std::vector<Label> data_col1;  /* columns to get from mtz_file1 */
    std::vector<Label> data_col2;  /* columns to get from mtz_file2 */
    std::vector<SF> refl1; /* data from mtz_file1 */
    std::vector<SF> refl2; /* data from mtz_file2 */
    
    bool verbose = true;
    double dist;
        
    
    idx1 = SF1_name.rfind('.');
    idx2 = SF2_name.rfind('.');
    SF1_name_ext = std::string("");
    SF2_name_ext = std::string("");
    if (idx1+1<=SF1_name.length()) SF1_name_ext = SF1_name.substr(idx1+1);
    if (idx2+1<=SF2_name.length()) SF2_name_ext = SF2_name.substr(idx2+1);
    
    if(SF1_name_ext.compare("MTZ")==0 ||
       SF1_name_ext.compare("mtz")==0 ){
        try {
           if ( ! (mtz_file1 = CMtz::MtzGet (SF1_name.c_str(), 0)) )
           {
              return -1.0;
           }
        }
        catch (...) {
              return -1.0;
        }
        
        if ( ! sfguess_labels(mtz_file1, data_col1))
        {
           CMtz::MtzFree(mtz_file1);
           return -1.1;
        }
        
        // get data and store in @c refl1
        sfget_data (mtz_file1, data_col1, refl1, verbose);
    } else {
        return -1.2;
    }
    
    
    if(SF2_name_ext.compare("MTZ")==0 ||
       SF2_name_ext.compare("mtz")==0 ){
        try {
           if ( ! (mtz_file2 = CMtz::MtzGet (SF2_name.c_str(), 0)) )
           {
               std::vector<Label>().swap(data_col1);
               std::vector<SF>().swap(refl1);
   	       CMtz::MtzFree(mtz_file1);
               return -2.;
           }
        }
        catch (...) {
               std::vector<Label>().swap(data_col1);
               std::vector<SF>().swap(refl1);
               CMtz::MtzFree(mtz_file1);
               return -2.;
        }
        if ( ! sfguess_labels(mtz_file2, data_col2))
        {           
            std::vector<Label>().swap(data_col1);
            std::vector<SF>().swap(refl1);
            CMtz::MtzFree(mtz_file1);
            std::vector<Label>().swap(data_col2);
            std::vector<SF>().swap(refl2);
            CMtz::MtzFree(mtz_file2);
            return -2.1;
        }
        
        // get data and store in @c refl2
        sfget_data (mtz_file2, data_col2, refl2, verbose);
    } else {
        std::vector<Label>().swap(data_col1);
        std::vector<SF>().swap(refl1);
        CMtz::MtzFree(mtz_file1);
        return -2.2;
    }
    
    dist =  SFDist_refl(refl1,refl2);
    std::vector<Label>().swap(data_col1);
    std::vector<Label>().swap(data_col2);
    std::vector<SF>().swap(refl1);
    std::vector<SF>().swap(refl2);
    CMtz::MtzFree(mtz_file1);
    CMtz::MtzFree(mtz_file2);

    return dist;
}
