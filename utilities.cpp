/* FILE: utilities.cpp --------------------------------------------- */
/*   Several utility routines used by imfit, makeimage, etc.
 */

// Copyright 2010, 2011, 2012, 2013 by Peter Erwin.
// 
// This file is part of Imfit.
// 
// Imfit is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// Imfit is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License along
// with Imfit.  If not, see <http://www.gnu.org/licenses/>.


#include <ctype.h>   /* for isdigit() */
#include <stdio.h>
#include <stdlib.h>  /* for exit() */
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdarg>

using namespace std;

#include "utilities_pub.h"

/* ------------------- Function Prototypes ----------------------------- */
/* Local Functions: */
string vformat( const char *fmt, va_list ap );



// The following two functions are taken from 
// http://stackoverflow.com/questions/69738/c-how-to-get-fprintf-results-as-a-stdstring-w-o-sprintf#69911
// and are originally written by Larry Gritz, August 2009
// ("This code is part of an open source project that I'm writing (BSD license), 
// so everybody feel free to use this as you wish.")

// Public function, which is mostly a wrapper around vformat
string PrintToString( const char *fmt, ... )
{
  va_list ap;
  va_start(ap, fmt);
  string  buf = vformat(fmt, ap);
  va_end(ap);
  return buf;
}

// Main function which does the work for PrintToString
string vformat( const char *fmt, va_list ap )
{
  // Allocate a buffer on the stack that's big enough for us almost
  // all the time.  Be prepared to allocate dynamically if it doesn't fit.
  size_t  size = 1024;
  char  stackbuf[1024];
  vector<char>  dynamicbuf;
  char *buf = &stackbuf[0];

  while (1) {
    // Try to vsnprintf into our buffer.
    int  needed = vsnprintf(buf, size, fmt, ap);
    // NB. C99 (which modern Linux and OS X follow) says vsnprintf
    // failure returns the length it would have needed.  But older
    // glibc and current Windows return -1 for failure, i.e., not
    // telling us how much was needed.

    if (needed <= (int)size && needed >= 0) {
      // It fit fine so we're done.
      return string(buf, (size_t)needed);
    }

    // vsnprintf reported that it wanted to write more characters
    // than we allotted.  So try again using a dynamic buffer.  This
    // doesn't happen very often if we chose our initial size well.
    size = (needed > 0) ? (needed+1) : (size*2);
    dynamicbuf.resize(size);
    buf = &dynamicbuf[0];
  }
}



/* ---------------- FUNCTION: SplitString() ------------------------ */
/// This function tokenizes a string, splitting it into substrings using
/// delimiters as the separator (delimiters can be more than one character, in
/// which case all of them can serve as delimiters).  The substrings are
/// added to the user-supplied vector<string> tokens.
/// The default value for delimiter is "\n\t ", meaning newlines, tabs, and spaces.
void SplitString( const string& str, vector<string>& tokens, const string& delimiters )
{

  tokens.clear();

  // Skip delimiters at beginning.
  string::size_type  lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type  pos = str.find_first_of(delimiters, lastPos);
  if (pos == string::npos) {
    // oops, no delimiters in this string, so just return the whole string
    tokens.push_back(str);
  }
  else {
    while (string::npos != pos || string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }
  }
}


/* ---------------- FUNCTION: SplitStringAdd() --------------------- */
/// Same as SplitString, but the pieces of the input string are *added* to the
/// tokens vector, instead of the tokens vector being cleared first
void SplitStringAdd( const string& str, vector<string>& tokens, const string& delimiters )
{
  // Skip delimiters at beginning.
  string::size_type  lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type  pos = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}


/* ---------------- FUNCTION: ChopComment() ------------------------ */
// This function removes the remainder of line after a comment character
// (latter is specified by delimiter, which defaults to '#')
void ChopComment( string& inputString, const char delimiter )
{
  string::size_type  loc;
  
  loc = inputString.find(delimiter, 0);
  inputString = inputString.substr(0, loc);
}


/* ---------------- FUNCTION: TrimWhitespace() --------------------- */
// This function removes leading and trailing whitespace from a string; if
// the string is *all* whitespace, then it converts the input string to an
// empty string.  ("Whitespace" = newlines, spaces, or tabs)
void TrimWhitespace( string& stringToModify )
{
  if (stringToModify.empty())
    return;

  string::size_type  startIndex = stringToModify.find_first_not_of(" \t\n");
  string::size_type  endIndex = stringToModify.find_last_not_of(" \t\n");
  if (startIndex == endIndex)
    stringToModify.clear();
  else
    stringToModify = stringToModify.substr(startIndex, (endIndex - startIndex + 1) );
}



/* ---------------- FUNCTION: FileExists() ------------------------- */

bool FileExists(const char * filename)
{
  return (bool)ifstream(filename);
}



/* ---------------- FUNCTION: TimeStamp() -------------------------- */

char * TimeStamp( void )
{  
  time_t  currentTime;
  char *dateString;
  
  currentTime = time(NULL);
  dateString = ctime(&currentTime);
  // Hack! Shift the null-termination up one character to knock out the
  // '\n' which otherwise ends the string.  This works because the
  // output of ctime() is supposed to be a 26-character string.
  dateString[24] = '\0';
  
  return dateString;
}



/* ---------------- FUNCTION: CommandLineError() ------------------- */

void CommandLineError( const char errorString[] )
{

  fprintf(stderr, "Error in command line:\n   %s\nExiting...\n",
	 errorString);
  exit(1);
}



/* ---------------- FUNCTION: NotANumber() ------------------------- */
// Possible cases:
//    0, 0.0, 0.1, .1
//    -0.1, -.1?
//    -1
bool NotANumber( const char theString[], const int index, const int restriction )
{
  int  theCharacter = theString[index];

  switch (restriction) {
    case kAnyInt:
      if (theCharacter == '-')
        return NotANumber( theString, index + 1, kAnyInt );
      else
        return (bool)( ! isdigit(theCharacter) );
    
    case kNonzeroInt:
      if (theCharacter == '-')
        return false;
      else
        return (bool)( ! isdigit(theCharacter) );
    
    case kPosInt:
      if ( isdigit(theCharacter) && (theCharacter != '0') )
        return false;
      else
        return true;
    
    case kAnyReal:
      switch (theCharacter) {
        case '-':
          return NotANumber( theString, index + 1, kAnyReal );
        case '.':
          return NotANumber( theString, index + 1, kAnyInt );
        default:
          return (bool)( ! isdigit(theCharacter) );
      }  /* end switch (theCharacter) */
    
    case kPosReal:
      // THIS STILL NEEDS WORK!
      switch (theCharacter) {
        case '-':
          return true;
        case '.':
          return NotANumber( theString, index + 1, kAnyInt );
        default:
          return (bool)( ! isdigit(theCharacter) );
      }  /* end switch (theCharacter) */
    
    default:
      return true;
  }  /* end switch (restriction) */
}


// returns 1 if input string can successfully be converted to double, 0 if not
int IsNumeric( const char *aString )
{
    char *p;

    if (aString == NULL || *aString == '\0' || isspace(*aString))
      return 0;
    strtod(aString, &p);
    return (*p == '\0');
}







/* END OF FILE: utilities.cpp -------------------------------------- */
