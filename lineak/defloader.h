/***************************************************************************
                          defloader.h  -  description
                             -------------------
    begin                : Sun Jan 26 2003
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

#ifndef DEFLOADER_H
#   define DEFLOADER_H

#   ifdef HAVE_CONFIG_H
#      include <config.h>
#   endif

#   include <lineak/loader.h>
#   include <cstdlib>
#   include <map>

class LDef;
class LKbd;

/**Loads keyboard definition files.
  *@author Sheldon Lee Wen
  */

class DefLoader:public Loader
{
 public:
   DefLoader ();
   DefLoader (string filename);
   virtual ~ DefLoader ();

   /** Load a keyboard definition file */
   LDef loadDef ();
   /** Load the config for the file filename */
   LDef loadDef (string filename);
 private:
   vector < string > *processMulti (vector < string > *rawData);
   /** Return a keyboard definition object. */
   map < string, LKbd * >*getDefObj (vector < string > *results);

};
LDef & operator>> (DefLoader & ldr, LDef & ldef);
#endif
