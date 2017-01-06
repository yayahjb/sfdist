/* label.h handle data labels from mtz-file */

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
    \file label.h - provides class Label
    \date: 26/03/2004
    \author: Tim Gruene
    Label contain information about 
    crystal, set, column and the labelname, so that one can easily
    find the appropriate column in the mtz file
*/

#ifndef LABEL_H
#define LABEL_H

#include <string>

class Label
{
 private:
  int xtal, set, column;
  int absolute_column;   // absolute column in mtz-data
  std::string label;
  std::string type;
  
 public:
  Label();
  Label(int, int, int, std::string, std::string, int);
  ~Label();
  int get_xtal()            { return xtal; }
  int get_set()             { return set;  }
  int get_column()          { return column; }
  int get_absolute_column() { return absolute_column;}
  std::string get_label()   { return label; }
  std::string get_type()    { return type;  }
  static void print(Label);
};

#endif //LABEL_H
