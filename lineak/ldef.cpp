/***************************************************************************
                          ldef.cpp  -  description
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

#include <iterator>
#include <algorithm>
#include <lineak/ldef.h>
#include <lineak/defloader.h>
#include <lineak/lconfigdata.h>

//using namespace lineak_util_functions;

LDef::LDef(){
	deffilename = snull;
	table.clear();
}
LDef::LDef(string file) : deffilename(file) {
}

LDef::LDef(DefLoader &def){
	if (def.getFile() != snull) {
    	setTable(def.loadDef().getTable());
     	setFilename(def.getFile());
    }
    else 
    	setFilename(snull);
     	
}
LDef::~LDef(){
}
/** Return a list of keyboard identifiers */
vector<string> LDef::getKeyboards() const {
	vector<string> result;
    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
		result.push_back(m->first);
	}
	return result;
}
/** Return a sorted list of brands */
vector<string> LDef::getBrands() const {
	vector<string> result;
    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
		result.push_back(m->second->brand);
	}
	sort(result.begin(),result.end());
	vector<string> ret(result.begin(), unique(result.begin(), result.end()));
	return ret;
}
vector<string> LDef::getModels() const {
	vector<string> result;
    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
		result.push_back(m->second->model);
	}
	sort(result.begin(),result.end());
	vector<string> ret(result.begin(), unique(result.begin(), result.end()));
	return ret;
}
map<string,LKbd*> LDef::getModels(string brand) {
    map<string,LKbd*> models;
    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
    	if (m->second->brand == brand)
     		models[m->first] = m->second;
    }
    return models;
}
map<string,LKbd*>& LDef::getTable() {
	return table;
}
LKbd& LDef::getKeyboard(string brand, string model) {

    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
    	if (m->second->brand == brand && m->second->model == model)
     		return *(m->second);
    }
    // If we can't find our keyboard, return a reference to our empty keyboard.
    blank.name = snull;
    return (blank);
}
LKbd& LDef::getKeyboard(string name) {
    //cout << "Getting keyboard: " << name << endl;
    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
        //cout << "Examining keyboard " << m->first << endl;
    	if (m->first == name) {
	        //cout << "returning keyboard " << m->first << endl;
		//cout << "returning keyboard " << m->second->name << endl;
		//cout << *(m->second);
     		return *(m->second);
	}
    }
    cerr << "Returning a blank keyboard." << endl;
    // If we can't find our keyboard, return a reference to our empty keyboard.
    blank.name = snull;
    return (blank);
}
const bool LDef::hasKeyboard(string name) {
    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
    	if (m->first == name)
     		return true;
    }
   return false;
}

void LDef::setFilename(string file) {
	deffilename = file;
}
const bool LDef::isEmpty() {
	if (table.empty() || (deffilename == snull))
		return true;
	else
		return false;
	//return (table.empty() || (deffilename == snull));
}
void LDef::print(ostream &out) {
    out << DEF_HEADER;
    out << endl;
    for (map<string,LKbd*>::const_iterator m = table.begin(); m != table.end(); m++) {
    	out << *(m->second);
    }
}
void LDef::addKeyboard(LKbd &newkb) {
    // Add a keyboard.
	table[newkb.name] = &newkb;

}
void LDef::addKeyboards(LDef &newdef) {
    map<string,LKbd*> &tmp = newdef.getTable();
    for (map<string,LKbd*>::const_iterator m = tmp.begin(); m != tmp.end(); m++) {
    	// Add a keyboard.
	table[m->first] = m->second;
    }
}
