/* utils.h - collection of utilities */

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
   @section utils
   @file utils.h 
   @date 28/03/2004
   @author: Tim Gruene
   some functions mostly used for debugginh and printing of various
   labels
   
*/

#ifndef UTILS_H
#define UTILS_H          1

#include <ccp4/cmtzlib.h>
#include <string>
#include "defines.h"

namespace Utils {
  void        mtz_all_info     		(CMtz::MTZ *);
  void        print_labels     		(CMtz::MTZ *);
  std::string scaSGname        		(std::string str);
  std::string tolower          		(std::string str);
  std::string spacegroupname   		(int sgnumber);
  int         spacegroupnumber 		(std::string sgname);
}



#endif // UTILS_H
