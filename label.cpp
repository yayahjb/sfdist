/* label.cpp   handle data labels from mtz-file */

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
#include "label.h"
#include <iostream>
#include <iomanip>

// default constructor
Label::Label ()
{
  xtal = set = column = -1;
  absolute_column     = -1;
}

// constructor from arguments
Label::Label (int x, int s, int c, std::string L, std::string T, int abscol)
{
  xtal            = x;
  set             = s;
  column          = c;
  label           = L;
  type            = T;
  absolute_column = abscol;
}

// default destructor
Label::~Label(){}

void Label::print(Label L)
{
  std::cout << "Label: "  <<  std::setw(32) << std::left << L.label 
	    << std::right << " Type: " << L.type << '\n';
}
