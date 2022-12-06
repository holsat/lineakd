/***************************************************************************
                          loader.h  -  description
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

#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>
#include <lineak/definitions.h>

/**Loads lineak related configuration files.
  *@author Sheldon Lee Wen
  */

class Loader {
public: 
	Loader() { file = snull; }
	Loader(string filename);
	virtual ~Loader();
  	/** Set the filename of the configloader */
	virtual bool setFile(const string filename);
	/** Get the name of the file */
	virtual inline const string& getFile() const { return file; }
protected:
	vector<string>* loadFile();
	/** Contains the name of the file to load from */
	string file;	
};

#endif
