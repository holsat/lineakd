/***************************************************************************
                          defloader.cpp  -  description
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
#include <lineak/defloader.h>
#include <lineak/lineak_util_functions.h>
#include <lineak/ldef.h>
#include <lineak/lkbd.h>
#include <lineak/lkey.h>
#include <lineak/lobject.h>
#include <lineak/lbutton.h>

using namespace lineak_util_functions;

DefLoader::DefLoader() {
}
DefLoader::DefLoader(string filename) : Loader(filename) {
}
DefLoader::~DefLoader(){
}
/** Process a file with sub sections */
vector<string>* DefLoader::processMulti(vector<string>* rawData) {
        if (rawData->empty())
    		return rawData;

       vector<string> *parsedData = new vector<string>;
       string tmpStore = snull;
       string key=snull;
       string data=snull;
       for(vector<string>::size_type i = 0; i < rawData->size(); i++) {
          	tmpStore = (*rawData)[i];
            data = strip_space(tmpStore);
            parsedData->push_back(data);
       }
      delete (rawData);
      return parsedData;
}
/** Load a keyboard definition file */
LDef DefLoader::loadDef() {
      LDef def(file);
      def.setTable(*(getDefObj(processMulti(loadFile()))));
      return def;
}
/** Load the config for the file filename */
LDef DefLoader::loadDef(string filename) {
    LDef tmp;
	if ( setFile(filename) )
		return (loadDef());
	return	 tmp;
}

/** Return a keyboard definition object. */
/** We take out the standard heading from the map,
	brandname, modelname and the keyboard name.
	Then everything that is left should be key definitions.
	We then add those keys to the keyboard object */
map<string,LKbd*>* DefLoader::getDefObj(vector<string> *results){
	map<string,LKbd*> *deftable = new map<string,LKbd*>;
	// Initialize the table.
	deftable->clear();
         // If we don't have any values loaded. Return empty keyboard table.
        if (results == pnull )
    	   return deftable;
        if (results->empty()) {
		delete (results);
		return deftable;
	}

	LKbd *def = new LKbd();
	string tmp,tmp2,tmp3=snull, heading = snull;
 	string key=snull,data=snull;
	//unsigned int loc = 0;
	string::size_type loc = 0;

   for (vector<string>::size_type i=0; i < results->size(); i++) {
        tmp = (*results)[i];
	// If this line of the file has brackets, it's a beginning or end of a section.
        if ( tmp.find('[') != string::npos && tmp.find(']') != string::npos && ( tmp.find("RAWCOMMAND") == string::npos ) ) {
           // The Keyboard Name, i.e. it's a section heading w/o the keyword KEY,BUTTON or END
	   if ( tmp.find("KEY") == string::npos && 
	        tmp.find("BUTTON") == string::npos &&
		tmp.find("END") == string::npos )
	      // Assign the keyboard identifier. i.e. ACEAKV12
              def->name = strip_space(tmp.substr(1,tmp.size()-2));
           /** create a new keyboard object */
           // End of Keyboard Definition Marker i.e. there is an END with no KEY or BUTTON
           if ( tmp.find("KEY") == string::npos && 
	        tmp.find("BUTTON") == string::npos && 
	        tmp.find("END") != string::npos ) {
              heading = "END";
              /**  set heading = snull;
                   add the keyboard config object to a map<string,LKbd>
                   create a new keyboard config object to add to */
              heading = snull;
              (*deftable)[def->name]= def;
	      //cout << "Outputing defintion" << endl;
	      //cout << *def << endl;
              def = new LKbd();
           }
           // Beginning of KEYS section. i.e. there is a KEYS keyword, and no END
           if ( tmp.find("KEY") != string::npos && tmp.find("END") == string::npos )
              heading = "KEYS";
           // End of KEYS section
           if ( tmp.find("KEY") != string::npos && tmp.find("END") != string::npos ) {
              heading = "END KEYS";
              continue;
           }
	   // Beginning of BUTTONS section. i.e. there is a BUTTONS keyword, and no END
           if ( tmp.find("BUTTON") != string::npos && tmp.find("END") == string::npos )
              heading = "BUTTONS";
           // End of BUTTONS section
           if ( tmp.find("BUTTON") != string::npos && tmp.find("END") != string::npos ) {
              heading = "END BUTTONS";
              continue;
           }
        }
        else {
	   // Handle empty lines.
	   loc = tmp.find('=');
           // empty line
   	   if (loc == string::npos)
   	     continue;

	   // key == the name of the key.
           key = strip_space(tmp.substr(0,loc));
	   // data == the keycode of the key.
           data = strip_space(tmp.substr(loc+1, tmp.size()-(loc+1)));
	   if ( heading == snull ) {
              // found a global entry
              if (loc == 0) continue;
              if ( key == "brandname")
                 def->brand = strip(data,"\"");
              if ( key == "modelname")
                 def->model = strip(data,"\"");
	      if ( key.find("RAWCOMMAND") != string::npos ) {
                 // Parse the RAWCOMMAND name 
		      string::size_type i,j;
		      i = key.find('[');
		      j = key.find(']');
		      string name = key.substr(i+1, (key.size() - (i+2)) );
		      data = lineak_util_functions::strip(data, "\"#");
		      def->addRawCommand(name, data); 
		      //if (verbose) cout << "addRawCommand(" << name << ", " << data << ");" << endl;
	      } 
           }
           else {
              // found a key or button entry
              if (loc == 0) continue;
	      if ( heading == "KEYS" ) {
	         //cout << "code = " << data.c_str();
		 //cout << ", int code = " << atoi(data.c_str()) << endl;
		 int code = atoi(data.c_str());
	         LKey * akey = new LKey(key,code);
		 //cout << "Adding key: " << key << " = " << code << endl;
		 //cout << akey;
	         /** Now determine if this is a toggleable key. We will have to parse
	             the name format name1|name2 if indeed it is a toggleable key. */
		 if (key.find('|') != string::npos) {
		    key+='|';
		    akey->setUsedAsToggle(true);
		    string::size_type index;
		    string tmp;
	            //vector<string>names;
                    // compairing index to npos while assigning is not portable
                    // see Josuttis  496
                    while (key.find('|') != string::npos) {
                       // messier than assignment in while loop above, 
                       // but portable
                       index = key.find('|');
		       tmp = key.substr(0,index);
		       //cout << "adding toggle name: " << tmp << " to " << akey->getName() << endl;
                       akey->addToggleName(tmp);
                       key.erase(0, index+1);
                    }
		    //cout << *akey;
                 }
		 //cout << "adding key" << endl;
		 //cout << *akey << endl;
                 def->addObject(akey);
		 akey = NULL;
		 
	      }
	      if (heading == "BUTTONS" ) {
	         //cout << "Adding a button" << endl;
		 LButton * abutton = new LButton(key,atoi(data.c_str()));
	         def->addObject(abutton);
		 abutton = NULL;
	      }
           }
        }
   }
   // Initialize and fill in the values for the config object here.
   delete (results);
   return (deftable);
}

LDef & operator>>(DefLoader &ldr, LDef &ldef) {
	return (ldef = ldr.loadDef());
}
