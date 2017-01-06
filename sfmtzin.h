/* sfmtzin.h - read an mtz-file */

/*
 * derived from mtzin.h in
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
   @file sfmtzin.h
   @date 29/12/2016
   @author Herbert J. Bernstein
   derived from mtzib.h by Tim Gruene
   several functions used to read in structure factor data from an MTZ-file
*/

#ifndef SFMTZIN_H
#define SFMTZIN_H

#include <ccp4/cmtzlib.h>

#include <string>
#include <vector>

#include "defines.h"
#include "label.h"
#include "sf.h"

bool sfguess_labels   	    (CMtz::MTZ *, std::vector<Label> &);  // find labels in mtz-data
bool sfguess_sigma    		(CMtz::MTZ *, std::vector<Label> &,
			    				std::string search_type, std::string parent); 
bool sffind_labels    		(CMtz::MTZ *, std::string[4], std::vector<Label>&);
void sfget_data       		(CMtz::MTZ *, std::vector<Label> &,
							 std::vector<SF>&,
		     				 bool  			= false);
void sfscale_data     		(std::vector<SF> &);

#endif // SFMTZIN_H
