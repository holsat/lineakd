/***************************************************************************
                          lcommand.h  -  description
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

#ifndef LCOMMAND_H
#define LCOMMAND_H
#include <config.h>
#include <lineak/definitions.h>
#include <string>
#include <vector>

/**Class containing a lineak command. It may be just a string, or a lineak macro with arguments.
  *@author Sheldon Lee Wen
  */
using namespace std;
class LCommand {
public:
    LCommand();
    LCommand(string icommand);
    LCommand(const LCommand& rhs);
    LCommand &operator=(const LCommand& rhs);
    bool operator==(const LCommand& rhs);
    ~LCommand();

    /* Return the original command, as parsed from the config file */
    inline const string& getCommand() { return command; }
    /* Set the command and parse. */
    inline void setCommand(string icommand) { command = icommand;
                                                                            parse(); }
    /** Return a list of the arguments */                                                                            
    inline const vector<string>& getArgs() { return args; };
    inline void addArg(string arg) { args.push_back(arg); }
//    void remArg(string arg);
    /* You should not need to call this. The default argument separator is a comma. It is set in both constructors to the default.
        If however, you do change the separator, and (provided that command is not null), you need to call parse() */
    void setSeparator(string sep);
    inline const string& getSeparator() { return separator; }
    inline const int numArgs() { return args.size(); }
    inline bool& isEmpty() { return isempty; }
    /** According to the macros list in specials, are we a macro? */
    bool isMacro();
    /** Search through the list of macros defined in the specials and return the one that matches */
    string getMacroType();
    /** Called when command is set, either from the constructor, or from setCommand.
          It should be called if the Seperator is set manually with setSeperator. It will have no
          effect, i.e. do nothing, if command is null, or isMacro returns false.
          It determines if the command is a macro, what type of macro, and get the arguments.
          Be aware that it clears any arguments that may have been manually added with the addArg()
          method */
    void parse();
    //#ifdef _DEBUG_
    void print(ostream &out);
    //#endif
    static void setMacros(const vector<string>& macros);
private:
    // Parse the string for macro arguments.
    bool getMacroArgs();
    static vector<string> specials;
    static bool specials_init;
    string command;
    string separator;
    string macro_type;
    vector<string> args;
    bool isempty,ismacro;
};

ostream & operator<<(ostream &out, LCommand &rhs);
ostream & operator<<(ostream &out, const LCommand &rhs);

#endif
