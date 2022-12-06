// C++ Interface: lobject
//
// Description: Lineak Object which is the base class for LKey and 
//   LButton.
//
// Author: Sheldon Lee Wen <tormak@rogers.com> (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
#ifndef LOBJECT_H
#define LOBJECT_H

#include <string>
#include <vector>
#include <map>
#include <queue>

#include <stdio.h>
//#include <X11/Xlib.h>
//#include <X11/XKBlib.h>
//#include <X11/extensions/XKBfile.h>

#include <lineak/definitions.h>
#include <lineak/lconfig.h>

class LCommand;

typedef enum { PRESS, RELEASE, UNKNOWN_EVENT } EventType_t;
typedef enum { SYM, CODE, BUTTON, UNKNOWN_KEY } KeyType_t;

using namespace std;
/**
   Base object from which LKey and LButton inherits.
   It is initialized with a name, event type, and key type.
   OK This class is a mess, and it sucks.

   @author Sheldon Lee Wen.
   
*/
class LObject {
   public:
      LObject();
      LObject(string iname, EventType_t ietype, KeyType_t itype);
      virtual ~LObject();
      //bool operator==(LObject & rhs);
      /** This method gets the name of the object initially defined in the constructor. 
          (which is set from the unparsed key name from the keyboard definition file. 
          It returns this name. */
      virtual inline string getName() { return name; }  
      virtual inline void setName(string iname) {name = iname;}
      virtual inline const vector<unsigned int>& getModifiers() const { return modifiers; }
      virtual inline const EventType_t getEventType() const { return event_type; }
      virtual inline const KeyType_t getType() const { return type; }
      virtual void addModifier(unsigned int imodifier);
      virtual inline void setEventType(EventType_t iet) { event_type = iet; }
      virtual inline void setType(KeyType_t itype) { type = itype; }
      /** Get a command. The command that gets returned depends on a few things:
      		If the command is a togglable command, we return the command corresponding to the name at the front of the toggle_names queue.
		If the command is not a toggleable command, and we call it with mod = 0, or the default way, we return the value of the default unmodified command for this object.
		If the command is not a toggleable command, and we call it with mod != 0, return the command that corresponds to that modifier.
      */
      virtual LCommand & getCommand(unsigned int mod = 0);
      // Get the command corresponding to toggle name togname
      virtual LCommand & getToggleCommand(const string& togname);
      // Get the display name associated with the command of modifier mod
      virtual string & getCommandDisplayName(unsigned int mod = 0);
      // Get the display name corresponding to command for toggle name togname
      virtual string & getToggleCommandDisplayName(const string& togname);
      /** Set a command corresponding to a modifier sequence. If mod = 0, set it as the default command. */
      virtual void setCommand(LCommand command, unsigned int mod = 0);
      /** Set a display name corresponding to a modifier sequence. By default set the default display name. */
      virtual void setCommandDisplayName(string dname, unsigned int mod = 0);
      //void setCommand(LCommand command);
      virtual string getModifierString(unsigned int modifier);
      virtual string getEventTypeString();
      virtual string getTypeString();
      virtual void print(ostream &out);
      //virtual void serialize(ostream &out);
      //virtual void unserialize(/*istream &in*/) {/** This doesn't do anything yet*/};
      
      // TOGGLE SHIT. This is where this gets nasty, probably as a result of poor OO design on my part.
      /** Return whether or not this is a toggleable key. The distinction is
          necessary for on screen display where you have a config file entry of the
	  form:
             key|key = commandm
          In this case the key is not being used as toggleable in the sense that
          each key press is bound to a different command. We are using it as
          a normal key. However, for the sake of the on screen display, we want
          to toggle the display. */
      //inline bool isDisplayToggle() const { return dtoggle; }
      /** Return whether or not we are using this key as a toggleable key */
      virtual bool isUsedAsToggle() const;
      virtual bool isToggle() const;
      virtual void setUsedAsToggle(bool tog);
      //inline void setDisplayToggle(bool tog) { dtoggle = tog; }
      /** If isToggle() , rotate the key to the next display name. */
      virtual void toggleState();
      /** If it is a toggleable key, return the command for the next state 
          (determined by the next display name in the queue. */
      /** Add a toggleable name. This automatically sets the toggleable status,
          and adds the name to the display_names queue. */
      virtual void addToggleName(const string iname);
      virtual void setToggleCommandDisplayName(string dname, const string iname);
      virtual vector<string> getToggleNames();
      /** This method returns true if this object is named, or has a toggleable name
          that matches iname. */
      virtual bool ownsName(string iname);
      virtual bool hasModifier(const unsigned int modifier);
      virtual string getNextToggleName();
      virtual void setCommand(LCommand command, string iname);
      //virtual void setToggleModifier(string iname, unsigned int imodifier);
      virtual void clear();
      // Clear all data for the toggle part of the key.
      virtual void clearToggleData();
      // Clear all modifier data.
      virtual void clearModifierData();
      vector<keycommand_info> getKeycommand_info();

      // Remove a command.
      virtual void removeCommand(unsigned int mod = 0);
      virtual void removeCommand(const string& togname);
   protected:
      string name;
      string blank;
      LCommand empty;
      EventType_t event_type;
      KeyType_t type;
      // Keep a list of all modifiers we respect.
      vector<unsigned int> modifiers;
      // Keep a list of commands indexed by modifiers.
      map<unsigned int,LCommand> commands;
      map<unsigned int,string> commandDisplayNames;
      
      bool used_toggle,is_toggle,dtoggle;
      map<string,LCommand> tog_commands;
      map<string,string> tog_commandDisplayNames;
      queue<string> toggle_names;
      
};

ostream & operator<<(ostream &out, LObject &rhs);

#endif
