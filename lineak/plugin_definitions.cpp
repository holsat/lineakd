/***************************************************************************
                          plugin_definitions.cpp  -  description
                             -------------------
    begin                : ? 10? 27 2003
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

#include <lineak/plugin_definitions.h>

ostream & lineak_plugins::operator<<(ostream &out, identifier_info &rhs) {
    out << "description = " << rhs.description << endl;
    out << "identifier = " << rhs.identifier << endl;
    out << "type = " << rhs.type << endl;
    out << "version = " << rhs.version << endl; 
    return out;
}
   
ostream & lineak_plugins::operator<<(ostream &out, macro_info &rhs) {
     out << "num_macros = " << rhs.num_macros << endl;
     for (int i = 0; i <= rhs.num_macros; i++ ) {
        out << "macro_list[" << i << "] = " << (rhs.macro_list)[i] << endl;
	out << "macro_info[" << i << "] = " << (rhs.macro_info)[i] << endl;
     }
   return out;
}

ostream & lineak_plugins::operator<<(ostream &out, directive_info &rhs) {
      rhs.directives->print(out);
      return out;
}
   
ostream & lineak_plugins::operator<<(ostream &out, init_info &rhs) {
   out << "verbose = " << rhs.verbose << endl;
   out << "global_enable = " << rhs.global_enable << endl;
   //out << "--------- kbd --------" << endl;
   //out << *(rhs.kbd);
   return out;
}

ostream & lineak_plugins::operator<<(ostream &out, plugin_info &rhs) {
      out << "filename = " << rhs.filename << endl;
      out << "identifier = " << *(rhs.identifier) << endl;
      out << "initialized_display = " << rhs.initialized_display << endl;
      out << "directives_defined = " << rhs.directives_defined << endl;
      out << "macros_defined = " << rhs.macros_defined << endl;
      out << "loaded = " << rhs.loaded << endl;
      out << "initialized = " << rhs.initialized << endl;
      for (vector<string>::size_type i = 0; i < rhs.macros.size(); i++) {
	      out << "macro[" << i << "] = " << rhs.macros[i] << endl;
      }
      out << rhs.directives << endl;
      return out;
}
