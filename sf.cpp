/* sf.cpp - simple class to handle a structure factor version of a reflection */

/*
 * derived from reflection.cpp in
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

/**
 @file sf.cpp
 @date 29/12/2016
 @author Herbert J. Bernstein
 derived from reflection.cpp by Tim Gruene
 several functions used to read in structure factor data from an MTZ-file
 */


#include "sf.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <ccp4/csymlib.h>

// default constructor
SF::SF(){}

//! initialise reflection
SF::SF(int h, int k, int l,
       double fp, double sigfp, double fm, double sigfm)
{
    H         = h;
    K         = k;
    L         = l;
    Fplus     = fp;
    sigFplus  = sigfp;
    Fminus    = fm;
    sigFminus = sigfm;
}

// default destructor
SF::~SF(){}

/**
 turns \f$I_\pm\f$ and their \f$\sigma_\pm\f$ to intensities.
 The function takes the square root of \f$I_+\f$ and \f$I_-\f$
 to get \f$F_+\f$ and \f$F_-\f$ respectively,
 \f$\sigma\f$ values are calulcated according to
 \f$\sigma_F = \fsqrt(I+\sigma_I} - F\f$
 */
void SF::Ipm2Fpm()
{
    
    if (sigFplus > 0)
    {
        double I_p    = Fplus;
        double sigI_p = sigFplus;
        
        if (I_p > 0.) {
            Fplus = std::sqrt(I_p);
        } else {
            Fplus = std::sqrt(-I_p);
            sigI_p -= 2*I_p;
        }
        if (I_p + sigI_p > 0.) {
            sigFplus = std::sqrt(I_p + sigI_p) - Fplus;
        } else {
            sigFplus = 0.;
        }
    }
    if (sigFminus > 0)
    {
        double I_m    = Fminus;
        double sigI_m = sigFminus;
        
        if (I_m > 0.) {
            Fminus = std::sqrt(I_m);
        } else {
            Fminus = std::sqrt(-I_m);
            sigI_m -= 2*I_m;
        }
        if (I_m + sigI_m > 0.) {
            sigFminus = std::sqrt(I_m + sigI_m) - Fminus;
        } else {
            sigFminus = 0.;
        }
    }
}

/**
 turns \f$F\f$, \f$\Delta_{ano}\f$ and their \f$\sigma's\f$ to pm intensities.
 Calculates \f$F_+ = F + \frac{1}{2}\Delta_{ano}\f$ and \f$F_- = F -
 \frac{1}{2}\Delta_{ano}\f$
 */
void SF::dF2Fpm()
{
    double F       = Fplus;
    double Dano    = Fminus;
    double sigF    = sigFplus;
    double sigDano = sigFminus;
    if ( sigFplus >  0)
    {
        Fplus     = (   F + 0.5 * Dano );
        sigFplus  = (sigF + 0.5 * sigDano );
    }
    if ( sigFminus > 0 )
    {
        Fminus    = (    F - 0.5 * Dano);
        sigFminus = ( sigF + 0.5 * sigDano);
    }
}

/**
 formatted printing of h, k, l, and data;
 set static so that it can be used with for_each for vectors of
 reflections.
 \param SFF structure factor to be printed
 */
void SF::print(SF SFF)
{
    std::cout << std::fixed
    << std::setw(4) << SFF.H
    << std::setw(4) << SFF.K
    << std::setw(4) << SFF.L
    << std::setw(10) << std::setprecision(3) << SFF.Fplus
    << std::setw(10) << std::setprecision(3) << SFF.sigFplus
    << std::setw(10) << std::setprecision(3) << SFF.Fminus
    << std::setw(10) << std::setprecision(3) << SFF.sigFminus
    << '\n';
    
}

/**
 returns true if reflection indices are pairwise identical
 @c R comparison reflection
 */
bool SF::operator==(SF SFF)
{
    return  ( SFF.H == H && SFF.K == K && SFF.L == L );
}


/**
 returns @c factor * (@c H, @c K, @c L)
 @c factor factor of product
 */
SF SF::operator*(int factor)
{
    SF reflex = SF(*this);
    
    reflex.H = factor * H;
    reflex.K = factor * K;
    reflex.L = factor * L;
    
    return reflex;
}


/**
 returns @c with all data (\f$F_\pm, \sigma_\pm\f$) multiplied by @c k
 */
SF SF::operator*(double k)
{
    SF result = SF((*this));
    result.Fplus     *= k;
    result.sigFplus  *= k;
    result.Fminus    *= k;
    result.sigFminus *= k;
    
    return result;
}

/**
 *  multiply a reflection with the 3x3 matrix of a reciprocal space symmetry
 *  operator
 *  @c rsymop symmetry operator
 *  \todo as of 3/6/2004 I noticed that my RSymOps are not the transposed of
 *  the real space operators, as they should be.  Therefore, the following
 *  method swaps row and colums of the symmetry operator until this is fixed.
 */
SF SF::operator*(RSymOp *rsymop)
{
    SF reflex = SF(*this);
    
    reflex.H = rsymop->TR[0][0] * H + rsymop->TR[0][1] * K + rsymop->TR[0][2] * L;
    reflex.K = rsymop->TR[1][0] * H + rsymop->TR[1][1] * K + rsymop->TR[1][2] * L;
    reflex.L = rsymop->TR[2][0] * H + rsymop->TR[2][1] * K + rsymop->TR[2][2] * L;
    
    return reflex;
}

