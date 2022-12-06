/***************************************************************************
                          saver.h  -  description
                             -------------------
    begin                : Sat Feb 8 2003
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

#ifndef SAVER_H
#define SAVER_H
#include <iostream>
#include <string>
#include <lineak/lconfigdata.h>
using namespace std;
/**Top level class responsible for saving configuration and definition data.
  *@author Sheldon Lee Wen
  */

class Saver {
public: 
	Saver();
	Saver (string filename);
	virtual ~Saver();
  	/** Write property of string file. */
  	virtual void setFile( const string& _newVal);
  	/** Read property of string file. */
  	virtual inline const string& getFile() { return file; }
  	/** Save a config file */
  	virtual bool saveFile(LConfigData &data);
protected: // Protected attributes
  	/** Contains the name of the file to write to. */
  	string file;
};

Saver & operator<<(Saver &save, LConfigData &rhs);

#endif
