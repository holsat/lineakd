/***************************************************************************
                          lcommand.cpp  -  description
                             -------------------
    begin                : ?  8? 20 2003
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

#include <lcommand.h>
#include <lineak_util_functions.h>
#include <iostream>
using namespace std;


vector<string> LCommand::specials;
bool LCommand::specials_init = false;

LCommand::LCommand() {
  isempty = true;
  ismacro = false;
  separator = ',';
  command = snull;
  macro_type = snull;
  args.clear();
}

LCommand::LCommand(string icommand) {
   isempty = false;
   ismacro = false;
   separator = ',';
   macro_type = snull;
   args.clear();
   command = icommand;
    //   command = lineak_util_functions::strip_quotes(icommand);
    // Now, see if this thing is a macro, and if so, get the arguments.
   parse();
}

/* Copy constructor */
LCommand::LCommand(const LCommand& rhs) {
 if (this != &rhs ) {
   isempty = rhs.isempty;
   ismacro = rhs.ismacro;
   separator = rhs.separator;
   macro_type = rhs.macro_type;
   command = rhs.command;
   args.clear();
   for (vector<string>::const_iterator it = rhs.args.begin(); it != rhs.args.end(); it++ )
   	args.push_back(*it);
  }
}
/** UPdate the list of macros we support. */
void LCommand::setMacros(const vector<string>& macrolist) {
	if (specials_init)
		specials.clear();
		
	specials = macrolist;
	specials_init = true;
}
LCommand &LCommand::operator=(const LCommand& rhs) {
 if (this != &rhs ) {
   isempty = rhs.isempty;
   ismacro = rhs.ismacro;
   separator = rhs.separator;
   macro_type = rhs.macro_type;
   command = rhs.command;
   args.clear();
   for (vector<string>::const_iterator it = rhs.args.begin(); it != rhs.args.end(); it++ )
   	args.push_back(*it);
  }
  return *this;
}
bool LCommand::operator==(const LCommand& rhs) {
 if (this != &rhs ) {
   if (isempty != rhs.isempty) return false;
   if (ismacro != rhs.ismacro) return false;
   if (separator != rhs.separator) return false;
   if (macro_type != rhs.macro_type) return false;
   if (command != rhs.command) return false;

   if (args.size() == rhs.args.size() ) {
      vector<string>::const_iterator it = args.begin();
      vector<string>::const_iterator rit = rhs.args.begin();
      for (; it != args.end() && rit != rhs.args.end(); ) {
         if (*it != *rit) return false;	
         it++;
         rit++;
      }
   }
   else
      return false;
 }
 return true;
}

void LCommand::parse() {
  if (command != snull) {
	//cout << "Striping command in LCommand::parse()" << endl;
  	command = lineak_util_functions::strip(command, "\"#");
    	macro_type = getMacroType();
    	isempty = false;
    if (macro_type != snull)
    {
      getMacroArgs();
    }
  }
}
LCommand::~LCommand(){
}
//void LCommand::remArg(string arg) {
//
//  VecIter it = args.find(arg);
//  args.erase(it);
//}

/* You should not need to call this. The default argument separator is a comma. It is set in both constructors to the default.
    If however, you do change the separator, and (provided that command is not snull), you need to call parse() */
void LCommand::setSeparator(string sep) {
    separator = sep;
    if (command != snull)
      parse();
    else {
      isempty = false;
      ismacro = false;
      macro_type = snull;
      args.clear();
    }
}

/** Determines if the command is a special EAK command or not. */
bool LCommand::isMacro(){
    /** go through our list of special commands and if the command contains those characters
          return true */
	if (specials_init) {
    		for ( vector<string>::iterator it=specials.begin(); it < specials.end(); it++) {
      			if ( command.find(*it) == 0) {
      			//if ( macro == *it ) {
        			return true;
      			}
    		}
	} else
		cerr << "Macrolist has not been set!" << endl;
	return false;
}
/** Determine the type of Macro this command is and return */
string LCommand::getMacroType() {
	if (specials_init) {
           string macro;
           macro = command.find('(');
           if (command.find('(') == string::npos)
              macro = command;
           else
              macro = command.substr(0,command.find('('));

           //if (verbose) cout << "isMacro(): macro = " << macro << endl;

    		for ( vector<string>::iterator it=specials.begin(); it < specials.end(); it++) {
        		//if (command.find(*it) == 0) {
        		if (macro == *it) {
          			ismacro = true;
          			return (*it);
        		}
    		}
	} else
		cerr << "Macrolist has not been set!" << endl;
    	ismacro = false;
    	return snull;
}
/** Later we want to do this on config file load. Parse in the loader and extend the LConfig class so that a command
    is not just a string, but a class that contains the command, and a vector<string> of arguments */
bool LCommand::getMacroArgs() {
    int e;
    string::size_type b;

    // Strip off the '(' and ')' characters. tmp contains the argument string.
    b = command.find('(');
    // If we don't find an opening brace, we have no arguments
    if (b == string::npos )
      return false;

    // So... we've found something to parse.
    string tmp = command.substr(b+1, command.length()-1);
    string tmp2;
    //cout << "macro args = " << tmp << endl;
    e = tmp.rfind(')');
    tmp = tmp.substr(0,e);
    //cout << "macro args = " << tmp << endl;
    // If we actually have no arguments, return snull.
    if ( tmp == "") {
//      args.clear();
      return false;
    }

    /** We have arguments now.
          Traverse the remaining string, and rip out the arguments. The string should look like this:
          "/dev/cdrom,/devcdrom1"  or
          "5, /dev/mixer"  or
          "-1"
    */
    do {
      e = tmp.find(separator);
      if (e <0) {
        tmp = lineak_util_functions::unescape(tmp, "\"#");
        args.push_back(tmp);
        break;
      } else {
        tmp2 = lineak_util_functions::unescape(tmp.substr(0,e), "\"#");
        args.push_back(tmp2);
        tmp = tmp.substr(e+1,tmp.length());
      }
    } while (tmp != "" );

    return true;
}

//#ifdef _DEBUG_
void LCommand::print(ostream &out) {
  out << "command = " << command << endl;
  out << "separator = " << separator << endl;
  out << "isempty = " << isempty << endl;
  out << "ismacro = " << ismacro << endl;
  if (ismacro) {
    out << "   macro_type = " << macro_type << endl;
    out << "   There are: " << args.size() << " macro arguments: { ";
	for (vector<string>::const_iterator it = args.begin(); it != args.end(); it++)
		out << *it << " ";
	out << "} " << endl;
  }

}
//#endif

ostream & operator<<(ostream &out, LCommand &rhs) {
  out << rhs.getCommand() << endl;
  return out;
}
ostream & operator<<(ostream &out, const LCommand &rhs) {
  out << const_cast<LCommand &>(rhs).getCommand() << endl;
  return out;
}
