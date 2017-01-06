/* utils.cpp - collection of utilities */

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

#include "utils.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cctype>

//! mostly for debugging: print all information in mtz-file
void Utils::mtz_all_info(CMtz::MTZ *mtz)
{
  int x, s, c;    // loop variables
  std::cout << "Information in mtz-file:"
	    << "\nHistory:    " << mtz->hist 
	    << "\nhistlines:  " << mtz->histlines
	    << "\nnxtal:      " << mtz->nxtal
	    << "\nncol_read:  " << mtz->ncol_read
	    << "\nnref:       " << mtz->nref
	    << "\nnref_fi:    " << mtz->nref_filein
	    << "\nn_orif_bat: " << mtz->n_orig_bat
	    << "\nresmax:     " << mtz->resmax_out
	    << "\nresmin:     " << mtz->resmin_out
	    << "\nSG:         " << mtz->mtzsymm.spcgrpname
	    << "\n";
  //! get cell information
  for ( int i = 0; i < mtz->nxtal; i++ )
    {
      std::cout << "\nInfo for crystal " << i << ":\n"
		<< "xtalid: "    << mtz->xtal[i]->xtalid << "\n"
		<< " xtal->set " << mtz->xtal[i]->nset << "\n"
		<< "Cell: ";
      for ( int c = 0; c < 6; c++)
	{
	  std::cout << mtz->xtal[i]->cell[c] << ' ';
	}
      std::cout << "\n";
    }
  //! extract column
  for ( x = 0; x < mtz->nxtal; x++)
    {
      for ( s = 0; s < mtz->xtal[x]->nset; s++)
	{
	  std::cout << "Number of labels in crystal " << x 
		    << ", set " << s
		    << ": " << mtz->xtal[x]->set[s]->ncol
		    << "\n";
	  for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++)
	    {
	      std::cout << "Name and type of label: "
			<< std::setw(31) << std::left 
			<< mtz->xtal[x]->set[s]->col[c]->label
			<< '\t' 
			<< std::setw(3) << std::right 
			<< mtz->xtal[x]->set[s]->col[c]->type
			<< '\t' 
			<< std::setw(3) 
			<< std::right << mtz->xtal[x]->set[s]->col[c]->source
			<< '\n';
	    }
	}
    }
}


void Utils::print_labels(CMtz::MTZ *mtz)
{
  int x, s, c;
  std::string coltype;
  std::string collabel;

    for ( x = 0; x < mtz->nxtal; x++)
    {
      for ( s = 0; s < mtz->xtal[x]->nset; s++)
	{
	  for ( c = 0; c < mtz->xtal[x]->set[s]->ncol; c++)
	    {
	      coltype  = std::string(mtz->xtal[x]->set[s]->col[c]->type);
	      collabel = std::string(mtz->xtal[x]->set[s]->col[c]->label);

	      std::cout << "Label: " << std::setw(32) << std::left << collabel
			<< std::right << " Type: " << coltype << "\n";
	    }
	}
    }
}

//! make @c name a valid name for .sca-files
/** turn @c name to lower case and remove any spaces (only real spaces, does not
 * check for other white spaces
 * \paral name name of space group to convert
 */ 
std::string Utils::scaSGname (std::string name)
{
  size_t pos;
  name = Utils::tolower(name);
  
  
  while ( (pos = name.find(" ")) != std::string::npos )
    {
      name.erase(pos,1);
    }
  return name;
}

//! return @c str with lowercase characters
std::string Utils::tolower(std::string str)
{
  std::string lowerstr;
  char c;
  std::string::const_iterator it;

  for ( it = str.begin(); it != str.end(); it++)
    {
      c = std::tolower(*it);
      lowerstr += c ;
    }
  return (lowerstr);
}

//! return string representation of Spacegroup @c sgnumber
std::string Utils::spacegroupname ( int sgnumber )
{
  return Defs::SGLIST[sgnumber];
}

//! return spacegroup number of Spacegroup @c sgname
/**
   return -1, if @c sgname was not found. The string @c sgname must
   correspond to the definition of Defs::SGLIST ( \f defines.h )
   @sgname string representation of spacegroup
*/
int Utils::spacegroupnumber( std::string sgname )
{
  int i;
  
  for ( i = 0; i < 231; i++)
    {
      if ( Defs::SGLIST[i] == sgname )
      	return i;		
    }
      return -1;
}

