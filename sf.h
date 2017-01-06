/* sf.h - simple class to handle a structure factor version of a reflection */

/*
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
 *  \file sf.h
 *  \class SF
 *  \date 29/12/2016
 *  \author Herbert J. Bernstein
 *  derived from reflection.h by Tim Gruene
 *  provides a simple class of reflections
 *  a reflection carries the information 
 *  @c H @c K @c L 
 *  and four double types for amplitude for +H and
 *  -H respectively, and their sigmas.
 *  
 *  Centric Reflections are marked by @c Fminus == @c sigFminus = -1.0
 *  whereas missing data are marked by F = 0 and sigmaF = -1.0
 */
#ifndef SF_H
#define SF_H

#include "rsymop.h"

class SF
{
 private:
  int H, K, L;
  double Iplus, sigIplus, Iminus, sigIminus;
    double Fplus, sigFplus, Fminus, sigFminus;
  bool Ipm, Fpm, dF;
  //TODO need a way to distinguish between I, F, and F/Dano
 public:
  SF();
  SF ( int, int, int, double, double, double, double);
  ~SF();
  int get_h() const          { return H;}           ///< return @H
  int get_k() const          { return K;}           ///< return @K
  int get_l() const          { return L;}           ///< return @L
  double get_Fplus() const    { return Fplus;}      ///< return \f$I_+\f$
  double get_sigFplus() const { return sigFplus;}   ///< return \f$\sigma_+\f$
  double get_Fminus() const   { return Fminus;}     ///< return \f$I_-\f$
  double get_sigFminus() const { return sigFminus;} ///< return \f$\sigma_-\f$
  //! turn intensities to amplitudes; modifies the calling structure factor!
  void Ipm2Fpm();
  //! turn intensities to amplitudes; modifies the calling structure factor!
  void dF2Fpm();
  //! formatted printing of structure factor @c SFF
  static void print (SF SFF);
  //! comparison operator 
  bool operator ==(SF SFF);
  //! multiply (H,K,L) with an integer
  SF operator*(int factor);
  //! multiply intensities and sigmas with double
  SF operator* (double k);
  //! apply reciprocal space symmetry operator
  SF operator*(RSymOp*);
  
  //! comparison by Iplus
  friend bool lesserIplus(const SF& SFF1, const SF& SFF2)
  											{ return SFF1.Fplus < SFF2.Fplus; }
  
  //! comparison by Iminus
  friend bool lesserIminus(const SF& SFF1, const SF& SFF2)
  											{ return SFF1.Iminus < SFF2.Iminus; }
    
};

bool lesserIplus (const SF& SFF1, const SF& SFF2);
bool lesserIminus(const SF& SFF1, const SF& SFF2);
bool version01Sorting(const SF& SFF1, const SF& SFF2);
#endif //SF_H
