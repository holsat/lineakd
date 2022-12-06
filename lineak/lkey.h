/***************************************************************************
                          lkey.h  -  description
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

#ifndef LKEY_H
#define LKEY_H

#include <string>
#include <vector>
#include <map>
#include <queue>

#include <stdio.h>
#include <X11/Xlib.h>

#include <lineak/lconfigdata.h>
#include <lineak/lcommand.h>
#include <lineak/lobject.h>

//typedef unsigned long KeySym; 

/**This class contains a key or button.
  *@author Sheldon Lee Wen
  */
class LKey : public LObject {
   public:
      LKey();
      LKey(string name, int code);
      virtual ~LKey();

      void clearKeycount() {
         keycount = 0;
      }
      virtual void print(ostream&out);
      //virtual void serialize(ostream &out);
      //virtual void unserialize(istream &in);
      void setKeySym(KeySym ikeysym);
      void setKeyCode(KeyCode code);

      KeySym getKeySym() const {
         return keysym;
      }
      int getKeyCode() const {
         return keycode;
      }

   private:
      //void init();
      KeySym keysym;
      /** Using KeyCode instead of int, leads to strange things. */
      //KeyCode keycode;
      int keycode;
      static int keycount;

};

ostream & operator<<(ostream &out, LKey &rhs);
//istream & operator>>(istream &in, LKey &rhs);

#endif
