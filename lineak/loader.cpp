/***************************************************************************
                          loader.cpp  -  description
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

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/loader.h>

using namespace lineak_util_functions;
extern bool verbose;

Loader::Loader(string filename) : file(filename) {
}
Loader::~Loader(){
}
/** Attempt to open a file, and put all of it's lines in a vector
	Return a snull pointer if unsuccessful */
vector<string>* Loader::loadFile() {
      char buffer[500] = {0};
      string tmpStore = snull;
      string seperators = " \n\t";
      vector<string> *tmpResults;

      ifstream in(file.c_str());
      if (!in.is_open()) {
      	cerr << endl << "Failed to open file " << file << " for a load operation" << endl;;
       	in.clear();
        in.close(); //for good measure.
      	return (new vector<string>);
      }

      tmpResults = new vector<string>;
      string::size_type index = 0;
     	while(!in.eof()) {
        	in.getline(buffer, 500, '\n');
         	tmpStore = string(buffer);
          // If there is a '#' in the line somewhere...... start looking at the end of the line.
          if(tmpStore.rfind('#') != string::npos) {
		// Look for the right most quotation.
		//qindex = tmpStore.rfind('"');
          	index = tmpStore.rfind('#');
            /** If the # is the first thing on the line, save ourselves the time and just continue */
            if (index == 0)
            	continue;
            /** Determine if there is a comment at the end of the line, or an escaped '#'
                that is part of the command. Loop through all of the characters in the string
                from the end to the beginning (in reverse) */
            while (index != 0) {
            	/** If the previous character is not an escaped '#' */
              if (index > 0 &&  tmpStore[index -1] != '\\') {
//		      cout << "NOT ESCAPED #" << endl;
		// Now there are two possiblilities. 1 is that the command portion of the directive
		// is enclosed in quotes. If so, we should ignore this #.
		// Get the right most "
		string::size_type qindex = tmpStore.rfind('"');
		// Get the position of the =
		string::size_type eindex = tmpStore.find('=');
		// Get the position of the first " after the =
		string::size_type bindex = tmpStore.find('"', eindex+1);
                // If the position of the # is after the first " and before the last "
//		cout << "qindex = " << qindex << endl;
//		cout << "eindex = " << eindex << endl;
//		cout << "bindex = " << bindex << endl;
  //              cout << "index = " << index << endl;

		if (index > bindex && index < qindex) {
	           // Index is the position of the last "
                   index = tmpStore.rfind('#',index-1);
		}
		else
              	   // get out of this loop
                   break;
	      }
              /** Else, we have a '\#' sequence. i.e. the '#' has been escaped by the user */
              else {
              	/** The previous character is an escape and the index
                    is at least one. Which means that at least we have '\#' on the line. */
                if (index > 0)
                	/* index now becomes the index of any remaining '#' character in the string */
                	index = tmpStore.rfind('#',index-1);
                  // We shouldn't get here.
                else {
                	cerr << "Shouldn't be here" << endl;
                  break;
                }
              }
            } // End of while loop
            /** Get the portion of the line, from the beginning, up to the comment */
            tmpStore = tmpStore.substr(0, index);
          } // End of if(tmpStore.rfind('#') != string::npos)
          if (tmpStore.length()<=0 || tmpStore.find_first_not_of(seperators) == string::npos) continue;
					// tmp store now has a line of input from the config file.
					//if (verbose) cout << "Adding line: " << tmpStore << endl;
					tmpResults->push_back(tmpStore);
      }
      in.close();
      in.clear();
			return (tmpResults);
}
/** Set the filename of the configloader */
bool Loader::setFile(const string filename){
	if ( file_exists(filename)) {
		file = filename;
		return true;
	}
	return false;
}
