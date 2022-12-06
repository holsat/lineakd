/***************************************************************************
                          configdirectives.cpp  -  description
                             -------------------
    begin                : ?  9? 13 2003
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

#include <lineak/configdirectives.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/definitions.h>
#include <iostream>
using namespace std;
using namespace lineak_core_functions;

ConfigDirectives::ConfigDirectives(){
	directives.clear();
}
ConfigDirectives::~ConfigDirectives(){
	directives.clear();
	si_directives.clear();
}
ConfigDirectives::ConfigDirectives(const ConfigDirectives & rhs) {
        if (this != &rhs) {
                directives.clear();
                si_directives.clear();

                map<string,string>::const_iterator it = rhs.directives.begin();
                for (; it != rhs.directives.end(); it++) {
                   directives[it->first] = it->second;
                }
                map<string,int>::const_iterator sit = rhs.si_directives.begin();
                for (; sit != rhs.si_directives.end(); sit++) {
                   si_directives[sit->first] = sit->second;
                }
        }
}
ConfigDirectives & ConfigDirectives::operator=(const ConfigDirectives & rhs) {
        if (this != &rhs) {
                directives.clear();
                si_directives.clear();

                map<string,string>::const_iterator it = rhs.directives.begin();
                for (; it != rhs.directives.end(); it++) {
                   directives[it->first] = it->second;
                }
                map<string,int>::const_iterator sit = rhs.si_directives.begin();
                for (; sit != rhs.si_directives.end(); sit++) {
                   si_directives[sit->first] = sit->second;
                }
        }
        return *this;
}

string ConfigDirectives::getValue(string key) const {
 	string retval;
	map<string,string>::const_iterator it = directives.find(key);
    if ( it == directives.end())
      retval = snull;
    else
	  retval = (it->second);

	return (retval);
}
bool ConfigDirectives::isSet(string key) {
  if ( directives.find(key) != directives.end() ||
 		 si_directives.find(key) != si_directives.end())
	      return (true);

	return (false);
}
//int ConfigDirectives::getValue(int key) {
//	return (int_directives[key]);
//}
int ConfigDirectives::getIntValue(string key) const {
    int retval;
    map<string,int>::const_iterator it = si_directives.find(key);
    if ( it == si_directives.end())
      retval = 0;
    else
	  retval = (it->second);

	return (retval);
}
void ConfigDirectives::addValue(string key, string value) {
	directives[key] = value;
}
//void ConfigDirectives::addValue(int key, int value) {
//	int_directives[key] = value;
//}
void ConfigDirectives::addValue(string key, int value) {
	si_directives[key] = value;
}
string& ConfigDirectives::operator[](string key) {
 return (directives[key]);
}
//int& ConfigDirectives::operator[](int key) {
// return (int_directives[key]);
//}
vector<string> ConfigDirectives::getKeys() {
	map<string,string>::iterator it = directives.begin();
	vector<string> keys;

	for (;it != directives.end(); it++) {
		keys.push_back(it->first);
	}
  return keys;
}
vector<string> ConfigDirectives::getIntKeys() {
	map<string,int>::iterator si_it = si_directives.begin();
	vector<string> keys;

	for (;si_it != si_directives.end(); si_it++) {
		keys.push_back(si_it->first);
	}
  return keys;
}

bool ConfigDirectives::isIntValue(string key) {
	// If si_directives contains the key, return true
	if (si_directives.find(key) != si_directives.end())
		return true;
	// else return false;
	return false;
}
bool ConfigDirectives::isEmpty() {
	return (directives.empty() && si_directives.empty());
}
void ConfigDirectives::removeValue(string key) {
	directives.erase(key);
 	si_directives.erase(key);
}
void ConfigDirectives::print() const {
	this->print(cout);
}
void ConfigDirectives::print(ostream &out) const {
	string value;
	// Print out the string directives first.
       	for (map<string, string>::const_iterator i = directives.begin(); i != directives.end(); i++ ) {
	   //if ( i->second != NULL) {
	      //msg("Assigning value");
              value = i->second;
	      if (value == snull)
	         value = "";
	      out << i->first << " = " << value << endl;
	   //} else
	   //   break;
	}
	// Print out the integer directives.
	for (map<string, int>::const_iterator i = si_directives.begin(); i != si_directives.end(); i++ ) {
	   //if ( i->second != NULL) {
	    out << i->first << " = " << value << endl;
	   //} else break;
	}
}

ostream & operator<<(ostream &out, ConfigDirectives &rhs) {
        rhs.print(out);
        return out;
}

