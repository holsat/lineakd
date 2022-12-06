/***************************************************************************
                          ldef.h  -  description
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

#ifndef LDEF_H
#define LDEF_H

#include <string>
#include <map>

#include <lineak/definitions.h>
#include <lineak/lkbd.h>
#include <lineak/lconfigdata.h>
#include <lineak/ldef.h>

class DefLoader;
class LDef;
/**Contains a Lineak Keyboard Definition file.
  *@author Sheldon Lee Wen
  */

class LDef : public LConfigData {
public: 
	LDef();
	LDef(string filename);
	LDef(DefLoader &def);
	virtual ~LDef();
	/** Return a list of keyboards */
	vector<string> getKeyboards() const;
	/** Return a list of brands */
	vector<string> getBrands() const;
	/** Return a list of models */
	vector<string> getModels() const;
	/** Get all models of keyboards with a certain brand. */
	map<string,LKbd*> getModels(string brand);
	/** Get all of the keyboards */
	map<string,LKbd*>& getTable();
	/** Get a specific keyboard */
	LKbd& getKeyboard(string brand, string model);
	/** Get a specific keyboard */
	LKbd& getKeyboard(string name);
	/** Do we have a keyboard with name name */
	const bool hasKeyboard(string name);
	/** Add a keyboard to the definition file */
	void addKeyboard(LKbd &newkb);
	/** Add all keyboard from another definition file to ours. */
	void addKeyboards(LDef &newdef);
	/** Set the keyboard table */
	void setTable(map<string,LKbd*> &itable) { table = itable; }
	/** Get the name of the file containing this definition */
	string getFilename() { return deffilename; }
	/** Set the file */
	void setFilename(string file);
	/** Is this definition empty */
	virtual const bool isEmpty();
	/** print */
	virtual void print(ostream&out);
//	void print(void) const;

private:
	map<string,LKbd*> table;
	string deffilename;
	LKbd blank;
};

#endif

