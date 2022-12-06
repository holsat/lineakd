/***************************************************************************
                          lbutton.cpp  -  description
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

#include <lineak/lbutton.h>
#include <lineak/lineak_core_functions.h>
using namespace std;
using namespace lineak_core_functions;


LButton::LButton() : LObject(snull, PRESS, BUTTON) {
	button = 0;
}
LButton::~LButton() {}
LButton::LButton(string name, unsigned int ibutton) : LObject(name, PRESS, BUTTON), button(ibutton) {
}
/**
void LButton::serialize(ostream &out) {
   int com=0;
   if (name != snull) {
      out << "object=" << name;
      out << ":event_type=" << lineak_core_functions::getEventTypeString(event_type);
      out << ":type=" << lineak_core_functions::getTypeString(type);
      out << ":button=" << button;
      if (!isUsedAsToggle()) {
          out << ":toggle=false";
          for (map<unsigned int,LCommand>::iterator i = commands.begin(); i!= commands.end(); i++, com++) {
             out << ":command_entry=" << com;
             out << ",modifier=" << lineak_core_functions::getModifierString((i->first));
             out << ",command=" << commands[(i->first)];
             out << ",display_name=" << commandDisplayNames[(i->first)];
          }
       }
       else {
          out << ":toggle=true" << endl;
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
*/
void LButton::print(ostream&out)  {
   if (name != snull) {
   	out << "Object: " << name << endl;
	out << "   event_type = " << getEventTypeString() << endl;
	out << "   type = " << getTypeString() << endl;
	out << "   button = " << button << endl;
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
		//out << "   and command = " << commands[name] << endl;
	}
	else {
		out << "   used_toggle = true" << endl;
		for (map<string,LCommand>::const_iterator i = tog_commands.begin(); i != tog_commands.end(); i++) {
			out << "    for togglename = " << i->first << " command = " << i->second << endl;
		        out << "    and display name = " << tog_commandDisplayNames[i->first] << endl;
		}
		//out << "   and command = " << commands[name] << endl;
	}
        out << endl;
   } else
      cerr << "Attempting to output an empty button!" << endl;
}

ostream & operator<<(ostream &out, LButton &rhs) {
   //rhs.serialize(out);
   rhs.print(out);
   return out;
}
