/***************************************************************************
                          lbutton.h  -  description
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

#ifndef LBUTTON_H
#define LBUTTON_H

#include <string>
#include <vector>
#include <map>
#include <queue>

#include <stdio.h>

#include <lineak/definitions.h>
#include <lineak/lconfigdata.h>
#include <lineak/lcommand.h>
#include <lineak/lobject.h>

/**This class contains a button.
  *@author Sheldon Lee Wen
  */
class LButton : public LObject {
   public:
      LButton();
      LButton(string name, unsigned int ibutton);
      virtual ~LButton();

      virtual void print(ostream&out);
      //virtual void serialize(ostream&out);
      void setButton(unsigned int button);

      const unsigned int getButton() {
         return button;
      }

   private:
      unsigned int button;

};

ostream & operator<<(ostream &out, LButton &rhs);

#endif
