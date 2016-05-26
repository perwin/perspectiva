/*! \file
   \brief  Utility functions for string processing, testing file existence, etc.
 */

#ifndef _UTILITIES_PUB_H_
#define _UTILITIES_PUB_H_

#include <string>
#include <vector>

using namespace std;

/* constants for use parameter "restriction" when calling NotANumber(): */
const int kAnyInt     =    0;
const int kNonzeroInt =    1;
const int kPosInt     =    2;
const int kAnyReal    =    3;
const int kPosReal    =    4;


// String-processing functions

/// like fprintf, but returns a string instead of writing to a file
string PrintToString( const char *fmt, ... );


/// \brief Splits a string and returns substrings ("tokens").
///
/// The vector tokens is cleared before adding the substrings.
void SplitString( const string& str, vector<string>& tokens, 
									const string& delimiters = "\n\t " );

/// \brief Same as SplitString, but the pieces of the input string are *added* to the
///        tokens vector, instead of the tokens vector being cleared first
void SplitStringAdd( const string& str, vector<string>& tokens, 
									const string& delimiters = "\t " );

/// Modifies inputString to remove all characters from the first appearance of the delimiter onwards
void ChopComment( string& inputString, char delimiter = '#' );

/// Removes leading and trailing whitespace ("\t ") from a string
void TrimWhitespace( string& stringToModify );


/// Checks to see if (local) file exists
bool FileExists( const char * filename );


/// Returns string with current date-and-time (Dow Mon dd hh:mm:ss yyy)
char * TimeStamp( void );


/// Prints "Error in command line:" + errorString, then calls exit(1)
void CommandLineError( char errorString[] );


/// Returns true if string is *not* a number of correct subtype (restriction)
bool NotANumber( const char theString[], const int index, int const restriction );


/// \brief Returns 1 if entire string is a valid number (positive or negative, including floating-point);
///        returns 0 if not.
int IsNumeric( const char *aString );



#endif /* _UTILITIES_PUB_H_ */
