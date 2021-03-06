//
//  SFDist.cpp
//
//
//  Created by Herbert J. Bernstein on 12/27/16.
//
//
/* Calculation of distance between two sets of
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

extern "C" double SFDist_refl(std::vector<SF>& refl_list1,std::vector<SF>& refl_list2) {
    std::map< ssize_t, std::pair<int, SF> > reflmap1;
    std::map< ssize_t, std::pair<int, SF> > reflmap2;
    std::map< ssize_t, std::pair<int, SF> >::iterator itmap;
    std::vector< std::map< ssize_t, std::pair<int, SF> >::iterator > reflmap1_it_vector;
    SF curSF;
    int r, icur;
    
    size_t nrefl1;
    size_t nrefl2;
    size_t nreflmap1;
    size_t unique1;
    size_t unique2;
    
    size_t i1, i2;
    
    int hc, kc, lc;
    int h1, k1, l1;
    int h2, k2, l2;
    int hchigh, hclow, kchigh, kclow, lchigh, lclow;
    int hchigh1, hclow1, kchigh1, kclow1, lchigh1, lclow1;
    int hchigh2, hclow2, kchigh2, kclow2, lchigh2, lclow2;
    size_t key;
    
    size_t ncommon;
    bool first1, first2;
    double Fp, sFp, Fm, sFm, Fp2, sFp2, Fm2, sFm2;
    double Fsum1, Fsumsq1, Fsum2, Fsumsq2;
    double weight1, weight2;
    double norm1, norm2, mean1, mean2;
    double dist, distsq;
    
    nrefl1 = refl_list1.size();
    nrefl2 = refl_list2.size();
    
    first1 = true;
    first2 = true;   
    /* collect the HKL ranges  for the reflection lists*/
    
    for ( i1 = 0; i1 < nrefl1; i1++)
    {
        h1  = refl_list1[i1].get_h();
        k1  = refl_list1[i1].get_k();
        l1  = refl_list1[i1].get_l();
        {
            if (first1) {
                hchigh1 = hclow1 = h1;
                kchigh1 = kclow1 = k1;
                lchigh1 = lclow1 = l1;
                first1 = false;
            } else {
                if (h1 > hchigh1) hchigh1 = h1;
                if (h1 < hclow1) hclow1 = h1;
                if (k1 > kchigh1) kchigh1 = k1;
                if (k1 < kclow1) kclow1 = k1;
                if (l1 > lchigh1) lchigh1 = l1;
                if (l1 < lclow1) lclow1 = l1;
            }
        }
    }
    for ( i2 = 0; i2 < nrefl1; i2++)
    {
        h2  = refl_list2[i2].get_h();
        k2  = refl_list2[i2].get_k();
        l2  = refl_list2[i2].get_l();
        {
            if (first2) {
                hchigh2 = hclow2 = h2;
                kchigh2 = kclow2 = k2;
                lchigh2 = lclow2 = l2;
            } else {
                if (h2 > hchigh2) hchigh2 = h2;
                if (h2 < hclow2) hclow2 = h2;
                if (k2 > kchigh2) kchigh2 = k2;
                if (k2 < kclow2) kclow2 = k2;
                if (l2 > lchigh2) lchigh2 = l2;
                if (l2 < lclow2) lclow2 = l2;
            }
        }
    }
    hchigh = hchigh2;
    if (hchigh1 > hchigh2) hchigh = hchigh1;
    kchigh = kchigh2;
    if (kchigh1 > kchigh2) kchigh = kchigh1;
    lchigh = lchigh2;
    if (lchigh1 > lchigh2) lchigh = lchigh1;
    hclow  = hclow2;
    if (hclow1  < hclow2)  hclow  = hclow1;
    kclow = kclow2;
    if (kclow1  < kclow2)  kclow  = kclow1;
    lchigh = lclow2;
    if (lclow1  < lclow2)  lclow  = lclow1;

    /* convert each reflection list to a map, averaging the duplicates */
    r = 0;
    unique1 = 0;

    for ( i1 = 0; i1 < nrefl1; i1++)
    {
        h1  = refl_list1[i1].get_h();
        k1  = refl_list1[i1].get_k();
        l1  = refl_list1[i1].get_l();
        Fp  = refl_list1[i1].get_Fplus();
        sFp = refl_list1[i1].get_sigFplus();
        Fm  = refl_list1[i1].get_Fminus();
        sFm = refl_list1[i1].get_sigFminus();
        key = h1-hclow + (1+hchigh-hclow)*(k1-kclow)+ (1+hchigh-hclow)*(1+kchigh-kclow)*(l1-lclow);
        {
            if (reflmap1.count(key) == 0) {
                if (sFp > 0. && sFm > 0.) {
                    unique1++;
                    reflmap1[key] = std::make_pair(1,refl_list1[i1]);
#ifdef DEBUG
                    if (r < 20 ) {
                        std::clog << "reflmap1 key: " << key << ": ";
                        SF::print(reflmap1[key].second);
                    }
#endif
                }
            } else {
                icur = reflmap1[key].first;
                curSF = reflmap1[key].second;
                if ( curSF.get_sigFplus() > 0. && curSF.get_sigFminus() >0. ) {
                    Fp  = (Fp+ icur*curSF.get_Fplus())/((double)(icur+1));
                    Fm  = (Fm+ icur*curSF.get_Fminus())/((double)(icur+1));
                    sFp = std::sqrt(((sFp*sFp)
                                     + icur*curSF.get_sigFplus()*curSF.get_sigFplus())/((double)(icur+1)));
                    sFm = std::sqrt(((sFm*sFm)
                                     + icur*curSF.get_sigFminus()*curSF.get_sigFminus())/((double)(icur+1)));
                    reflmap1[key].first=icur+1;
                    reflmap1[key].second=SF(h1,k1,l1,Fp,sFp,Fm,sFm);
                }
            }
        }
        r++;
    }
    
    r = 0;
    unique2 = 0;
    
    for ( i2 = 0; i2 < nrefl1; i2++)
    {
        h2  = refl_list2[i2].get_h();
        k2  = refl_list2[i2].get_k();
        l2  = refl_list2[i2].get_l();
        Fp  = refl_list2[i2].get_Fplus();
        sFp = refl_list2[i2].get_sigFplus();
        Fm  = refl_list2[i2].get_Fminus();
        sFm = refl_list2[i2].get_sigFminus();
        key = h2-hclow + (1+hchigh-hclow)*(k2-kclow)+ (1+hchigh-hclow)*(1+kchigh-kclow)*(l2-lclow);
        {
            if (reflmap2.count(key) == 0) {
                if (sFp > 0. && sFm > 0.) {
                    unique2++;
                    reflmap2[key] = std::make_pair(1,refl_list2[i2]);
#ifdef DEBUG
                    if (r < 20 ) {
                        std::clog << "reflmap2 key: " << key << ": ";
                        SF::print(reflmap2[key].second);
                    }
#endif
                }
            } else {
                icur = reflmap2[key].first;
                curSF = reflmap2[key].second;
                if ( curSF.get_sigFplus() > 0. && curSF.get_sigFminus() > 0 ) {
                    Fp  = (Fp+ icur*curSF.get_Fplus())/((double)(icur+1));
                    Fm  = (Fm+ icur*curSF.get_Fminus())/((double)(icur+1));
                    sFp = std::sqrt(((sFp*sFp)
                                     + icur*curSF.get_sigFplus()*curSF.get_sigFplus())/((double)(icur+1)));
                    sFm = std::sqrt(((sFm*sFm)
                                     + icur*curSF.get_sigFminus()*curSF.get_sigFminus())/((double)(icur+1)));
                    reflmap2[key].first = icur+1;
                    reflmap2[key].second = SF(h2,k2,l2,Fp,sFp,Fm,sFm);
                }
            }
        }
        r++;
    }
    
    /* find the common reflections
     compute Fsum1 = sum(Fp_i/sFp_i+Fm_i/sFp_i)
     
     */
    
    ncommon = 0;
    Fsum1 = Fsum2 = 0.;
    Fsumsq1 = Fsumsq2 = 0.;
    weight1 = 0.;
    weight2 = 0.;

   /* Collect the reflmap1 iterators into a vector */

    for ( itmap  = reflmap1.begin(); itmap != reflmap1.end(); itmap++) reflmap1_it_vector.push_back(itmap);
    nreflmap1 = reflmap1_it_vector.size();

    {
        for ( i1 = 0; i1 < nreflmap1; i1++)
        {
            itmap = reflmap1_it_vector[i1];
            h1  = (itmap->second).second.get_h();
            k1  = (itmap->second).second.get_k();
            l1  = (itmap->second).second.get_l();
            key = h1-hclow + (1+hchigh-hclow)*(k1-kclow)+ (1+hchigh-hclow)*(1+kchigh-kclow)*(l1-lclow);
            if (reflmap2.count(key) ==  0) continue;
            else {
                Fp = (reflmap1[key].second).get_Fplus();
                Fm = (reflmap1[key].second).get_Fminus();
                sFp = (reflmap1[key].second).get_sigFplus();
                sFm = (reflmap1[key].second).get_sigFminus();
                Fp2 = (reflmap2[key].second).get_Fplus();
                Fm2 = (reflmap2[key].second).get_Fminus();
                sFp2 = (reflmap2[key].second).get_sigFplus();
                sFm2 = (reflmap2[key].second).get_sigFminus();
                
                {
                    ncommon++;
                    
                    if (sFp > 0. && sFm > 0.) {
                        Fsum1 += Fp/sFp; Fsumsq1 += Fp*Fp/(sFp*sFp);
                        Fsum1 += Fm/sFm; Fsumsq1 += Fm*Fm/(sFm*sFm);
                        weight1 += (1./sFp+1./sFm);
                    }
                    if (sFp2 > 0. && sFm2 > 0.){
                        Fsum2 += Fp2/sFp2; Fsumsq2 += Fp2*Fp2/(sFp2*sFp2);
                        Fsum2 += Fm2/sFm2; Fsumsq2 += Fm2*Fm2/(sFm2*sFm2);
                        weight2 += (1./sFp2+1./sFm2);
                    }
                }
                
#ifdef DEBUG
                if (ncommon < 1000 && ncommon > 1) {
                    mean1 = Fsum1/weight1;
                    mean2 = Fsum2/weight2;
                    if (Fsumsq1 < mean1*mean1 ||
                        Fsumsq2 < mean2*mean2) {
                        SF::print(reflmap1[key].second);
                        SF::print(reflmap2[key].second);
                        std::cerr << "Fsumsq1 << mean1*mean1 || Fsumsq2 << mean2*mean2, ncommmon" << std::endl;
                        std::cerr << "ncommon, mean1^2, mean2^2, Fsumsq1, Fsumsq2, weight1, weight2 "
                        << ncommon <<","<< mean1*mean2 <<","<< mean2*mean2 <<","<< Fsumsq1 <<","<< Fsumsq2 <<","<< weight1 $
                    }
                }
                
                if (ncommon < 50) {
                    std::cerr << "ncommon, Fsum1, Fsum2, Fsumsq1, Fsumsq: "
                    << ncommon << ", "
                    <<  Fsum1 << ", "
                    <<  Fsum2 << ", "
                    <<  Fsumsq1 << ", "
                    <<  Fsumsq2 << std::endl;
                    SF::print(reflmap2[key].second);
                }
                
                if (Fp != Fp2 || Fm != Fm2 && r > 50) {
                    SF::print((reflmap1[key].second));
                    SF::print((reflmap2[key].second));
                    
                }
#endif
                
            }
            
        }
    }
    
    
#ifdef DEBUG
    std::cerr << "ncommon, unique1, unique2: "
    << ncommon << ", "
    << unique1 << ", "
    << unique2 << " " << std::endl;
    
    
    std::cerr << "Fsum1, Fsum2, Fsumsq1, Fsum2sq1, weight1, weight2: "
    << Fsum1 << ", "
    << Fsum2 << ", "
    << Fsumsq1 << ", "
    << Fsumsq2 << " "
    << weight1 << ", "
    << weight2 << " "
    << std::endl;
#endif
    
    
    if (ncommon != 0) {
        
        mean1 = Fsum1/weight1;
        mean2 = Fsum2/weight2;
        norm1 = norm2 = 0.;
        
        if (Fsumsq1 - mean1*mean1 > 0.)
            norm1 = std::sqrt(Fsumsq1 - mean1*mean1);
        if (Fsumsq2 - mean2*mean2 > 0.)
            norm2 = std::sqrt(Fsumsq2 - mean2*mean2);
        if (norm1 < 1.e-38) norm1 = 1.e-38;
        if (norm2 < 1.e-38) norm2 = 1.e-38;
        
        dist =  0.;
        distsq = 0.;
        
       {
#pragma omp parallel for schedule(dynamic) private(h1, k1, l1, Fp, sFp, Fm, sFm, Fp2, sFp2, Fm2, sFm2, key )
            for ( i1 = 0; i1 < nreflmap1; i1++)
            {
                itmap = reflmap1_it_vector[i1];
                h1  = (itmap->second).second.get_h();
                k1  = (itmap->second).second.get_k();
                l1  = (itmap->second).second.get_l();
                key = h1-hclow + (1+hchigh-hclow)*(k1-kclow)+ (1+hchigh-hclow)*(1+kchigh-kclow)*(l1-lclow);
                if (reflmap2.count(key) == 0 ) continue;
                else {
                    Fp = (reflmap1[key].second).get_Fplus();
                    Fm = (reflmap1[key].second).get_Fminus();
                    sFp = (reflmap1[key].second).get_sigFplus();
                    sFm = (reflmap1[key].second).get_sigFminus();
                    Fp2 = (reflmap2[key].second).get_Fplus();
                    Fm2 = (reflmap2[key].second).get_Fminus();
                    sFp2 = (reflmap2[key].second).get_sigFplus();
                    sFm2 = (reflmap2[key].second).get_sigFminus();
#pragma omp flush(distsq,norm1,norm2,mean1,mean2)
#pragma omp critical(finalsum)
                    {
                        distsq += ((Fp/sFp-mean1)/norm1-(Fp2/sFp2-mean2)/norm2)*((Fp/sFp-mean1)/norm1-(Fp2/sFp2-mean2)/norm2)
                        +((Fm/sFm-mean1)/norm1-(Fm2/sFm2-mean2)/norm2)*((Fm/sFm-mean1)/norm1-(Fm2/sFm2-mean2)/norm2);
                    }
                }
            }
        }
    }
    
    distsq = ((double)ncommon)/((double)(unique1+unique2-ncommon))*distsq
    +  ((mean1/norm1)*(mean1/norm1)+(mean2/norm2)*(mean2/norm2))/4.
    *  ((double)(unique1+unique2-2*ncommon))/((double)(unique1+unique2-ncommon));
    
    if (ncommon == 0) return 1.;
    
    return std::sqrt(distsq);
}

