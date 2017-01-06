/* rsymop.h - simple struct for symmetry operator in reciprocal space */

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
 * \file rsymop.h
 * \date 9/6/2004
 * \author Tim Gruene
 */

#ifndef R_SYMOP_H
#define R_SYMOP_H

#include <iostream>
#include <iomanip>

//! a simple class of symmetry operators in reciprocal space
class RSymOp
{
public:
	int TR [3][3];  ///< symmetry operator as 3x3 Matrix
public:
	RSymOp () {};
	RSymOp (int [3][3]);
	RSymOp (const RSymOp &);
	~RSymOp() {};

};

#endif // RSYMOP_H
