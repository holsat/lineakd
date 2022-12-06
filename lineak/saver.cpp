/***************************************************************************
                          saver.cpp  -  description
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

#include <lineak/saver.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <lineak/definitions.h>

using namespace std;

Saver::Saver(){
	file = snull;
}
Saver::Saver(string filename){
	file = filename;
}
Saver::~Saver(){
}
/** Attempt to open a file and put all of the data in the file */
bool Saver::saveFile(LConfigData &data) {
    // If our file is snull return false.
	if ( file == snull ) {
        cerr << "File " << file << " to save to is invalid. Please set a valid filename with setFile(filename)" << endl;
		return false;
	}
    ofstream ofout;
    ofout.open(file.c_str(), ios::out | ios::trunc );
    if (!ofout) {
    	cerr << endl << "Failed to open file" << file << " for a save operation" << endl;
     	ofout.clear();
     	return false;
    }
//    out.clear();
    ofout << data << endl;

    if (!ofout)
    	cerr << endl << "Failed to save configuration to file " << file << endl;
     
    ofout.close();
    ofout.clear();
	return true;
}

/** Write property of string file. */
void Saver::setFile( const string& filename){
		file = filename;
}
Saver & operator<<(Saver &save, LConfigData &rhs) {
	save.saveFile(rhs);
	return save;
}

