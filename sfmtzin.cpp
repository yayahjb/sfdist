/* sfmtzin.cpp - read an mtz-file for structure factors */

/*
 * derived from mtzin.cpp in
 *
 * mtz2sca - convert mtz-file with anomalous data to sca-format
 * Copyright (C) 2004 Tim Gruene
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "sfmtzin.h"

#include <ccp4/csymlib.h>
#include <ccp4/cmtzlib.h>
#include <iostream>
#include <limits>
#include <algorithm>
#include <functional>
#include <cmath>

//! guess the correct labels of input @c mtz and store them in @c labels
/**
 sfguess_labels - find appropriate labels in mtz
 priority:
 \li try to find F(+) and F(-) -> column type G,
 \f$\sigma\f$'s:              L
 \li try to find \<F\> and \f$\Delta_{ano}\f$  -> column types F and D, respectively
 \f$\sigma\f$'s:              Q
 \li try to find I(+) and I(-) -> column type K,
 \f$\sigma\f$'s:              M
 \param mtz the mtz data
 \param labels  Label vector
 */
bool sfguess_labels (CMtz::MTZ *mtz, std::vector<Label> &labels)
{
    int x, s, c;
    int abscol;
    Label newlabel;
    std::string collabel, coltype, next_coltype;
    std::string label_string;   // to control which combination of labels was found
    
    // Try 3 cases of anomalous data
    // there are probably more elegant ways than to repeat this block three
    // times to search for  G, F/D and K columns in mtz, but since this is
    // not a big speed issue, I simply repleat what I've already typed
    
    /* step 1: GLGL (F(+)sigF(+)F(-)sigF(-))
     try to find column type G in dataset ((+/-)-amplitudes) */
    {
        label_string = "";
        labels.erase(labels.begin(), labels.end());
        for ( x = 0; x < mtz->nxtal; x++)
        {
            for ( s = 0; s < mtz->xtal[x]->nset; s++)
            {
                for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++)
                {
                    coltype = std::string(mtz->xtal[x]->set[s]->col[c]->type);
                    if ( coltype.find("G") != std::string::npos )
                    {
                        label_string += "G";
                        collabel = mtz->xtal[x]->set[s]->col[c]->label;
                        abscol   = mtz->xtal[x]->set[s]->col[c]->source;
                        newlabel = Label(x,s,c, collabel, coltype, abscol);
                        labels.push_back(newlabel);
                        next_coltype = std::string(mtz->xtal[x]->set[s]->col[c+1]->type);
                        // We assume that the sigma follows directly to this
                        // column type. Otherwise
                        // we will try to find a label with the same name and prefix "SIG"
                        // if this fails, give up guessing.
                        if (next_coltype.find("L") != std::string::npos)
                        {
                            label_string += "L";
                            collabel = mtz->xtal[x]->set[s]->col[c+1]->label;
                            abscol   = mtz->xtal[x]->set[s]->col[c+1]->source;
#ifdef DEBUG
                            std::cerr << "label: " << collabel << std::endl;
#endif
                            newlabel = Label (x, s, c+1, collabel, next_coltype, abscol);
                            labels.push_back(newlabel);
                            
                        }
                        else
                        {
                            if ( sfguess_sigma(mtz, labels, "L", collabel))
                            {
                                label_string += "L";
                            }
                            else
                            {
                                std::clog << "Could not find standard deviation for column "
                                << collabel
                                << "\nPlease re-run program with explicit label names.\n";
                                return (false);
                            }
                        }
                    }
                }
            }
        }
    }
    if ( label_string == "GLGL")
    {
#ifdef DEBUG
        std::clog << "Found anomalous amplitudes " << label_string  << std::endl;
#endif
        return (true);
    }
    
    /* step 2: FQDQ (F(avg)sigF(avg)dF(+/-)sigdF(+/-))
     try to find column types F and Q in dataset (avg amplitudes and +/- delta)
     If not delta, accept as non-anomalous*/
    else {
        label_string = "";
        labels.erase(labels.begin(), labels.end());
        for ( x = 0; x < mtz->nxtal; x++) {
            for ( s = 0; s < mtz->xtal[x]->nset; s++) {
                for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++) {
                    coltype = std::string(mtz->xtal[x]->set[s]->col[c]->type);
                    if ( ( coltype.find("F") != std::string::npos )||
                        ( coltype.find("D") != std::string::npos )) {
                        label_string += coltype;
                        collabel = mtz->xtal[x]->set[s]->col[c]->label;
                        abscol   = mtz->xtal[x]->set[s]->col[c]->source;
                        newlabel = Label(x,s,c, collabel, coltype, abscol);
                        labels.push_back(newlabel);
                        next_coltype = std::string(mtz->xtal[x]->set[s]->col[c+1]->type);
                        // We assume that the sigma follows directly to this column type. Otherwise
                        // we will try to find a label with the same name and prefix "SIG"
                        // if this fails, give up guessing.
                        if (next_coltype.find("Q") != std::string::npos) {
                            label_string += "Q";
                            collabel = mtz->xtal[x]->set[s]->col[c+1]->label;
                            abscol   = mtz->xtal[x]->set[s]->col[c+1]->source;
#ifdef DEBUG
                            std::cerr << "label: " << collabel << std::endl;
#endif
                            newlabel = Label (x, s, c+1, collabel, next_coltype, abscol);
                            labels.push_back(newlabel);
                        }
                        else {
                            // look for same label with 'SIG' prepended
                            if ( sfguess_sigma(mtz, labels, "Q", collabel)) {
                                label_string += "Q";
                            }
                            else {
                                std::clog << "Could not find standard deviation for column "
                                << collabel
                                << "\nPlease re-run program with explicit label names.\n";
                                return (false);
                            }
                        }
                    }
                }
            }
        }
    }
    if ( label_string == "FQDQ" || label_string == "FQ")
    {
#ifdef DEBUG
        std::clog << "Found intensities " << label_string  << std::endl;
#endif
        return (true);
    }
    
    /* step 3: KMKM (I(+)sigI(+)I(-)sigI(-))
     try to find column type K in dataset ((+/-)-intensities) */
    else {
        label_string = "";
        labels.erase(labels.begin(), labels.end());
        
        for ( x = 0; x < mtz->nxtal; x++)
        {
            for ( s = 0; s < mtz->xtal[x]->nset; s++)
            {
                for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++)
                {
                    coltype = std::string(mtz->xtal[x]->set[s]->col[c]->type);
                    if ( coltype.find("K") != std::string::npos )
                    {
                        label_string += "K";
                        collabel = mtz->xtal[x]->set[s]->col[c]->label;
                        abscol   = mtz->xtal[x]->set[s]->col[c]->source;
                        newlabel = Label(x,s,c, collabel, coltype, abscol);
                        labels.push_back(newlabel);
                        next_coltype = std::string(mtz->xtal[x]->set[s]->col[c+1]->type);
#ifdef DEBUG
                        std::clog << "Found label type " << coltype << ".\n";
#endif
                        // We assume that the sigma follows directly to this column type. Otherwise
                        // we will try to find a label with the same name and prefix "SIG"
                        // if this fails, give up guessing.
                        if (next_coltype.find("M") != std::string::npos)
                        {
                            label_string += "M";
                            collabel = mtz->xtal[x]->set[s]->col[c+1]->label;
                            abscol   = mtz->xtal[x]->set[s]->col[c+1]->source;
#ifdef DEBUG
                            std::cerr << "label: " << collabel << std::endl;
#endif
                            newlabel = Label (x, s, c+1, collabel, next_coltype, abscol);
                            labels.push_back(newlabel);
                        }
                        else
                        {
                            if ( sfguess_sigma(mtz, labels, "M", collabel))
                            {
                                label_string += "M";
                            }
                            else
                            {
                                std::clog << "Could not find standard deviation for column "
                                << collabel
                                << "\nPlease re-run program with explicit label names.\n";
                                return (false);
                            }
                        }
                    }
                }
            }
        }
    }
    if ( label_string == "KMKM")
    {
#ifdef DEBUG
        std::clog << "Found anomalous intensities KMKM" << std::endl;
#endif
        return (true);
    }
    
    // Try 1 remaining case of non-anomalous data, as intensities
    /* step 4: JQ (IsigI)
     try to find column type J in dataset ((intensities) */
    else {
        label_string = "";
        labels.erase(labels.begin(), labels.end());
        
        for ( x = 0; x < mtz->nxtal; x++)
        {
            for ( s = 0; s < mtz->xtal[x]->nset; s++)
            {
                for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++)
                {
                    coltype = std::string(mtz->xtal[x]->set[s]->col[c]->type);
                    if ( coltype.find("J") != std::string::npos )
                    {
                        label_string += "J";
                        collabel = mtz->xtal[x]->set[s]->col[c]->label;
                        abscol   = mtz->xtal[x]->set[s]->col[c]->source;
#ifdef DEBUG
                        std::cerr << "label: " << collabel << std::endl;
#endif
                        newlabel = Label(x,s,c, collabel, coltype, abscol);
                        labels.push_back(newlabel);
                        next_coltype = std::string(mtz->xtal[x]->set[s]->col[c+1]->type);
#ifdef DEBUG
                        std::clog << "Found label type " << coltype << ".\n";
#endif
                        // We assume that the sigma follows directly to this column type. Otherwise
                        // we will try to find a label with the same name and prefix "SIG"
                        // if this fails, give up guessing.
                        if (next_coltype.find("Q") != std::string::npos)
                        {
                            label_string += "Q";
                            collabel = mtz->xtal[x]->set[s]->col[c+1]->label;
                            abscol   = mtz->xtal[x]->set[s]->col[c+1]->source;
#ifdef DEBUG
                            std::cerr << "label: " << collabel << std::endl;
#endif
                            newlabel = Label (x, s, c+1, collabel, next_coltype, abscol);
                            labels.push_back(newlabel);
                            break;
                        }
                        else
                        {
                            if ( sfguess_sigma(mtz, labels, "Q", collabel))
                            {
                                label_string += "Q";
                                break;
                            }
                            else
                            {
                                std::clog << "Could not find standard deviation for column "
                                << collabel
                                << "\nPlease re-run program with explicit label names.\n";
                                return (false);
                            }
                        }
                    }
                }
            }
        }
    }
    if ( label_string == "JQ")
    {
#ifdef DEBUG
        std::clog << "Found non-anomalous intensities JQ"  << std::endl;
#endif
        return (true);
    }
    
    
    else   // no labels at all found
    {
        return (false);
    }
    
}
//! find sigma label in mtz struct
/**
 sfguess_sigma - find a label with name "SIG" + @c parent
 returns true, if found, false, otherwise
 @c mtz mtz data to search, @c labels vector of labels where to add the
 new label
 @c search_label string describing column type to look for
 @c parent name of the parent label
 */
bool sfguess_sigma(CMtz::MTZ *mtz, std::vector<Label> &labels,
                   std::string search_type, std::string parent)
{
    int x, s, c;
    int abscol;
    Label newlabel;
    std::string templabel;
    std::string searchlabel;
    std::string coltype;
    std::string collabel;
    
    searchlabel = "SIG"+parent;
    
    for ( x = 0; x < mtz->nxtal; x++)
    {
        for ( s = 0; s < mtz->xtal[x]->nset; s++)
        {
            for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++)
            {
                templabel = mtz->xtal[x]->set[s]->col[c]->label;
                if ( templabel == searchlabel )
                {
                    coltype  = mtz->xtal[x]->set[s]->col[c]->type;
                    collabel = mtz->xtal[x]->set[s]->col[c]->label;
                    abscol   = mtz->xtal[x]->set[s]->col[c]->source;
                    newlabel = Label(x,s,c, templabel, coltype, abscol);
                    if (coltype != search_type)
                    {
                        std::cerr << "Incorrect label type. The label "
                        << templabel << " is of type " << coltype << '\n'
                        << "and not " << search_type << ".\n"
                        << "Please enter column types.\n";
                        return (false);
                    }
                    else
                    {
#ifdef DEBUG
                        std::cerr << "label: " << collabel << std::endl;
#endif
                        labels.push_back(newlabel);
                        return (true);
                    }
                }
            }
        }
    }
    // if we got here, the SIGlabel could not be found
    return (false);
}

//! find @c labels in @c mtz
/**
 find columns with labels given in @c labels[4] and store them as Label's
 in @c columns.
 \param mtz data
 \param to_find the four strings to look for. Should be in correct
 order
 \param labels storage place for labels
 return true iff all four labels were found, otherwise false;
 
 */
bool sffind_labels(CMtz::MTZ *mtz, std::string to_find[4], std::vector<Label>& labels)
{
    int i,x,s,c;
    int abscol;
    Label newlabel;
    std::string templabel;
    std::string searchlabel;
    std::string coltype;
    for ( i = 0; i < 4; i++) {
        for ( x = 0; x < mtz->nxtal; x++) {
            for ( s = 0; s < mtz->xtal[x]->nset; s++) {
                for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++) {
                    templabel = mtz->xtal[x]->set[s]->col[c]->label;
                    if ( templabel == to_find[i] ) {
                        coltype  = mtz->xtal[x]->set[s]->col[c]->type;
                        abscol   = mtz->xtal[x]->set[s]->col[c]->source;
                        newlabel = Label(x,s,c, templabel, coltype, abscol);
                        labels.push_back(newlabel);
                    }
                    else { // label not found
                        // issue an error here!
                    }
                }
            }
        }
    }
    
    if ( labels.size() != 4)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//! read data from @c mtz and store columns selected by @c data_columnes
/**
 use CMtz::ccp4_lrrefl to get all columns and select the ones given
 by @c data_columns afterwards. @c data_columns should contain four entries,
 unless we are reading F's with no deltas, in which case we force
 delta to 0.
 
 @c verbose indicates whether or not some extra information is written to
 stdout.
 */
void sfget_data (CMtz::MTZ *mtz,
                 std::vector<Label> &data_columns,
                 std::vector<SF> &target,
                 bool verbose)
{
    
    if ( data_columns.size() != 4 && data_columns.size() !=2 )
    {
        std::cerr << "Parameter data_columns should contain exactly two or four entries.\n"
        << "There are " << data_columns.size() << " columns. Exiting\n";
        exit (Defs::BUG);
#ifdef DEBUG
    } else {
        std::cerr << "sfget_data processing " << data_columns.size() << "columns" << std::endl;
#endif
    }
    
    int   total_cols;
    int   sel_columns[4];
    std::vector<Label>::iterator itL;
    std::vector<SF>::iterator itR;
    int   h, k, l;
    int   i, r;            // loop variables
    int   num_refl;
    int   *logmss;        // usabilitiy flags returned by ccp4_lrreff
    float resol, *adata;       // data returned by ccp4_lrreff
    SF refl;
    char  coltype;           // Columntype
    int   sg;
    
    total_cols = mtz->ncol_read;
    logmss = new int[total_cols];
    adata  = new float [total_cols];
    
    // get columns from data_columns
    i = 0;
    for ( itL = data_columns.begin(); itL != data_columns.end(); itL++)
    {
        //ccp4 starts counting at 1 instead of 0
        sel_columns[i] = (*itL).get_absolute_column() - 1;
        i++;
    }
    coltype = (*(data_columns.begin())).get_type().at(0);
    
    
    num_refl = mtz->nref_filein;      // total number of input reflections
    sg       = mtz->mtzsymm.spcgrp;
    
#ifdef DEBUG
    {
        std::cout << "Reading data from columns "
        << sel_columns[0] << ' '
        << sel_columns[1];
        if (data_columns.size() == 4)
            std::cout << ' '
            << sel_columns[2] << ' '
            << sel_columns[3];
        std::cout << "\n";
    }
#endif
    
    for ( r = 0; r < num_refl; r++)
    {
        CMtz::ccp4_lrrefl(mtz, &resol, adata, logmss, r+1);
        // only accept structure factor if either F+ or F- is present
        if ( logmss[sel_columns[0]] &&
            (data_columns.size() < 4 || logmss[sel_columns[2]]))
        {
            continue;
        }
        else
        {
            h = int (adata[0]);
            k = int (adata[1]);
            l = int (adata[2]);
            if ( logmss[sel_columns[0]] || adata[sel_columns[1]] <= 0.0 )
                // no data for first data item
            {
                refl = SF ( h, k, l,
                           0.0,
                           -1.0,
                           double (adata[sel_columns[2]]),
                           double (adata[sel_columns[3]])
                           );
            }
            else if ( data_columns.size() < 4  || logmss[sel_columns[2]] || adata[sel_columns[3]] <= 0.0 )
                // no data for second data item
            {
                if (data_columns.size() < 4) {
                    refl = SF ( h, k, l,
                               double (adata[sel_columns[0]]),
                               double (adata[sel_columns[1]]),
                               double (adata[sel_columns[0]]),
                               double (adata[sel_columns[1]])
                               );
                } else {
                    refl = SF ( h, k, l,
                               double (adata[sel_columns[0]]),
                               double (adata[sel_columns[1]]),
                               0.0, -1.0
                               );
                }
            }
            else
            {
                refl = SF ( h, k, l,
                           double (adata[sel_columns[0]]),
                           double (adata[sel_columns[1]]),
                           double (adata[sel_columns[2]]),
                           double (adata[sel_columns[3]])
                           );
            }
            target.push_back(refl);
#ifdef DEBUG
            if (r < 10) SF::print(refl);
#endif
        }
    }
    switch (coltype)
    {
        case 'F':
        {
            r = 0;
#ifdef DEBUG
            std::cerr << "case F" << std::endl;
#endif
            for (itR = target.begin(); itR != target.end(); itR++)
            {
                (*itR).dF2Fpm();
#ifdef DEBUG
                if (r < 10)  SF::print(*itR);
                r++;
#endif
            }
            // scale data to fit to file format
            // sfscale_data (target);
            break;
        }
        case 'G':
        {
#ifdef DEBUG
            std::cerr << "case G" << std::endl;
#endif
            sfscale_data (target);
            break;
            // scale data to fit to file format
            // sfscale_data (target);
            break;
        }
        case 'K': case'J':
        {
            r = 0;
#ifdef DEBUG
            std::cerr << "case K or J" << std::endl;
#endif
            for (itR = target.begin(); itR != target.end(); itR++)
            {
                (*itR).Ipm2Fpm();
#ifdef DEBUG
                if (r < 10)  SF::print(*itR);
#endif
                r++;
            }
            // scale data to fit to file format
            // sfscale_data (target);
            break;
        }
        default:
        {
            // this should not happen
            std::cerr
            << "The CCP4 column type should be either F, G, J or K, but it is "
            << coltype << ".\n";
            exit(Defs::ERROR_COLTYPE);
        }
            
    }
}

//! scale data so the intensities would fit to F8.1 format
/**
 finds the maximum amplitude \f$I_{max}\f$ of @c reflections and scales
 them by scale factor \f$k = 316.23/ F_{max} \f$
 */
void sfscale_data (std::vector<SF> &reflections)
{
    double k;                    // final scaling factor
    double maxF;                 // Structure factor with maximal amplitude
    int r;
    std::vector<SF>::const_iterator  maxPlus, maxMinus;
    std::vector<SF>::iterator it;
    
    maxPlus  = std::max_element(reflections.begin(), reflections.end(), lesserIplus);
    maxMinus = std::max_element(reflections.begin(), reflections.end(), lesserIminus);
    
    maxF = maxPlus->get_Fplus();
    
    if ( maxF < maxMinus->get_Fminus() )
    {
        maxF = maxMinus->get_Fminus();
    }
    
    
    k = 316.23/maxF;
    
    r = 0;
    for ( it = reflections.begin(); it != reflections.end(); it++)
    {
        (*it) = (*it) * k;
#ifdef DEBUG
        if (r < 10)  SF::print(*it);
#endif
        r++;
    }
}

