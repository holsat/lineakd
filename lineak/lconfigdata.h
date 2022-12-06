/***************************************************************************
                          lconfigdata.h  -  description
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

#ifndef LCONFIGDATA_H
#define LCONFIGDATA_H

#include <iostream>
#include <lineak/definitions.h>

/**Base class for configuration objects. It just really defines an interface, and the
	*method for printing out an object, via an overloaded << operator.
  *@author Sheldon Lee Wen
  */
using namespace std;
  
class LConfigData {
public: 
	LConfigData();
	virtual ~LConfigData();
	virtual const bool isEmpty() =0;
	virtual void print(void) { print(cout); }
	virtual void print(ostream &out) =0;
};

ostream & operator<<(ostream &out, LConfigData &rhs);

#endif
