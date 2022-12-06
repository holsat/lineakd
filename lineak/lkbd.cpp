/***************************************************************************
                          lkbd.cpp  -  description
                             -------------------
    begin                : Fri Jan 17 2003
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

#include <lineak/lkbd.h>
#include <lineak/lconfig.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/lkey.h>
#include <lineak/lbutton.h>
#include <lineak/lcommand.h>
#include <map>
#include <sstream>
#include <algorithm>
using namespace lineak_core_functions;
extern bool verbose;
extern bool very_verbose;

//int LKey::keycount=0;

LKbd::LKbd() {
	name = snull;
	model = snull;
	brand = snull;
}
LKbd::LKbd(string iname, string ibrand, string imodel) {
	name = iname;
	brand = ibrand;
	model = imodel;
}
LKbd::~LKbd(){
   clear();
}
bool LKbd::operator==(LKbd & rhs) {
   if (this != &rhs) {
      if (name != rhs.name) return false;
      if (model != rhs.model) return false;
      if (brand != rhs.brand) return false;
      if (table.size() != rhs.table.size()) return false;
      /** Make sure we have the same entries */
      /** Make sure rhs has the same entries we do, and that we have the same entries rhs does. */
      map<string, LObject*>::iterator ib = table.begin();
      map<string, LObject*>::iterator ie = table.begin();
      map<string, LObject*>::iterator rib = rhs.table.begin();
      map<string, LObject*>::iterator rie = rhs.table.end();
      /** Yes, we are only checking the LObjects against each other, not the actual LKey
          or LButton objects. I'm lazy. */
      if (!equal(ib,ie, rib)) return false;
      if (!equal(rib,rie,ib)) return false;
   }
   return true;
}
void LKbd::clear() {
   //name = "";
   //model = "";
   //brand = "";
   /** Remove all of the mappings */
   for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
      delete(m->second);
      m->second = NULL;
   }
   table.clear();
}
LKbd & LKbd::operator=(const LKbd & rhs) {
   if (this != &rhs) {
      name = rhs.name;
      model = rhs.model;
      brand = rhs.brand;
      clear();
      /** Make new objects that are copies of the ones in rhs, and insert them into our table */
      for (map<string, LObject*>::const_iterator m = rhs.table.begin(); m != rhs.table.end(); m++) {
         if  (m->second->getType() == CODE || m->second->getType() == SYM) {
	    LKey * key = new LKey();
	    LKey * obj = static_cast<LKey *>(m->second);
	    (*key) = (*(obj));
	    table[m->first] = key;
	    key = NULL;
	 }
	 else if (m->second->getType() == BUTTON) {
	    LButton * btn = new LButton();
	    LButton * obj = static_cast<LButton *>(m->second);
	    (*btn) = (*(obj));
	    table[m->first] = btn;
	    btn = NULL;
	 }
      }
   }
return *this;
}
   
/** Return the map of names to objects */
const map<string, LObject*>& LKbd::getObjects() const {
	return table;
}
const map <string, LObject*> LKbd::getObjectsOfType(KeyType_t type) const {
   map<string,LObject*> tmap;
   for (map<string, LObject*>::const_iterator m = table.begin(); m != table.end(); m++) {
      if ((m->second)->getType() == type) {
         tmap[m->first] = m->second;
      }
   }   
   return tmap;
}
/** Return an lobject by it's name, or if it owns a toggle name that matches. */
LObject* LKbd::getObject(string iname) {
   LObject *key = NULL;
   // If we have an entry under this name
   if ( table.find(iname) != table.end() )
      return table[iname];
   /** If we don't have an entry under this name, perhaps we are looking for a
       object with this particular toggle name. */
   else {
      /** Loop through all of the keys and return the first one that owns this
          name. */
      for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
         key = m->second;
         if (key->ownsName(iname))
            return (key);
      }
   }
   return NULL;	
}
/** Look through the key lobjects and return the one with the matching keycode. */
LObject* LKbd::getObject(int keycode) {
    LKey *key = NULL;
    for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
	key = static_cast<LKey*>(m->second);
	if ( (key->getType() == CODE || key->getType() == SYM )) {
	      if (key->getKeyCode() == keycode) {
		return (m->second);
	      }
	}
    }
    return NULL;
}
/*** Look through the button lobjects and return the one with the matching button */
LObject* LKbd::getObject(unsigned int button) {
    LButton *btn = NULL;
    for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
        btn = static_cast<LButton*>(m->second);	
        if ( btn->getType() == BUTTON && btn->getButton() == button) {
               return (btn);
        }
     }
     return NULL;
}

void LKbd::removeObject(string iname) {
   LObject *key = NULL;
   // If we have an entry under this name
   if ( table.find(iname) != table.end() ) {
      delete (table[iname]);
      table.erase(iname);
   }
   /** If we don't have an entry under this name, perhaps we are looking for a
       object with this particular toggle name. */
   else {
      /** Loop through all of the keys and return the first one that owns this
          name. */
      for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
         key = m->second;
         if (key->ownsName(iname)) {
	    delete (table[iname]);
            table.erase(iname);
	 }
      }
   }
}
/** Look through the key lobjects and return the one with the matching keycode. */
void LKbd::removeObject(int keycode) {
    LKey *key = NULL;
    for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
	key = static_cast<LKey*>(m->second);
	if ( (key->getType() == CODE || key->getType() == SYM )) {
	      if (key->getKeyCode() == keycode) {
	         delete (m->second);
                 table.erase(m);
	      }
	}
    }
    //key = new LObject();
}
/*** Look through the button lobjects and return the one with the matching button */
void LKbd::removeObject(unsigned int button) {
    LButton *btn = NULL;
    for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
        btn = static_cast<LButton*>(m->second);	
        if ( btn->getType() == BUTTON && btn->getButton() == button) {
           delete (m->second);
           table.erase(m);
        }
     }
}


/** Return a list of the names indexing the lobjects. This does not return the list
    of toggleable names. */
vector<string> LKbd::getNames() {
  vector<string> names; // = new Vector;

  for (map<string, LObject*>::const_iterator m = table.begin(); m != table.end(); m++)
  	names.push_back(m->first);
	return names;
}
/** Add an object to the map indexed by it's name. */
void LKbd::addObject( LObject * object ) {
        table[object->getName()] = object;
}
/** Set the commands on the lobjects with commands parsed from the config file. */
void LKbd::setCommands(LConfig &config) {

   keycommand_info tmp;
   LObject* obj;
   /** Go through our map of all the key names we have from the definition file.
       Using the key name from the definition file, pull the vector of keycommand_info structures
       from the config file, and set the appropriate stuff in each LObject. */
       
   /** Add commands to a togglable key */
   /** If it's a key with a play|pause form, for each of these forms, get the appropriate commands */
   vmsg("In setting commands!");
   map<string, LObject*>::iterator m = table.begin();
        
   for (; m != table.end(); m++) {
       obj = m->second;
       if (very_verbose) cout << "Processing object: " << obj->getName() << endl;
       /** Get the info vector for the key in it's whole form key1|key2. */
       vector<keycommand_info>& vtmp = config.getKeycomm(obj->getName());
       
      /** If it's a togglable key, and 
          there are no entries for this key in the key1|key2 form, 
          we are using it as a toggle. */
      if ((m->second)->isUsedAsToggle() && vtmp.empty()) {
         vmsg("Processing toggle"); 
         /** If we are using different commands for each toggleable key part */
	 vector<string> names = obj->getToggleNames();
	 for (vector<string>::const_iterator i = names.begin(); i != names.end(); i++) {
	    /** Get the vector containing all information on keys with this name. */
	    vtmp = config.getKeycomm(*i);
	    for (string::size_type i = 0; i < vtmp.size(); i++) {
	      // Get the keycommand_info struct w/ the appropriate info.
	      tmp = vtmp[i];
	      // We no longer have to add a modifier to a toggleable key.
	      // The rest of the code (Xmgr and lineakd-main.cpp understand how to
	      // process toggleable keys w/o resorting to modifiers. 
	      //obj->addModifier(0);
	      obj->setCommand(tmp.command, tmp.parsed_name); // may need to be config_name
	      obj->setToggleCommandDisplayName(tmp.display_name, tmp.parsed_name);
	    }
         }
      }
      else {
	 /** If we are treating a toggleable key as a normal key, i.e. one command 
	     for the whole thing, such as play|pause = command in the config file. 
	     Turn it's toggle status off. */
         if ((m->second)->isUsedAsToggle()) {
	    vmsg("Disabling toggleability on this key");
            (m->second)->setUsedAsToggle(false);
         }
	 if (!vtmp.empty()) {
            vmsg("Ignoring toggle status.");
	    /** Get the vector containing all information on keys with this name. */
	    //vtmp = config.getKeycomm(*i);
	    for (vector<keycommand_info>::size_type i = 0; i < vtmp.size(); i++) {
	      // Get the keycommand_info struct w/ the appropriate info.
	      tmp = vtmp[i];
	      /*
	      cout << "keycommand_info.config_name = " << tmp.config_name << endl;
	      cout << "keycommand_info.parsed_name = " << tmp.parsed_name << endl;
	      cout << "keycommand_info.modifiers = " << tmp.modifiers << endl;
	      cout << "keycommand_info.command = " << tmp.command << endl;
	      */
	      obj->addModifier(tmp.modifiers);
	      obj->setCommand(tmp.command, tmp.modifiers); // may need to be config_name
	      obj->setCommandDisplayName(tmp.display_name, tmp.modifiers);
	    }
         }
	 else {
	    cerr << "Could not find key information for: " << (m->first) << endl;
         }
      }
   }
   if (verbose) cout << "Finished setting commands" << endl;
}
/** Set the command for an lobject indexed by name */
void LKbd::setCommand(string name, LCommand command) {
	table[name]->setCommand(command);
}
/** Return a vector of all of the keycodes. */
vector<int> LKbd::getKeyCodes(){
   vector<int> keycodes;
   LObject * obj;
   LKey * key;
   for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
      obj = m->second;
      if (obj->getType() == CODE || obj->getType() == SYM) {
         key = static_cast<LKey*>(m->second);
         keycodes.push_back(key->getKeyCode());
      }
   }
// cout << "vector has " << keycodes.size() << " elements: { ";
// for (vector<int>::const_iterator it = keycodes.begin(); it != keycodes.end(); it++)
//   cout << *it << " ";
// cout << "} " << endl;

    return keycodes;
	
}
/** Return a vector of all of the button codes. */
vector<unsigned int> LKbd::getButtonCodes() {
   vector<unsigned int> buttoncodes;
   LObject * obj;
   LButton * btn;
   for (map<string, LObject*>::iterator m = table.begin(); m != table.end(); m++) {
      obj = m->second;
      if (obj->getType() == BUTTON ) {
         btn = static_cast<LButton*>(m->second);
         buttoncodes.push_back(btn->getButton());
      }
   }
   return buttoncodes;
}
/** Return the number of objects of type. */
int LKbd::getNumObjectOfType(KeyType_t type) {
   int num = 0;
   for (map<string, LObject*>::const_iterator m = table.begin(); m != table.end(); m++) {
      if ((m->second)->getType() == type) {
         num++;
      }
   }   
   return num;
}
/** Is the keyboard empty? */
const bool LKbd::isEmpty() {
	// An empty keyboard is one that has no name. The rest can be empty data,
	// but we at least need a name.
	if (name == snull)
		return true;
	return false;
}
void LKbd::print(ostream &out) {
   bool have_buttons=false; 
	// Don't print out empty keyboard entries.
   if (name != snull) {
    	out << "[" << name << "]" << endl;
    	out << "     " << "brandname = " << brand << endl;
    	out << "     " << "modelname = " << model << endl;
	for (map< string, string >::iterator i = raw_commands.begin(); i != raw_commands.end(); i++) {
           out << "     " <<  "RAWCOMMAND[" << i->first << "] = " << i->second << endl;
	}
     	out << "     " << "[KEYS]" << endl;
     	for (map<string, LObject*>::iterator i = table.begin(); i != table.end(); i++) {
	   if ( i->second->getType() == CODE || i->second->getType() == SYM ) {
              //cout << "Looping over keys" << endl;
	      LKey * key = static_cast<LKey *>(i->second);
	      //cout << "Examining key..." << endl;
	      //cout << *key << endl;
	      out << "          " << i->first << "=" << key->getKeyCode() << endl;
	   } else if (i->second->getType() == BUTTON) {
		   have_buttons=true;
	   }
	}
     	out << "     " << "[END KEYS]" << endl;

	if (have_buttons==true) {
	   out << "     " << "[BUTTONS]" << endl;
	   for (map<string, LObject*>::iterator i = table.begin(); i != table.end(); i++) {
	      if (i->second->getType() == BUTTON) {
	         LButton * btn = static_cast<LButton *>(i->second);
		 out << "          " << i->first << "=" << btn->getButton() << endl;
	      }
	   }
	   out << "     " << "[END BUTTONS]" << endl;
	}

    	out << "[END " << name << "]" << endl;
   }
   else
      cerr << "Attempting to output an empty keyboard!" << endl;
}

void LKbd::addRawCommand(string name, string rcomm) {

    if( ! (rcomm.empty() || name.empty()) ) { 

       string::size_type startIndex = rcomm.find_first_not_of(" ");
       string::size_type endIndex = rcomm.find_last_not_of(" ");
       string tempString = rcomm;
       rcomm.erase();

       rcomm = tempString.substr(startIndex, (endIndex-startIndex+ 1) );
	
       raw_commands[name] = rcomm;
   }
}

