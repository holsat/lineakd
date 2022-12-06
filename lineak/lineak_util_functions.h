/***************************************************************************
                          functions.h  -  description
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Sheldon Lee Wen
    email                : leewsb@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/* settings from configure */
#ifdef HAVE_CONFIG_H
#  include "../config.h"
#endif
 
// C++ Headers.
#include <string>
#include <vector>
using namespace std;

namespace lineak_util_functions {
/** Remove a file or directory */
bool remove_file( string name );
/** Check is a file exists */
bool file_exists( string filename );
/** Check is a directory exists */
bool dir_exists( string dirname );
/** Strip the whitespace from a string */
string strip_space ( string store );
/** Remove all characters in chars from a string */
string strip ( string store, string chars );
/** Un-escape quotes and hashes in a string */
string unescape ( const string istring, const string chars );
/** Escape quotes in a string */
string escape ( const string istring, const string chars );
/** Show the contents of a vector */
void inspect_vector(const vector<string>& v);
/** Do a case insensitive string comparison */
int strcmp_nocase(const string & s1, const string & s2);
}

#endif
