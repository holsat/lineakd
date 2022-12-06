/***************************************************************************
                          configdirectives.h  -  description
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

#ifndef CONFIGDIRECTIVES_H
#define CONFIGDIRECTIVES_H
#include <map>
#include <string>
#include <vector>
//class LConfig;

/**This class contains a map of the global configuration directives from the users config file.
	*It is used to initialize the LConfig object. I'm encapsulating it b/c I may change it at a
	*later point. I haven't decided.
  *@author Sheldon Lee Wen
  */
using namespace std;

class ConfigDirectives {
public:
	ConfigDirectives();
	ConfigDirectives(const ConfigDirectives &rhs);
	virtual ~ConfigDirectives();
	ConfigDirectives & operator=(const ConfigDirectives &rhs);
	/** Return a string value from a string key. */
	virtual string getValue(string key) const;
	//	virtual int getValue(int key);
	/** Add a string value */
	virtual void addValue(string key, string value);
//	virtual void addValue(int key, int value);
	/** Add an int value */
	virtual void addValue(string key, int value);
	virtual void removeValue(string key);
	/** Return an int value from a string key */
	virtual int getIntValue(string key) const;
	/** Clear all of the tables, integer and string */
	virtual void clear() { 	directives.clear();
					//	int_directives.clear();
						si_directives.clear(); }
	/** This is for convenience and only works for string values */
	virtual string& operator[](string key);
	/** Return a string vector of all keys */
	virtual vector<string> getKeys();
	virtual vector<string> getIntKeys();
	/** Tell us if a key returns an integer value */
	virtual bool isIntValue(string key);
	/** Tell us if a key has a value */
	virtual bool isSet(string key);
	/** Tell us if there are not directives */
	virtual bool isEmpty();
	/** print */
	virtual void print() const;
	/** print */
	virtual void print(ostream& out) const;
		
protected:
	map<string,string> directives;
//	map<int,int> int_directives;
	map<string,int> si_directives;
};

ostream & operator<<(ostream &out, ConfigDirectives &rhs);

#endif
