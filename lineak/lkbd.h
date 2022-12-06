/***************************************************************************
                          lkbd.h  -  description
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

#ifndef LKBD_H
#   define LKBD_H

#   include <string>
#   include <vector>
#   include <map>
#   include <queue>

#   include <stdio.h>

#   include <lineak/definitions.h>
#   include <lineak/lconfigdata.h>
#   include <lineak/lobject.h>
/**Lineak Keyboard
  *@author Sheldon Lee Wen
  */

class LConfig;
class LCommand;


class LKbd: public LConfigData
{
 public:
   LKbd ();
   LKbd (string iname, string ibrand, string imodel);
   virtual ~ LKbd ();

   string name;
   string brand;
   string model;

   /** Add an object to the map indexed by it's name. */
   void addObject ( LObject * object );
   /** Set the commands on the lobjects with commands parsed from the config file. */
   void setCommands (LConfig & config);
   /** Set the command for an lobject indexed by name */
   void setCommand (string name, LCommand command);
   bool operator== (LKbd & rhs);
   LKbd & operator=(const LKbd & rhs);
   
   /** Return a vector of all of the keycodes for all keys. */
   vector < int > getKeyCodes ();
   vector < unsigned int> getButtonCodes ();
   /** Return a vector of all of the names which index lobjects. This does not look
       for toggleable names. */
   vector < string > getNames ();
   /** Get the table of objects indexed by object names. */
   const map < string, LObject* > &getObjects () const;
   const map < string, LObject* > getObjectsOfType(KeyType_t type) const;
   /** Return an lobject by it's name, or if it owns a toggle name that matches. */
   LObject * getObject (string name);
   /** Look through the key lobjects and return the one with the matching keycode. */
   LObject * getObject (int keycode);
   /*** Look through the button lobjects and return the one with the matching button */
   LObject * getObject (unsigned int button);
   /** Remove an lobject by it's name. */
   void removeObject(string name);
   /** Remove an object by it's keycode */
   void removeObject(int keycode);
   /** Remove an object by it's button code */
   void removeObject(unsigned int button);
   /** Return the number of objects of type. */
   int getNumObjectOfType(KeyType_t type);

   /** Is the lkbd object empty? */
   const bool isEmpty ();
   /** Print */
   void print (ostream & out);
   /** Clear the object */
   void clear();

   /** Add a low level command. */
   void addRawCommand(string name, string rcomm);
   string getRawCommand(string name) { return raw_commands[name]; }
    
   
 private:
     map < string, LObject* > table;
     LObject empty_object;
     map < string, string > raw_commands;

};

//ostream & operator<<(ostream &out, LKbd &rhs);
#endif
