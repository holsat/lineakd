/***************************************************************************
                          lconfigdata.cpp  -  description
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

#include <lineak/lconfigdata.h>

LConfigData::LConfigData(){
}
LConfigData::~LConfigData(){
}
ostream & operator<<(ostream &out, LConfigData &rhs) {
	rhs.print(out);
	return out;
}
