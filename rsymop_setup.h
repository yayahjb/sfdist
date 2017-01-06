/* symop_setup.h - collection of functions to set up symmetry operators in reciprocal space */

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
 * \file symop_setup.h
 * \date 9/6/2004
 * \author Tim Gruene
 * description: this file provides functions to parse the list of symmetry
 * operators provided in syminfo.h
 */

#ifndef RSYMOP_SETUP_H
#define RSYMOP_SETUP_H

#include "rsymop.h"

#include <ccp4/cmtzlib.h>

#include <string>
#include <vector>

//! namespace for symmetry operators /-tions in reciprocal space
namespace RSym {
	void make_list  ( std::vector<RSymOp*> &ops, const CMtz::MTZ* mtz);
} // end of namespace RSym

#endif // RSYMOP_SETUP_H
