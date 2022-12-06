//
// C++ Implementation: lobject
//
// Description:
//
//
// Author: Sheldon Lee Wen <tormak@rogers.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <iostream>
#include <algorithm>
#include <lineak/lobject.h>
#include <lineak/lcommand.h>
#include <lineak/definitions.h>
#include <lineak/lineak_core_functions.h>
#include <lineak/lconfig.h>

using namespace std;
using namespace lineak_core_functions;

LObject::LObject()
{
        name = snull;
	blank = "";
        is_toggle = false;
        used_toggle = false;
        dtoggle = false;
        //modifier = 0;
        event_type = PRESS;
        type  = CODE;
	modifiers.clear();
	//modifiers.push_back(0);
	
}
LObject::~LObject()
{
}
/*
bool LObject::operator==(LObject &rhs) {
  if (this != rhs.this ) {
     if (name != rhs.name) return false;
     if (event_type != rhs.event_type) return false;
     if (type != rhs.type) return false;
     if (modifier != rhs.modifier) return false;
     if (used_toggle != rhs.used_toggle) return false;
     if (is_toggle != rhs.is_toggle) return false;
     if (dtoggle != rhs.toggle) return false;
    
     if (commands != rhs.commands) return false;
     if (toggle_modifiers != rhs.toggle_modifiers) return false;
     if (toggle_names != rhs.toggle_names) return false;
  }
  return true;
}
*/
LObject::LObject(string iname, EventType_t ietype, KeyType_t itype) : name(iname), event_type(ietype), type(itype)
{
	if (name.find('|') != string::npos)
           is_toggle = true;
	else
	   is_toggle = false;

	used_toggle = false;
	blank = "";
	modifiers.clear();
	dtoggle = false;
        //internal_init();
}

void LObject::addModifier(unsigned int imodifier) { 
	//modifiers.push_back(imodifier); 

	if (!isUsedAsToggle()) {
	   modifiers.push_back(imodifier);
	}
	else {
	   error("Attempting to add a modifier to a key that is being used as toggleable");
	   cout << "Error for key: " << name << endl;
	   cout << "Error adding modifier: ";
	   if (lineak_core_functions::getModifierString(imodifier) == "")
              cout << "default" << endl;
	   else
              cout << lineak_core_functions::getModifierString(imodifier) << endl;
	}
}

/** If this is not a toggleable key, return the command that corresponds to the name
        as defined in the keyboard definitions file. Else, return the command for the current
        state of the key and put the current state at the end of the queue. */
LCommand & LObject::getCommand(unsigned int mod) {
        if (isUsedAsToggle()) {
	   return tog_commands[toggle_names.front()];
        }
	return commands[mod];
}
LCommand & LObject::getToggleCommand(const string& togname) {
 //TODO: FIXME. I need to return the toggle command associated with toggle_name.
    
    if (isUsedAsToggle() && ownsName(togname))
	    return tog_commands[togname];
    else {
	    empty.setCommand("");
	    return empty;
    }
}

string & LObject::getCommandDisplayName(unsigned int mod) {
	if (isUsedAsToggle()) {
	   return tog_commandDisplayNames[toggle_names.front()];
	}
	return commandDisplayNames[mod];
}

/** Set the command for a non-togglable key. */
void LObject::setCommand(LCommand command, unsigned int mod) {
        if (!command.isEmpty()) {
           if (!isUsedAsToggle()) {
	      commands[mod] = command;
	      if (!hasModifier(mod))
	         addModifier(mod);
	   }
        }
}

/** Set the command display name for a non-toggleable key. */
void LObject::setCommandDisplayName(string dname, unsigned int mod) {
       if (!isUsedAsToggle()) {
	  commandDisplayNames[mod] = dname;
       }	  
}

void LObject::setToggleCommandDisplayName(string dname, const string iname) {
      if (isUsedAsToggle() && iname != snull) {
	  tog_commandDisplayNames[iname] = dname;
      }
}

string & LObject::getToggleCommandDisplayName(const string& togname) {
      if (isUsedAsToggle() && togname != snull && ownsName(togname))
	      return tog_commandDisplayNames[togname];
      else {
	      blank = "";
	      return blank;
      }
}

/** Set the command for a toggle state. This needs to have a properly parsed name. */
void LObject::setCommand(LCommand command, string iname) {
        if (!command.isEmpty()) {
           if (isUsedAsToggle() && (iname != snull) && (iname != "")) {
	      tog_commands[iname] = command;
           } 
        }
}

bool LObject::hasModifier(const unsigned int modifier) {
   for (vector<unsigned int>::size_type i=0; i < modifiers.size(); i++) {
      if ( modifiers[i] == modifier )
        return true;
   }
      return false;
}

string LObject::getModifierString(unsigned int modifier) {
	return lineak_core_functions::getModifierString(modifier);
}

string LObject::getEventTypeString() {
	return lineak_core_functions::getEventTypeString(event_type);        
}

string LObject::getTypeString() {
	return lineak_core_functions::getTypeString(type);
} 

/**
void LObject::serialize(ostream &out) {
   int com=0;
   if (name != snull) {
      out << "object=" << name;
      out << ":event_type=" << lineak_core_functions::getEventTypeString(event_type);
      out << ":type=" << lineak_core_functions::getTypeString(type);
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
*/
/** getKeycommand_info()
 * 
 * Return a vector of structs of the form: 
 *   struct keycommand_info
 *   {
 *      string config_name; //Name as it appears in the config file. 
 *                               i.e. [Display]key+modifier
 *      string parsed_name; //Parsed name, corresponds to the 'name' member variable.
 *      string display_name; //Display name.
 *      unsigned int modifiers; //Modifiers for this entry, in text form.
 *      LCommand command; //LCommand structure associated with this entry.
 *   };
 * Each structure represents one entry in the users config file, for the same key.
*/
/**
vector<keycommand_info> LObject::getKeycommand_info() {
   keycommand_info entry;
   vector<keycommand_info> list;
   cout << "THIS IS NOT CORRECT. FIXME!" << endl; 
   cout << "Does not handle toggleable keys!" << endl;
//   if (tog_commands.empty()) {
      // Loop through all of the commands assigned to this object and construct a keycommand_info struct
      // and add it to the vector to return.
      for (map<unsigned int,LCommand>::iterator i = commands.begin(); i!= commands.end(); i++) {
	 entry.parsed_name = name;
         entry.modifiers = i->first;
	 entry.display_name = commandDisplayNames[(i->first)];
	 entry.command = commands[(i->first)];
	 // Build the config file name.
	 if ( entry.display_name != "" ) {
	    entry.config_name = '[' + entry.display_name;
	    entry.config_name += "] ";
	 }
	 entry.config_name += name;
	 if (entry.modifiers != 0) {
	    entry.config_name += '+';
	    entry.config_name += lineak_core_functions::getModifierString((i->first));
         }
	 list.push_back(entry);
	 entry.modifiers = 0;
	 entry.display_name = "";
	 entry.config_name = "";
      }
  // }
  // else {
  // */
      /** There are two cases. 
       *   If this is a toggleable key it will have been initialized by the
       *   keyboard definition loader with it's name in the form key1|key2.
       *   The definition loader would also add a toggle name for each key form: key1,key2
       *   1) If we are using this as a toggleable key the config file has a command
       *      assigned to each key form. This is the form of the else.
       *   2) If we are not using this as a toggleable key, we have a command assigned
       *      to the form key1|key2
       *   
      string s;
      for (queue<string>::size_type i = 0; i<toggle_names.size(); i++) {
          s = toggle_names.front();
          //out << " toggle name = " << s << endl;
          //out << "    and command = " << tog_commands[s] << endl;
	  entry.parsed_name = name;
	  entry.modifiers = 0;
	  entry.display_name = tog_commandDisplayNames[s];

	  if ( entry.display_name != "" ) {
             entry.config_name = '[' + entry.display_name;
             entry.config_name += "] ";
          }
	  entry.config_name += name;

	  toggle_names.pop();
	  toggle_names.push(s);
          list.push_back(entry);
      } */
   /** //}
   return list;
}
**/

void LObject::print(ostream&out) {
   if (name != snull) {
      out << "Object: " << name << endl;
      out << "   event_type = " << lineak_core_functions::getEventTypeString(event_type) << endl;
      out << "   type = " << lineak_core_functions::getTypeString(type) << endl;
      if (isToggle())
          out << "   is_toggle=true";
      else
          out << "   is_toggle=false";

      if (!isUsedAsToggle()) {
         out << " used_toggle = false" << endl;
         for (map<unsigned int,LCommand>::iterator i = commands.begin(); i!= commands.end(); i++) {
            out << "   modifier = " << lineak_core_functions::getModifierString((i->first));
            out << " and command = " << commands[(i->first)] << endl;
	    out << " and display name = " << commandDisplayNames[(i->first)] << endl;
         }
      }
      else {
         out << " used_toggle = true" << endl;
         out << " number of toggle names = " << toggle_names.size() << endl;
         string s;
         for (queue<string>::size_type i = 0; i<toggle_names.size(); i++) {
            s = toggle_names.front();
            out << " toggle name = " << s << endl;
            out << "    and command = " << tog_commands[s] << endl;
	    out << "    and display name = " << tog_commandDisplayNames[s] << endl;
            toggle_names.pop();
            toggle_names.push(s);
         }
      }
   }
   else
      error("Attempting to output an empty key!");
}

void LObject::addToggleName(const string iname) {
   // We shouldn't be doing this here, but if we don't toggle keys don't work.
   // We need to track down why.
   setUsedAsToggle(true);
   if (isUsedAsToggle()) {
      if ( iname != snull ) {
        toggle_names.push(iname);	   
      } else {
        string err = "Attempted to add an empty toggle name: " + iname + " to object: " + name;
        error(err);
      }
   }
   else
	   error("Attempt to add a toggle name to an object that is not set to be used as toggleable");
}

vector<string> LObject::getToggleNames() {
	vector<string> names;
	queue<string> tmp = toggle_names;
	while(!tmp.empty()) {
	   names.push_back(tmp.front());
	   tmp.pop();
	}
	return names;
}

bool LObject::ownsName(string iname) {
   if ( name == iname )
      return true;

   if (isUsedAsToggle()) {
/*      queue<string>::iterator it = toggle_names.begin();
      for (; it != toggle_names.end(); it++) {
         if ( *it == togname )
            return true;
   }
   */
      queue<string> tmp = toggle_names;
      while(!tmp.empty()) {
         if ( iname == tmp.front()  )
            return true;
         tmp.pop();
      }
   }
   return false;
}
// void LObject::setToggleModifier(string iname, unsigned int imodifier) {
//    if (used_toggle && iname != snull && iname != "") {
//       toggle_modifiers[iname] = imodifier;
//    }
// }
//
bool LObject::isToggle() const {
        return is_toggle;
}

bool LObject::isUsedAsToggle() const {
        return used_toggle;
}

void LObject::toggleState() {
        if (isUsedAsToggle()) {
           string name = toggle_names.front();
           toggle_names.pop();
           toggle_names.push(name);
        }
}
/** Set the key to be used as a toggleable or not key. */
void LObject::setUsedAsToggle(bool tog) { 
   if (isToggle()) {
	used_toggle = tog; 
	// We are being used as a toggleable key
	// and we are being switched to a non-toggleable key.
	if (isUsedAsToggle() && !tog) {
	   // Clear the toggleable stuff
	   // Populate the modifier stuff.
	}
	// We are not being used as a toggleable key
	// and we are being switched to a toggleable key.
	else if (!isUsedAsToggle() && tog) {
	   // Clear the modifier stuff
	   // Populate the toggleable stuff.
	}
   }
}

/** Return the next name in the toggle queue. The toggle is not rotated until
    you actually get the command with getCommand() This is to keep the name
    and the command to be run in sync. */
string LObject::getNextToggleName() {
        if (isUsedAsToggle()) {
           return toggle_names.front();
        } else
           return name;
}

void LObject::clear() {
        name = snull;
        is_toggle = false;
        used_toggle = false;
        dtoggle = false;
        //modifier = 0;
        event_type = PRESS;
        type  = CODE;
	modifiers.clear();
	commands.clear();
	commandDisplayNames.clear();
	tog_commands.clear();
	tog_commandDisplayNames.clear();
	//toggle_names.clear();
	//modifiers.push_back(0);
	blank = "";
	empty.setCommand("");

}
// Clear All Toggle Data
void LObject::clearToggleData() {
   blank = "";
   empty.setCommand("");
   used_toggle = false;
   dtoggle = false;
   tog_commands.clear();
   tog_commandDisplayNames.clear();
   //toggle_names.clear();
   while ( !toggle_names.empty())
	   toggle_names.pop();
}
// Clear All Modifier Data
void LObject::clearModifierData() {
   blank = "";
   empty.setCommand("");
   modifiers.clear();
   commands.clear();
   commandDisplayNames.clear();
}

// Remove a command for a modifier key.
void LObject::removeCommand(unsigned int mod) {
	if (hasModifier(mod) && modifiers.size() != 0) {
	   // Remove the modifier from the vector.
	   vector<unsigned int>:: iterator it = modifiers.begin();
	   while (*it != mod) it++;
	   if (it != modifiers.end()) modifiers.erase(it);
	
	   // Remove the modifier command
           commands.erase(mod);
	   // Remove the modifier command display name.
	   commandDisplayNames.erase(mod);
	}
	cout << "exiting removeCommand" << endl;
}

// Remove a command for a toggleable key.
void LObject::removeCommand(const string& togname) {
	if (ownsName(togname)) {
           // Remove the toggle command.
	   tog_commands.erase(togname);
	   // Remove the toggle command name.
	   tog_commandDisplayNames.erase(togname);
	   // erase the name from toggle_names.
	   string s;
	   queue<string>::size_type si = toggle_names.size();
	   for (queue<string>::size_type i = 0; i < si ; i++) {
              s = toggle_names.front();
              toggle_names.pop();
	      if ( s != togname )
                 toggle_names.push(s);
           }
	}
}

ostream & operator<<(ostream &out, LObject &rhs) {
        //rhs.serialize(out);
        rhs.print(out);
        return out;
}
