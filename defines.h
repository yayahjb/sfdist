/* defines.h - Definitions used my several program modules */

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
 * \file defines.h
 * this file contains global constants used by mtz2sca
 */

#ifndef MY_DEFINES_H
#define MY_DEFINES_H

namespace Defs 
{
  char const VERSION[]  = "0.4";
  extern bool VERBOSE;
  
  //! help option was given to program, exit cleanly
  int const HELP_OPTION      =  0; 
  //! an option misses an argument
  int const MISSING_OPTION   = -1;
  //! could not guess labels - need user assistance
  int const NEED_LABELS      = -2;
  //! error with input labels or label root
  int const ERROR_LABELS     = -3;
  //! invalid column type
  int const ERROR_COLTYPE    = -4;
  //! invalid label combination
  int const LABEL_COMBIN     = -5;   
  //! unable to open a file
  int const ERROR_FILE_OPEN  = -11;
  //! unable to write to a file
  int const ERROR_WRITE_FILE = -12;
  //! program not yet set up to handle more than 2 xtals in mtz-file
  int const NUM_XTALS        = -21;
  //! something needs to be done
  int const TODO             = -31;
  int const BUG              = -32;  // e.g. program reached a stage where
				     // it should not come

  //! Indices for label array
  /** 
      @c p, @c m : Entries for \f$I_\pm\f$, \f$F_pm\f$, or \f$F,
      \Delta_{ano}\f$ (depending on column type)
      @c P, @c M : Entries for \f$\sigma_\pm\f$
      @c r : Root of labelname
      @c NUM_LABELS: counter to create arrays
  */
  enum LABELS {p, P, m, M, r, NUM_LABELS};

  unsigned char const BIT_P        = (1 << p);
  unsigned char const BIT_SIGP     = (1 << P);
  unsigned char const BIT_M        = (1 << m);
  unsigned char const BIT_SIGM     = (1 << M);
  unsigned char const BIT_R        = (1 << r);
  /** list of spacegroups; place corresponds to number according to \ref ITA "IT A".
      Generated with gawk from sitcom.sym (permission to be granted);
  */
  const std::string SGLIST [] = { "0",          // dummy so that entry matches number
				  "P1",  	//   1
				  "P-1",  	//   2
				  "P2",  	//   3
				  "P2(1)",  	//   4
				  "C2",  	//   5
				  "Pm",  	//   6
				  "Pc",  	//   7
				  "Cm",  	//   8
				  "Cc",  	//   9
				  "P2/m",  	//   10
				  "P2(1)/m",  	//   11
				  "C2/m",  	//   12
				  "P2/c",  	//   13
				  "P2(1)/c",  	//   14
				  "C2/c",  	//   15
				  "P222",  	//   16
				  "P222(1)",  	//   17
				  "P2(1)2(1)2",  //   18
				  "P2(1)2(1)2(1)",  //   19
				  "C222(1)",  	//   20
				  "C222",  	//   21
				  "F222",  	//   22
				  "I222",  	//   23
				  "I2(1)2(1)2(1)", //   24
				  "Pmm2",  	//   25
				  "Pmc2(1)",  	//   26
				  "Pcc2",  	//   27
				  "Pma2",  	//   28
				  "Pca2(1)",  	//   29
				  "Pnc2",  	//   30
				  "Pmn2(1)",  	//   31
				  "Pba2",  	//   32
				  "Pna2(1)",  	//   33
				  "Pnn2",  	//   34
				  "Cmm2",  	//   35
				  "Cmc2(1)",  	//   36
				  "Ccc2",  	//   37
				  "Amm2",  	//   38
				  "Abm2",  	//   39
				  "Ama2",  	//   40
				  "Aba2",  	//   41
				  "Fmm2",  	//   42
				  "Fdd2",  	//   43
				  "Imm2",  	//   44
				  "Iba2",  	//   45
				  "Ima2",  	//   46
				  "Pmmm",  	//   47
				  "Pnnn",  	//   48
				  "Pccm",  	//   49
				  "Pban",  	//   50
				  "Pmma",  	//   51
				  "Pnna",  	//   52
				  "Pmna",  	//   53
				  "Pcca",  	//   54
				  "Pbam",  	//   55
				  "Pccn",  	//   56
				  "Pbcm",  	//   57
				  "Pnnm",  	//   58
				  "Pmmn",  	//   59
				  "Pbcn",  	//   60
				  "Pbca",  	//   61
				  "Pnma",  	//   62
				  "Cmcm",  	//   63
				  "Cmca",  	//   64
				  "Cmmm",  	//   65
				  "Cccm",  	//   66
				  "Cmma",  	//   67
				  "Ccca",  	//   68
				  "Fmmm",  	//   69
				  "Fddd",  	//   70
				  "Immm",  	//   71
				  "Ibam",  	//   72
				  "Ibca",  	//   73
				  "Imma",  	//   74
				  "P4",  	//   75
				  "P4(1)",  	//   76
				  "P4(2)",  	//   77
				  "P4(3)",  	//   78
				  "I4",  	//   79
				  "I4(1)",  	//   80
				  "P-4",  	//   81
				  "I-4",  	//   82
				  "P4/m",  	//   83
				  "P4(2)/m",  	//   84
				  "P4/n",  	//   85
				  "P4(2)/n",  	//   86
				  "I4/m",  	//   87
				  "I4(1)/a",  	//   88
				  "P422",  	//   89
				  "P42(1)2",  	//   90
				  "P4(1)22",  	//   91
				  "P4(1)2(1)2", //   92
				  "P4(2)22",  	//   93
				  "P4(2)2(1)2", //   94
				  "P4(3)22",  	//   95
				  "P4(3)2(1)2", //   96
				  "I422",  	//   97
				  "I4(1)22",  	//   98
				  "P4mm",  	//   99
				  "P4bm",  	//   100
				  "P4(2)cm",  	//   101
				  "P4(2)nm",  	//   102
				  "P4cc",  	//   103
				  "P4nc",  	//   104
				  "P4(2)mc",  	//   105
				  "P4(2)bc",  	//   106
				  "I4mm",  	//   107
				  "I4cm",  	//   108
				  "I4(1)md",  	//   109
				  "I4(1)cd",  	//   110
				  "P-42m",  	//   111
				  "P-42c",  	//   112
				  "P-42(1)m",  	//   113
				  "P-42(1)c",  	//   114
				  "P-4m2",  	//   115
				  "P-4c2",  	//   116
				  "P-4b2",  	//   117
				  "P-4n2",  	//   118
				  "I-4m2",  	//   119
				  "I-4c2",  	//   120
				  "I-42m",  	//   121
				  "I-42d",  	//   122
				  "P4/mmm",  	//   123
				  "P4/mcc",  	//   124
				  "P4/nbm",  	//   125
				  "P4/nnc",  	//   126
				  "P4/mbm",  	//   127
				  "P4/mnc",  	//   128
				  "P4/nmm",  	//   129
				  "P4/ncc",  	//   130
				  "P4(2)/mmc",  //   131
				  "P4(2)/mcm",  //   132
				  "P4(2)/nbc",  //   133
				  "P4(2)/nnm",  //   134
				  "P4(2)/mbc",  //   135
				  "P4(2)/mnm",  //   136
				  "P4(2)/nmc",  //   137
				  "P4(2)/ncm",  //   138
				  "I4/mmm",  	//   139
				  "I4/mcm",  	//   140
				  "I4(1)/amd",  //   141
				  "I4(1)/acd",  //   142
				  "P3",  	//   143
				  "P3(1)",  	//   144
				  "P3(2)",  	//   145
				  "R3",  	//   146
				  "P-3",  	//   147
				  "R-3",  	//   148
				  "P312",  	//   149
				  "P321",  	//   150
				  "P3(1)12",  	//   151
				  "P3(1)21",  	//   152
				  "P3(2)12",  	//   153
				  "P3(2)21",  	//   154
				  "R32",  	//   155
				  "P3m1",  	//   156
				  "P31m",  	//   157
				  "P3c1",  	//   158
				  "P31c",  	//   159
				  "R3m",  	//   160
				  "R3c",  	//   161
				  "P-31m",  	//   162
				  "P-31c",  	//   163
				  "P-3m1",  	//   164
				  "P-3c1",  	//   165
				  "R-3m",  	//   166
				  "R-3c",  	//   167
				  "P6",  	//   168
				  "P6(1)",  	//   169
				  "P6(5)",  	//   170
				  "P6(2)",  	//   171
				  "P6(4)",  	//   172
				  "P6(3)",  	//   173
				  "P-6",  	//   174
				  "P6/m",  	//   175
				  "P6(3)/m",  	//   176
				  "P622",  	//   177
				  "P6(1)22",  	//   178
				  "P6(5)22",  	//   179
				  "P6(2)22",  	//   180
				  "P6(4)22",  	//   181
				  "P6(3)22",  	//   182
				  "P6mm",  	//   183
				  "P6cc",  	//   184
				  "P6(3)cm",  	//   185
				  "P6(3)mc",  	//   186
				  "P-6m2",  	//   187
				  "P-6c2",  	//   188
				  "P-62m",  	//   189
				  "P-62c",  	//   190
				  "P6/mmm",  	//   191
				  "P6/mcc",  	//   192
				  "P6(3)/mcm",  //   193
				  "P6(3)/mmc",  //   194
				  "P23",  	//   195
				  "F23",  	//   196
				  "I23",  	//   197
				  "P2(1)3",  	//   198
				  "I2(1)3",  	//   199
				  "Pm-3",  	//   200
				  "Pn-3",  	//   201
				  "Fm-3",  	//   202
				  "Fd-3",  	//   203
				  "Im-3",  	//   204
				  "Pa-3",  	//   205
				  "Ia-3",  	//   206
				  "P432",  	//   207
				  "P4(2)32",  	//   208
				  "F432",  	//   209
				  "F4(1)32",  	//   210
				  "I432",  	//   211
				  "P4(3)32",  	//   212
				  "P4(1)32",  	//   213
				  "I4(1)32",  	//   214
				  "P-43m",  	//   215
				  "F-43m",  	//   216
				  "I-43m",  	//   217
				  "P-43n",  	//   218
				  "F-43c",  	//   219
				  "I-43d",  	//   220
				  "Pm-3m",  	//   221
				  "Pn-3n",  	//   222
				  "Pm-3n",  	//   223
				  "Pn-3m",  	//   224
				  "Fm-3m",  	//   225
				  "Fm-3c",  	//   226
				  "Fd-3m",  	//   227
				  "Fd-3c",  	//   228
				  "Im-3m",  	//   229
				  "Ia-3d"  	//   230
  };

}  // end of namespace Defs
#endif // MY_DEFINES_H
