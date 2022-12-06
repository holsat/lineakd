/***************************************************************************
                          lkey.cpp  -  description
                             -------------------
    begin                : Fri Feb 28 2003
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

#include <lineak/lkey.h>
#include <lineak/lineak_core_functions.h>
#include <iostream>
#include <sstream>
//#include <X11/Xlib.h>

int LKey::keycount = 0;
using namespace std;
using namespace lineak_core_functions;


LKey::LKey() : LObject(snull, PRESS, CODE) {
   keysym = 0x1008FF01 + keycount;
   keycount++;
   keycode = 0;
}
LKey::~LKey() {}
LKey::LKey(string name, int code) : LObject(name, PRESS, CODE) {
   keysym = 0x1008FF01 + keycount;
   keycount++;
   keycode = code;
}
void LKey::setKeySym(KeySym ikeysym) {
	keysym = ikeysym;
}
void LKey::setKeyCode(KeyCode code) {
	keycode = code;
}
/**
void LKey::serialize(ostream &out) {
   int com=0;
   if (name != snull) {
      out << "object=" << name;
      out << ":event_type=" << lineak_core_functions::getEventTypeString(event_type);
      out << ":type=" << lineak_core_functions::getTypeString(type);
      out << ":keycode=" << keycode;
      out << ":keysym=" << keysym;
      if (isToggle())
          out << ":is_toggle=true";
      else
          out << ":is_toggle=false";

      if (!isUsedAsToggle()) {
          out << ":used_toggle=false";
          for (map<unsigned int,LCommand>::iterator i = commands.begin(); i!= commands.end(); i++, com++) {
             out << ":command_entry=" << com;
             out << ",modifier=" << lineak_core_functions::getModifierString((i->first));
             out << ",command=" << commands[(i->first)];
             out << ",display_name=" << commandDisplayNames[(i->first)];
          }
       }
       else {
          out << ":used_toggle=true" << endl;
          string s;
          for (queue<string>::size_type i = 0; i<toggle_names.size(); i++, com++) {
             s = toggle_names.front();
             out << ":command_entry=" << com;
             out << ",toggle_name=" << s;
             out << ",command=" << tog_commands[s];
             out << ",display_name=" << tog_commandDisplayNames[s];
             toggle_names.pop();
             toggle_names.push(s);
          }
      }
  }
  else {
    error("Attempting to output an empty key!");
  }
}

void LKey::unserialize(istream &in) {
  string s;
  string uns,key,value;
  vector<string> parts;
  unsigned int i, j, index1, index2;
  cout << "STARTING UNSERIALIZE" << endl;
  
  // Build the string to unserialize 
  while (in >> s) {
	  uns+=s;
  }
  
  index1 = 0;
  index2 = 0;
 
  while ((index2 = uns.find(':')) != string::npos) {
    s = uns.substr(index1,index2);
    uns.erase(index1,index2-index1+1);
    parts.push_back(s);
    // Parse and add.
    j = s.find('=');
    key = s.substr(0,j);
    value = s.substr(j+1,s.size()-1);
    cout << "key = " << key << endl;
    cout << "value = " << value << endl;
    if (key == "object")
       name = value;
    if (key == "event_type")
       event_type = lineak_core_functions::getEventType(value);
    if (key == "type")
       type = lineak_core_functions::getType(value);
    if (key == "keycode")
       keycode = atoi(value.c_str());
    if (key == "keysym")
       keysym = atoi(value.c_str());
    if (key == "toggle") {
       if (value == "true")
          setUsedAsToggle(true);
       if (value == "false")
          setUsedAsToggle(false);
    }
  }
  // uns now has the last entry.
  parts.push_back(uns);

  for (vector<string>::iterator it = parts.begin(); it != parts.end(); it++) {

    // Parse and add.
    j = s.find('=');
    key = s.substr(0,j);
    value = s.substr(j+1,s.size()-1);
    cout << "key = " << key << endl;
    cout << "value = " << value << endl;
    if (key == "object")
       name = value;
    if (key == "event_type")
       event_type = lineak_core_functions::getEventType(value);
    if (key == "type")
       type = lineak_core_functions::getType(value);
    if (key == "keycode")
       keycode = atoi(value.c_str());
    if (key == "keysym")
       keysym = atoi(value.c_str());
    if (key == "toggle") {
       if (value == "true")
          setUsedAsToggle(true);
       if (value == "false")
          setUsedAsToggle(false);
    }
    if (key == "command_entry") {
	    cout << "Parsing command: " << s << endl;
	    //while there are no more ',' in the string
	    //parse the commands and add them.
    }
	  
  }
  // s now has the last command entry. Parse and add them.
  cout << "part: " << uns << endl;

  //cout << "We have been given: " << uns << " to unserialize" << endl;
  cout << "DONE UNSERIALIZING" << endl; 
	
}
**/

void LKey::print(ostream&out)  {
   if (name != snull) {
	out << "Object: " << name << endl;
	out << "   event_type = " << LObject::getEventTypeString() << endl;
	out << "   type = " << LObject::getTypeString() << endl;
	out << "   keycode = " << keycode << endl;
	out << "   keysym = " << keysym << endl;
	//out << "   modifier = " << getModifierString() << endl;
	//out << "   modifier numeric = " << modifier << endl;
	if (isToggle())
            out << "   is_toggle=true";
        else
            out << "   is_toggle=false";

	if (!isUsedAsToggle()) {
		out << "   used_toggle = false" << endl;
		for (map<unsigned int,LCommand>::iterator i = commands.begin(); i!= commands.end(); i++) {
			out << "   modifier = " << LObject::getModifierString((i->first));
			out << " and command = " << commands[(i->first)];
			out << "   and display name = " << commandDisplayNames[(i->first)] << endl;
		}		
		
	}
	else {
		out << "   used_toggle = true" << endl;
		out << "   number of toggle names = " << toggle_names.size() << endl;
		string s;
		for (queue<string>::size_type i = 0; i< toggle_names.size(); i++) {
		   s = toggle_names.front();
		   out << "   toggle name = " << s << endl;
		   out << "    and command = " << tog_commands[s];
		   out << "    and display name = " << tog_commandDisplayNames[s] << endl;
		   toggle_names.pop();
		   toggle_names.push(s);
		}
	}
        out << endl;
   } else
      cerr << "Attempting to output an empty key!" << endl;
}

ostream & operator<<(ostream &out, LKey &rhs) {
   //rhs.serialize(out);
   rhs.print(out);
   return out;
}
//istream & operator>>(istream &in, LKey &rhs) {
   //rhs.unserialize(in);
//   return in;
//}
