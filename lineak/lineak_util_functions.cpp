/***************************************************************************
                          lineak_util_functions.cpp  -  description
                             -------------------
    begin                : Sat Jan 18 2003
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

#include <lineak/lineak_util_functions.h>
#include <lineak/definitions.h>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

// C headers
extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
extern int errno;
}
using namespace std;

bool lineak_util_functions::remove_file( string name ) {
  if (remove(name.c_str()) == -1 ) {
	#ifdef _DEBUG_
    cerr << strerror(errno) << ": " << name << endl;
    #endif
    return false;
  }
  return true;
}  
/* Check that the file exists */
bool lineak_util_functions::file_exists( string filename) {
	FILE *fp;
  	if ((fp = fopen(filename.c_str(), "r"))!=NULL) {
    	fclose (fp);
        return true;
  	} else  {
   		#ifdef _DEBUG_
        cerr << strerror (errno) << ": " << filename << endl;
        #endif
        return false;
    }
}
/* Check that the directory exists */
bool lineak_util_functions::dir_exists( string dirname ) {
	DIR *dp;
	if ((dp = opendir(dirname.c_str())) != NULL) {
		closedir (dp);
		return true;
	} else {
   		#ifdef _DEBUG_
	    cerr << strerror (errno) << ": " << dirname << endl;
        #endif
		return false;
	}
}

string lineak_util_functions::strip_space ( string store ) {
    string tmp = store;
    string result = snull;
    //cout << "Stripping control characters" << endl;
    // Strip out all control characters first.
    for (string::size_type i = 0; i <= tmp.length(); i++) {
    	if (!isprint(tmp[i]))
     		tmp.erase(i,1);
	}
    //cout << "Stripping out spaces" << endl;
	// Strip out the trailing spaces
    //cout << "tmp.length() = " << tmp.length() << endl;
    //cout << "tmp=" << tmp << "|" << endl;
    if ( tmp == " " ) {
	    tmp = "";
	    return tmp;
    }
	    
    string::size_type a = tmp.length()-1;
    string::size_type b = 0;
    //cout << "doing a" << endl;
    while (( a > 0 && a <= tmp.length()) && isspace(tmp[a])) a--;
//		    cout << "doing b" << endl;
    while (b <= tmp.length() && isspace(tmp[b])) b++;
  //  cout << "a = " << a << endl;
    //cout << "b = " << b << endl;
    //cout << "(a-b)+1 = " << (a-b)+1 << endl;
    result = tmp.substr(b,(a-b)+1);
    return result;
}
/** Strip quotes and hashes unless they are escaped. */
string lineak_util_functions::strip( const string istring, const string chars ) {
    string store = istring;
    string::size_type index=0;
    string::size_type j = 0;
    //cout << "strip( " << istring << ", " << chars << " )" << endl;    
    for ( string::size_type i = 0; i < chars.size(); i++)
    {
      char c = chars[i];
      //cout << "Stripping char: " << c << endl;
      while (store.find(c, index) != string::npos) {
        j = store.find(c, index);
        if (j < 1 || store[j-1] != '\\') {
	   //cout << "j: " << j << " store[j]: " << store[j] << " store[j-1]: " << endl;
	   store.erase(j,1);
           index = j;
        }
        else {
	   index = j+1;
	   //cout << "index: " << index << endl;
	}
	//cout << "store = " << store << endl;
      }
      //cout << "final store = " << store << endl;
   }
   return store;
}
/** Remove the escaping from the characters in the chars variable. */
string lineak_util_functions::unescape( const string istring, const string chars ) {
    string store = istring;
    string::size_type index=0;
    string::size_type j = 0;
    for ( string::size_type i = 0; i < chars.size(); i++)
    {
      char c = chars[i];
//      cout << "looking for: " << c << endl;
          while (store.find(c, index) != string::npos) {
	     j = store.find(c, index);
//          cout << "index = " << j << endl;
	    	if (j >= 1 && store[j-1] == '\\') {
    //            cout << "erasing character " << store[j-1] << "in string " << istring << endl;
				store.erase(j-1,1);
				index = j;
			}
			else
				index = j+1;
	  }
    }
    return store;
}
/** Escape the characters in the chars string. */
string lineak_util_functions::escape( const string istring, const string chars ) {
    string store = istring;
    string::size_type index=0;
    string::size_type j = 0;
    for ( string::size_type i = 0; i < chars.size(); i++)
    {
      char c = chars[i];

      while (store.find(c, index) != string::npos) {
         j = store.find(c, index);
	    	if (j >= 1 && store[j-1] != '\\') {
				store.insert(j,"\\");
				index = j+2;
			}
			else
				index = j+1;
	  }
    }    
  
	return store;
}
void lineak_util_functions::inspect_vector(const vector<string>& v) {
	cout << "vector has " << v.size() << " elements: { ";
	for (vector<string>::const_iterator it = v.begin(); it != v.end(); it++)
		cout << *it << " ";
	cout << "} " << endl;
}
int lineak_util_functions::strcmp_nocase(const string & s1, const string& s2) 
{
  string::const_iterator it1=s1.begin();
  string::const_iterator it2=s2.begin();

  //stop when either string's end has been reached
  while ( (it1!=s1.end()) && (it2!=s2.end()) ) 
  { 
    if(::toupper(*it1) != ::toupper(*it2)) //letters differ?
     // return -1 to indicate smaller than, 1 otherwise
      return (::toupper(*it1)  < ::toupper(*it2)) ? -1 : 1; 
    //proceed to the next character in each string
    ++it1;
    ++it2;
  }
  size_t size1=s1.size(), size2=s2.size();// cache lengths
   //return -1,0 or 1 according to strings' lengths
    if (size1==size2) 
      return 0;
    return (size1<size2) ? -1 : 1;
}
